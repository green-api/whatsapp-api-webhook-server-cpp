#ifndef RESPONSE_H
#define RESPONSE_H
#include <nlohmann/json.hpp>

namespace greenapi {
    struct Response {
        bool error = true;
        std::string typeWebhook = "";
        std::string bodyStr = "";
        nlohmann::json bodyJson = "";
    };
}
#endif