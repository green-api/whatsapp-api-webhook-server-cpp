#include "validator.h"

using namespace greenapi;

//  Returns:
//  -- Value, if key exists
//  -- nullptr, else
const auto Validator::Get(const Response &key, const std::string &value) {
    return key.bodyJson.contains(value) ? key.bodyJson[value] : nullptr;
}

//  Returns:
//  -- Value, if key exists
//  -- nullptr, else
const auto Validator::Get(const nlohmann::json &key, const std::string &value) {
    return key.contains(value) ? key[value] : nullptr;
}

//  If called, error is writing to r.bodyStr, logging the error
bool Validator::InteruptValidate(Response &r, const std::string& message) {
    r.bodyStr += message;
    r.bodyStr += "\n";
    Logger::Log(message, "error");
    return false;
}

//  Main handler, which called outside to start validate proccess. Based on TypeWebhook, it choose handler to validate
void Validator::Validate(Response &r, nlohmann::json &schemas) {
    bool exists {false};
    bool check {false};
    bool err {false};
    const auto TypeWebhook = Validator::Get(r, "typeWebhook");
    check = (TypeWebhook.type() == nlohmann::json::value_t::string); exists = (TypeWebhook != nullptr);
    if (!exists) {
        InteruptValidate(r, "No typeWebhook given, aborting validation");
        r.error = true; 
        return;
    }
    else if (!check) {
        InteruptValidate(r, "Wrong typeWebhook type given, aborting validation");
        r.error = true; 
        return;
    } else if (TypeWebhook == "") {
        InteruptValidate(r, "No typeWebhook value given, aborting validation");
        r.error = true; 
        return;
    }

    const auto typeWebhookAvailable = (schemas.contains("properties") && schemas["properties"].contains(TypeWebhook)) ? schemas["properties"][TypeWebhook] : nullptr;
    if (typeWebhookAvailable == nullptr) {
        // unknown typeWebhook
        r.error = false;
    }
    else if (typeWebhookAvailable.type() == nlohmann::json::value_t::string){
        r.error = Validator::ProccessValidate(r, "incomingMessageReceived", schemas);
        if (r.error) {
            std::ostringstream buffer;
            buffer << TypeWebhook;
            Logger::Log("Failed check: " + buffer.str(), "error");
        }
    } else {
        InteruptValidate(r, "TypeWebhook from schemas json is not a string, aborting validation");
    }    
}

//  Compare given json object with given schema, returns error, returns [true] if error, [false] if no error
bool Validator::ProccessValidate(Response &r, const std::string &schemaName, const nlohmann::json &schemas) {
    // don't use jsonData == nullptr, it will cause runtime error
    const auto jsonData_check = r.bodyJson;
    if (jsonData_check == nullptr) {
        Logger::Log("JSON in validation is nullptr, aborting validation.", "error");
        return true;
    }
    // don't use schemas == nullptr, it will cause runtime error
    const auto schemas_check = schemas; 
    if (schemas_check == nullptr) {
        Validator::InteruptValidate(r, "Unable to open " + schemaName + " in JSON verification files, aborting validation.");
        return true;
    }

    if (!schemas.contains("properties")) {
        Logger::Log("JSON file has no properties, this may cause unexpected errors.", "info");
    }
    
    // Request's content is "{ ... }". We are making it "schemaName = { ... }" for passing to JSON validator
    // Example: incomingMessageReceived webhook's data wiil be transofrmed from "{ ... }" into "incomingMessageReceived = { ... }
    nlohmann::json j;
    j[schemaName] = r.bodyJson;

    nlohmann::json_schema::json_validator validator;
    // to do: replace try-catch with custom error handling without throws
    try {
        validator.set_root_schema(schemas);
        const auto default_patch = validator.validate(j);
    } catch (const std::exception &e) {
        std::string error = e.what();
        Validator::InteruptValidate(r, e.what());
        return true;
    }
    return false;
}