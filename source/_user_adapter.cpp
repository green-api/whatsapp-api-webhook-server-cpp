#include "user_adapter.h"

// If your request hang up, please check your function is getting only existing fields in JSON.
// Example:
//      const auto val = body.bodyJson["not-existing-value"];
// will cause debug assert (Debug) or abort function execution (Release)
// Check for contains() if value may not be exists, example:
//      const auto val = body.bodyJson.contains("not-existing-value") ? body.bodyJson["not-existing-value"] : nullptr;


// Incoming message. Parameters: 
// [typeWebhook: string, instanceData: object, timestamp: integer, idMessage: string, senderData: object, messageData: object]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/incoming-message/Webhook-IncomingMessageReceived/
// Returns: [true], if error; [false], if no error     
bool UserAdapter::onIncomingMessageReceived(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If you encountered errors while hanlding, you should return true.
    // It will change response status to 400 Bad Request with immediate return of the HTTP request result
    // 
    // if (<error>) {
    //    return true;
    //}

    // You can get raw request body using Response.bodyStr (string):
    std::cout << "Received webhook: " << nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr << "\n";


    // Write your handler here:
    

    // Return false if no error, after this 200 OK response will be returned
    return false;
}

// Outcoming message (from phone). Parameters:
// [typeWebhook: string, instanceData: object, timestamp: integer, idMessage: string, senderData: object, messageData: object]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/outgoing-message/OutgoingMessage/
// Returns: [true], if error; [false], if no error
bool UserAdapter::onOutgoingMessageReceived(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If you encountered errors while hanlding, you should return true.
    // It will change response status to 400 Bad Request with immediate return of the HTTP request result
    // 
    // if (<error>) {
    //    return true;
    //}

    // You can get raw request body using Response.bodyStr (string):
    std::cout << "Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr << "\n";
    std::cout << "Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr << "\n";

    // Write your handler here:
    

    // Return false if no error, after this 200 OK response will be returned
    return false;
}

// Outcoming message (via API). Parameters:
// [typeWebhook: string, instanceData: object, timestamp: integer, idMessage: string, senderData: object, messageData: object]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/outgoing-message/OutgoingApiMessage/
// Returns: [true], if error; [false], if no error
bool UserAdapter::onOutgoingAPIMessageReceived(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If you encountered errors while hanlding, you should return true.
    // It will change response status to 400 Bad Request with immediate return of the HTTP request result
    // 
    // if (<error>) {
    //    return true;
    //}

    // You can get raw request body using Response.bodyStr (string):
    std::cout << "Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr << "\n";

    // Write your handler here:
    

    // Return false if no error, after this 200 OK response will be returned
    return false;
}

// Status of a previously sent message: sent, delivered, read, etc. Parameters:
// [typeWebhook: string, chatId: string, instanceData: object, timestamp: integer, idMessage: string, status: string, description: string, sendByApi: boolean]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/outgoing-message/OutgoingMessageStatus/
// Returns: [true], if error; [false], if no error
bool UserAdapter::onOutgoingMessageStatus(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If you encountered errors while hanlding, you should return true.
    // It will change response status to 400 Bad Request with immediate return of the HTTP request result
    // 
    // if (<error>) {
    //    return true;
    //}

    // You can get raw request body using Response.bodyStr (string):
    std::cout << "Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr << "\n";

    // Write your handler here:
    

    // Return false if no error, after this 200 OK response will be returned
    return false;
}

// Instance authorization status data. Parameters:
// [typeWebhook: string, instanceData: object, timestamp: integer, stateInstance: string]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/StateInstanceChanged/
// Returns: [true], if error; [false], if no error
bool UserAdapter::onStateInstanceChanged(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If you encountered errors while hanlding, you should return true.
    // It will change response status to 400 Bad Request with immediate return of the HTTP request result
    // 
    // if (<error>) {
    //    return true;
    //}

    // You can get raw request body using Response.bodyStr (string):
    std::cout << "Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr << "\n";

    // Write your handler here:
    

    // Return false if no error, after this 200 OK response will be returned
    return false;
}

// Information about the contact's avatar based on the requested phone number. Parameters:
// [typeWebhook: string, instanceData: object, timestamp: integer, deviceData: object]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/AvatarInfo/
// Returns: [true], if error; [false], if no error
bool UserAdapter::onDeviceInfo(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If you encountered errors while hanlding, you should return true.
    // It will change response status to 400 Bad Request with immediate return of the HTTP request result
    // 
    // if (<error>) {
    //    return true;
    //}

    // You can get raw request body using Response.bodyStr (string):
    std::cout << "Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr << "\n";

    // Write your handler here:
    

    // Return false if no error, after this 200 OK response will be returned
    return false;
}

// Appears when there is an incoming call and contains information about the initiator and recipient of the call. Parameters:
// [from: string, typeWebhook: string, instanceData: object, status: string, timestamp: integer, idMessage: string]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/IncomingCall/
// Returns: [true], if error; [false], if no error
bool UserAdapter::onIncomingCall(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If you encountered errors while hanlding, you should return true.
    // It will change response status to 400 Bad Request with immediate return of the HTTP request result
    // 
    // if (<error>) {
    //    return true;
    //}

    // You can get raw request body using Response.bodyStr (string):
    std::cout << "Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr << "\n";

    // Write your handler here:
    

    // Return false if no error, after this 200 OK response will be returned
    return false;
}

// Socket state data (whether instance is ready to send/receive messages). Parameters:
// [typeWebhook: string, instanceData: object, timestamp: integer, statusInstance: string]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/StatusInstanceChanged/
// Returns: [true], if error; [false], if no error
bool UserAdapter::onStatusInstanceChanged(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If you encountered errors while hanlding, you should return true.
    // It will change response status to 400 Bad Request with immediate return of the HTTP request result
    // 
    // if (<error>) {
    //    return true;
    //}

    // You can get raw request body using Response.bodyStr (string):
    std::cout << "Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr << "\n";

    // Write your handler here:
    

    // Return false if no error, after this 200 OK response will be returned
    return false;
}


// Incoming webhook quotaExceeded contains data about exceeding chat limitations on the Developer plan. 
// Parameters: [typeWebhook: string, instanceData: object, timestamp: integer, quotaData: object]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/StatusInstanceChanged/        
// Returns: [true], if error; [false], if no error
bool UserAdapter::onQuotaExceeded(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If you encountered errors while hanlding, you should return true.
    // It will change response status to 400 Bad Request with immediate return of the HTTP request result
    // 
    // if (<error>) {
    //    return true;
    //}

    // You can get raw request body using Response.bodyStr (string):
    std::cout << "Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr << "\n";

    // Write your handler here:


    // Return false if no error, after this 200 OK response will be returned
    return false;
}

// Handle error here: output error description and make other error handling
// Returns: [true], if error; [false], if no error
bool UserAdapter::onErrorValidation(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto TypeWebhook = body.bodyJson.contains("typeWebhook") ? body.bodyJson["typeWebhook"] : nullptr;
    
    if (TypeWebhook != nullptr && TypeWebhook.type() == nlohmann::json::value_t::string) {
        // Replace quotes if any
        const std::string typeWebhook = std::regex_replace(nlohmann::to_string(TypeWebhook), std::regex("\\\""), "");
        // Error description written in Response.bodyStr
        greenapi::Logger::Log("Received invalid webhook: " + typeWebhook + std::string(". Error: ") + body.bodyStr + std::string(". Body: ") + nlohmann::to_string(body.bodyJson), "error");
        // Return true will change response status to 400 Bad Request with immediate return of the HTTP request result
        return true;
    }
    greenapi::Logger::Log("Received invalid webhook with typeWebhook error. Error: " + body.bodyStr + std::string(". Body: ") + nlohmann::to_string(body.bodyJson), "error");
    // Return true if error, after this 400 Bad Request response will be returned

    // Write your handler here:

    return true;
}

// Requests with unknown webhook type will be handled here.
// Returns: [true], if error; [false], if no error
bool UserAdapter::onUnknownTypeWebhook(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If you encountered errors while hanlding, you should return true.
    // It will change response status to 400 Bad Request with immediate return of the HTTP request result
    // 
    // if (<error>) {
    //    return true;
    //}

    // No error given to request, if it's unknown
    std::cout << "Received unknown webhook" + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr << "\n";
    
    // Write your handler here:

    // Return false if no error, after this 200 OK response will be returned
    return false;
}