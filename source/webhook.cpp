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
        if (r.error == false) {
            // Setting bodyStr to raw request body, if no error
            r.bodyStr = jsonStr;
        }

        // This will forward request to user-defined functions
        handleTypeWebhook(r.typeWebhook, r, resp);       

        // Setting the status based on validation
        if (!r.error) {
            resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        } else {
            resp.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        }

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

// This is a bridge between greenapi library and user-defined functions
void RequestHandler::handleTypeWebhook(const std::string &typeWebhook, Response& body, Poco::Net::HTTPServerResponse &resp) {
    if (typeWebhook == "incomingMessageReceived")       { UserAdapter::onIncomingMessageReceived(body); return; }
    if (typeWebhook == "deviceInfo")                    { UserAdapter::onDeviceInfo(body); return; }
    if (typeWebhook == "incomingCall")                  { UserAdapter::onIncomingCall(body); return; }
    if (typeWebhook == "outgoingAPIMessageReceived")    { UserAdapter::onOutgoingAPIMessageReceived(body); return; }
    if (typeWebhook == "outgoingMessageReceived")       { UserAdapter::onOutgoingMessageReceived(body); return; }
    if (typeWebhook == "outgoingMessageStatus")         { UserAdapter::onOutgoingMessageStatus(body); return; }
    if (typeWebhook == "stateInstanceChanged")          { UserAdapter::onStateInstanceChanged(body); return; }
    if (typeWebhook == "statusInstanceChanged")         { UserAdapter::onStatusInstanceChanged(body); return; }
    else if (typeWebhook != "")                         { UserAdapter::onUnknownTypeWebhook(body); return; }
}

// HandlerFactory needs Pattern and WebhookToken for passing it to request handler
RequestHandlerFactory::RequestHandlerFactory(const std::string &Pattern, const std::string &WebhookToken, nlohmann::json &JsonConfigData) {
    this->Pattern = Pattern;
    this->WebhookToken = WebhookToken;
    this->JsonConfigData = JsonConfigData;
}