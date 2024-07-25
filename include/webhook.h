#ifndef WEBHOOK_H
#define WEBHOOK_H

#include <string>
#include <regex>
#include <memory>

#include <thread>
#include <future>
#include <iostream>
#include <sstream>

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>

#include "config.h"
#include "validator.h"
#include "response.h"
#include "user_adapter.h"


namespace greenapi {

    // Server creates RequestHandlerFactory which returns RequestHandler
    class Server {
        public:
            Server(const int &Address, const std::string &Pattern, const std::string &WebhookToken, nlohmann::json &JsonConfigData);
            void Start();
            //void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
        private:
            std::shared_ptr<Poco::Net::HTTPServer> serverInstance;
            std::string Pattern;
            std::string WebhookToken;
            nlohmann::json JsonConfigData;
    };

    // Starts blocking server in given Address
    class Webhook {
        public:
            Webhook(const std::string &Address, const std::string &Pattern, const std::string &WebhookToken);
            void StartServer();
        private:
            std::string Pattern;
            std::string WebhookToken;
            std::unique_ptr<Server> serv;
    };

    // Handles any requests by given Pattern. Address specified in Server
    class RequestHandler : public Poco::Net::HTTPRequestHandler {
        public:
            RequestHandler(const std::string &Pattern, const std::string &WebhookToken, nlohmann::json &JsonConfigData);
            virtual void handleRequest(
                Poco::Net::HTTPServerRequest &req, 
                Poco::Net::HTTPServerResponse &resp
            );
            void handleTypeWebhook(
                const std::string &typeWebhook, 
                Response& body,
                Poco::Net::HTTPServerResponse &resp
            );

        private:
            std::string Pattern;
            std::string WebhookToken;
            nlohmann::json JsonConfigData;
    };

    // Returns a new object of type RequestHandler
    class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
        public:
            RequestHandlerFactory(const std::string &Pattern, const std::string &WebhookToken, nlohmann::json &JsonConfigData);
            virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest &) {
                return new RequestHandler(Pattern, WebhookToken, JsonConfigData);
            }
        private:
            std::string Pattern;
            std::string WebhookToken;
            nlohmann::json JsonConfigData;
    };
}

#endif