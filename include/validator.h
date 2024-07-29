#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <iostream>

#include <nlohmann/json.hpp>
#include <nlohmann/json-schema.hpp>
#include "response.h"
#include "logger.h"
#include <sstream>
#include <regex>

namespace greenapi {
    class Validator {
        public:
            // Main handler, which called outside to start validate proccess
            static void Validate(Response &r, nlohmann::json &schemas);
        
        protected:
            // If called, error is writing to r.bodyStr, logging the error
            static inline bool InteruptValidate(Response &r, const std::string &message);
            
            // This function called by Validate, returtns [false] if Response.bodyJson valid, [true] if Response.bodyJson has errors.
            static bool ProccessValidate(Response &r, const std::string &schemaName, const nlohmann::json &schemas);
            
            Validator() {}
    };
}

#endif