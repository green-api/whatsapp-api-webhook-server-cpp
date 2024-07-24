#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <iostream>

#include <nlohmann/json.hpp>
#include <nlohmann/json-schema.hpp>
#include "response.h"
#include "logger.h"
#include <sstream>


namespace greenapi {
    class Validator {
        public:
            // Main handler, which called outside to start validate proccess. Based on TypeWebhook, it choose handler to validate
            static void Validate(Response &r, nlohmann::json &schemas);
        
        protected:
            // If called, error is writing to r.bodyStr, logging the error
            static inline bool InteruptValidate(Response &r, const std::string &message);
            
            /*  Returns: value, if key exists; else: nullptr    */
            static const auto Get(const Response &key, const std::string &value);
            static const auto Get(const nlohmann::json &key, const std::string &value);

            static bool ProccessValidate(Response &r, const std::string &schemaName, const nlohmann::json &schemas);
            Validator() {}
    };
}

#endif