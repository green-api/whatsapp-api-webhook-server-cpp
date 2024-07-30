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
            Logger::Log("JSON is empty or invalid, aborting. This may be caused by trying to load URI with browser.", "error");
            resp.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
            std::ostream& out = resp.send();
            out << "This is a WhatsApp API Webhook server developed by Green-API.\nIf you want to make a request to this server, please view webhooks documentation: https://green-api.com/en/docs/api/receiving/notifications-format/\n\nGREEN API (c) 2015 - 2024";
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
        
        // Invalid TypeWebhook, aborting
        if (r.bodyJson["typeWebhook"].type() != nlohmann::json::value_t::string) {
            Logger::Log("TypeWebhook is invalid, it should be string, unable to handle request", "error");
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

        // Call user-defined functions for handling.
        // Based on returned value, set response status (true: 400 Bad Request, false: 200 OK)
        //
        // If your request hang up, please check your function is getting only existing fields in JSON.
        // Example:
        //      const auto val = body.bodyJson["not-existing-value"];
        // will cause debug assert (Debug) or abort function execution (Release)
        // Check for contains() if value may not be exists, example:
        //      const auto val = body.bodyJson.contains("not-existing-value") ? body.bodyJson["not-existing-value"] : nullptr;
        bool handleError = handleTypeWebhook(r.typeWebhook, r, resp);
        if (handleError) {
            resp.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        } else {
            resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        }

        // Write answer
        std::ostream& out = resp.send();
        out << "";
        out.flush();
        
    } else {
        // Token in Authorization header is wrong
        resp.setStatus(Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
        std::ostream& out = resp.send();
        out << "";
        out.flush();
    }
}

// This is a bridge between green-api library and user-defined functions.
// Returns: [true], if error; [false], if no error
// If true, response with 200 OK sent for client, otherwise: 400 Bad Request sent
bool RequestHandler::handleTypeWebhook(const std::string &typeWebhook, Response& body, Poco::Net::HTTPServerResponse &resp) {
    // If your request hang up, please check your function is getting only existing fields in JSON.
    // Example:
    //      const auto val = body.bodyJson["not-existing-value"];
    // will cause debug assert (Debug) or abort function execution (Release)
    // Check for contains() if value may not be exists, example:
    //      const auto val = body.bodyJson.contains("not-existing-value") ? body.bodyJson["not-existing-value"] : nullptr;

    bool error {true};
    if (body.error) {
        error = UserAdapter::onErrorValidation(body);
    }
    else if (typeWebhook == "incomingMessageReceived") {
        error = UserAdapter::onIncomingMessageReceived(body);
    }
    else if (typeWebhook == "deviceInfo") {
        error = UserAdapter::onDeviceInfo(body);
    }
    else if (typeWebhook == "incomingCall") {
        error = UserAdapter::onIncomingCall(body);
    }
    else if (typeWebhook == "outgoingAPIMessageReceived") {
        error = UserAdapter::onOutgoingAPIMessageReceived(body);
    }
    else if (typeWebhook == "outgoingMessageReceived") {
        error = UserAdapter::onOutgoingMessageReceived(body);
    }
    else if (typeWebhook == "outgoingMessageStatus") {
        error = UserAdapter::onOutgoingMessageStatus(body);
    }
    else if (typeWebhook == "stateInstanceChanged") {
        error = UserAdapter::onStateInstanceChanged(body);
    }
    else if (typeWebhook == "statusInstanceChanged") {
        error = UserAdapter::onStatusInstanceChanged(body);
    }
    else if (typeWebhook == "quotaExceeded") {
        error = UserAdapter::onQuotaExceeded(body);
    }
    else if (typeWebhook != "") {
        error = UserAdapter::onUnknownTypeWebhook(body);
    }
    return error;
}

// HandlerFactory needs Pattern and WebhookToken for passing it to request handler
RequestHandlerFactory::RequestHandlerFactory(const std::string &Pattern, const std::string &WebhookToken, nlohmann::json &JsonConfigData) {
    this->Pattern = Pattern;
    this->WebhookToken = WebhookToken;
    this->JsonConfigData = JsonConfigData;
}