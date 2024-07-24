#ifndef RESPONSE_H
#define RESPONSE_H
#include <nlohmann/json.hpp>

namespace greenapi {
    struct ResponseHeaders {
        std::string contentType = "application/json";
        std::string currentDate = "";
        std::string uploadDate = "";
        std::string expirationDate = "";
    };

    struct Response {
        bool error = true;
        unsigned int status_code = 0;
        double total_time = 0.0;
        ResponseHeaders headers;
        std::string typeWebhook = "";
        std::string bodyStr = "";
        nlohmann::json bodyJson = "";
    };
}
#endif