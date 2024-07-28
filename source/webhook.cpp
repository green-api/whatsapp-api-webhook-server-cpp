#include "webhook.h"

using namespace greenapi;

//  Replaces string port with int port, creating server object
Webhook::Webhook(const std::string &Address, const std::string &Pattern, const std::string &WebhookToken) : Pattern(Pattern), WebhookToken(WebhookToken) {
    std::regex reg("[^0-9]");
    std::string port = std::regex_replace(Address, reg, "");
    nlohmann::json JsonConfigData = Config::OpenSchemas();
    serv = std::make_unique<Server>(std::stoi(port), Pattern, WebhookToken, JsonConfigData);
}

//  Starting server, blocking thread
void Webhook::StartServer() {
    serv->Start();
    std::promise<void>().get_future().wait(); // blocking server thread
}

// Server object creates new server instance
Server::Server(const int &Address, const std::string &Pattern, const std::string &WebhookToken, nlohmann::json &JsonConfigData) : Pattern(Pattern), WebhookToken(WebhookToken), JsonConfigData(JsonConfigData) {
    serverInstance = std::make_shared<Poco::Net::HTTPServer>(
        new RequestHandlerFactory(Pattern, WebhookToken, JsonConfigData), 
        Poco::Net::ServerSocket(Address), 
        new Poco::Net::HTTPServerParams
    );
}

// Starting server. This function will lock the thread
void Server::Start() {
    serverInstance->start();
    Logger::Log("Server started on port: " + std::to_string(serverInstance->socket().address().port()), "info");
}

// Passing Pattern and WebhookToken is nesessary because of checking request before handling it
RequestHandler::RequestHandler(const std::string &Pattern, const std::string &WebhookToken, nlohmann::json &JsonConfigData) {
    this->Pattern = Pattern;
    this->WebhookToken = WebhookToken;
    this->JsonConfigData = JsonConfigData;
}

// All requests to this server will be handled there
void RequestHandler::handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp) {
    // Get auth token if any
    std::string GivenToken = req.get("Authorization", "");
    if (GivenToken.rfind("Basic ", 0) == 0) {
        GivenToken = GivenToken.substr(6);
    } else if (GivenToken.rfind("Bearer ", 0) == 0) {
        GivenToken = GivenToken.substr(7);
    }
    // Default Pattern is url + "/". Requests with wrong patterns will be forwarded with 404 
    if (Pattern != req.getURI()) {
        resp.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
        std::ostream& out = resp.send();
        out.flush();
        return;
    }
    
    // If authorization complete
    if (WebhookToken == "" || GivenToken == WebhookToken) {
        // Reading body
        std::istream& in = req.stream();
        std::ostringstream ss;
        ss << in.rdbuf();
        std::string jsonStr = ss.str();

        // Handling wrong JSON
        if (!nlohmann::json::accept(jsonStr)) {
            Logger::Log("JSON is empty or invalid, aborting", "error");
            resp.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
            std::ostream& out = resp.send();
            out << "";
            out.flush();
            return;
        }

        // Preparing response structure
        Response r{};
        r.bodyStr = ""; // body = error text, if error; otherwise: raw request body 
        r.bodyJson = nlohmann::json::parse(jsonStr);

        // No TypeWebhook, unable to complete
        if (!r.bodyJson.contains("typeWebhook")) {
            Logger::Log("No typeWebhook, unable to handle request", "error");
            resp.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
            std::ostream& out = resp.send();
            out << "";
            out.flush();
            return;
        }

        // TypeWebhook is available, ready to validate
        r.typeWebhook = r.bodyJson["typeWebhook"];
        Validator::Validate(r, JsonConfigData);
        if (!r.error) {
            // Setting bodyStr to raw request body, if no error
            r.bodyStr = jsonStr;
        }

        // Setting the status based on validation
        if (!r.error) {
            resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        } else {
            resp.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        }
        std::ostream& out = resp.send();
        out << "";
        out.flush();

        // This will forward request to user-defined functions
        handleTypeWebhook(r.typeWebhook, r, resp);
    } else {
        // Token in Authorization header is wrong
        resp.setStatus(Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
        std::ostream& out = resp.send();
        out << "";
        out.flush();
    }
}

// This is a bridge between greenapi library and user-defined functions
void RequestHandler::handleTypeWebhook(const std::string &typeWebhook, Response& body, Poco::Net::HTTPServerResponse &resp) {
    if (typeWebhook == "incomingMessageReceived") {
        #ifdef ON_INCOMING_MESSAGE_RECEIVED_EXISTS
            UserAdapter::onIncomingMessageReceived(body);
        #endif
        return;
    }
    if (typeWebhook == "deviceInfo") {
        #ifdef ON_DEVICE_INFO_EXISTS
            UserAdapter::onDeviceInfo(body);
        #endif
        return;
    }
    if (typeWebhook == "incomingCall") {
        #ifdef ON_INCOMING_CALL_EXISTS
            UserAdapter::onIncomingCall(body);
        #endif
        return;
    }
    if (typeWebhook == "outgoingAPIMessageReceived") {
        #ifdef ON_OUTGOING_API_MESSAGE_RECEIVED_EXISTS
            UserAdapter::onOutgoingAPIMessageReceived(body);
        #endif
        return;
    }
    if (typeWebhook == "outgoingMessageReceived") {
        #ifdef ON_OUTGOING_MESSAGE_RECEIVED_EXISTS
            UserAdapter::onOutgoingMessageReceived(body);
        #endif
        return;
    }
    if (typeWebhook == "outgoingMessageStatus") {
        #ifdef ON_OUTGOING_MESSAGE_STATUS_EXISTS
            UserAdapter::onOutgoingMessageStatus(body);
        #endif
        return;
    }
    if (typeWebhook == "stateInstanceChanged") {
        #ifdef ON_STATE_INSTANCE_CHANGED_EXISTS
            UserAdapter::onStateInstanceChanged(body);
        #endif
        return;
    }
    if (typeWebhook == "statusInstanceChanged") {
        #ifdef ON_STATUS_INSTANCE_CHANGED_EXISTS
            UserAdapter::onStatusInstanceChanged(body);
        #endif
        return;
    }
    if (typeWebhook == "quotaExceeded") {
        #ifdef ON_QUOTA_EXCEEDED_EXISTS
            UserAdapter::onQuotaExceeded(body);
        #endif
        return;
    }
    else if (typeWebhook != "") {
        #ifdef ON_UNKNOWN_TYPEWEBHOOK_EXISTS
            UserAdapter::onUnknownTypeWebhook(body);
        #endif
        return;
    }
}

// HandlerFactory needs Pattern and WebhookToken for passing it to request handler
RequestHandlerFactory::RequestHandlerFactory(const std::string &Pattern, const std::string &WebhookToken, nlohmann::json &JsonConfigData) {
    this->Pattern = Pattern;
    this->WebhookToken = WebhookToken;
    this->JsonConfigData = JsonConfigData;
}