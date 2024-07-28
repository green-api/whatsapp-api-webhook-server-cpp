#include "user_adapter.h"

// Incoming message. Parameters: 
// [typeWebhook: string, instanceData: object, timestamp: integer, idMessage: string, senderData: object, messageData: object]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/incoming-message/Webhook-IncomingMessageReceived/     
void UserAdapter::onIncomingMessageReceived(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If a webhook is invalid, you could log a reason.
    // Server respondes to client before UserAdapter function. Response based on validation result.
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "error");
        return;
    }

    // You can get request body using bodyStr:
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");

    // Write your function here:
}

// Outcoming message (from phone). Parameters:
// [typeWebhook: string, instanceData: object, timestamp: integer, idMessage: string, senderData: object, messageData: object]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/outgoing-message/OutgoingMessage/
void UserAdapter::onOutgoingMessageReceived(greenapi::Response& body) {
    const auto typeWebhook = body.bodyJson["typeWebhook"];
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "info");
        return;
    }
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");
    
    // Write your function here:
}

// Outcoming message (via API). Parameters:
// [typeWebhook: string, instanceData: object, timestamp: integer, idMessage: string, senderData: object, messageData: object]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/outgoing-message/OutgoingApiMessage/
void UserAdapter::onOutgoingAPIMessageReceived(greenapi::Response& body) {
    const auto typeWebhook = body.bodyJson["typeWebhook"];
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "info");
        return;
    }
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");
    
    // Write your function here:
}

// Status of a previously sent message: sent, delivered, read, etc. Parameters:
// [typeWebhook: string, chatId: string, instanceData: object, timestamp: integer, idMessage: string, status: string, description: string, sendByApi: boolean]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/outgoing-message/OutgoingMessageStatus/
void UserAdapter::onOutgoingMessageStatus(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If a webhook is invalid, you could log a reason.
    // Server respondes to client before UserAdapter function. Response based on validation result.
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "error");
        return;
    }

    // You can get request body using bodyStr:
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");

    // Write your function here:
}

// Instance authorization status data. Parameters:
// [typeWebhook: string, instanceData: object, timestamp: integer, stateInstance: string]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/StateInstanceChanged/
void UserAdapter::onStateInstanceChanged(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If a webhook is invalid, you could log a reason.
    // Server respondes to client before UserAdapter function. Response based on validation result.
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "error");
        return;
    }

    // Write your function here:
}

// Information about the contact's avatar based on the requested phone number. Parameters:
// [typeWebhook: string, instanceData: object, timestamp: integer, deviceData: object]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/AvatarInfo/
void UserAdapter::onDeviceInfo(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If a webhook is invalid, you could log a reason.
    // Server respondes to client before UserAdapter function. Response based on validation result.
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "error");
        return;
    }

    // You can get request body using bodyStr:
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");

    // Write your function here:
}

// Appears when there is an incoming call and contains information about the initiator and recipient of the call. Parameters:
// [from: string, typeWebhook: string, instanceData: object, status: string, timestamp: integer, idMessage: string]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/IncomingCall/
void UserAdapter::onIncomingCall(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If a webhook is invalid, you could log a reason.
    // Server respondes to client before UserAdapter function. Response based on validation result.
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "error");
        return;
    }

    // You can get request body using bodyStr:
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");

    // Write your function here:
}

// Socket state data (whether instance is ready to send/receive messages). Parameters:
// [typeWebhook: string, instanceData: object, timestamp: integer, statusInstance: string]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/StatusInstanceChanged/
void UserAdapter::onStatusInstanceChanged(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If a webhook is invalid, you could log a reason.
    // Server respondes to client before UserAdapter function. Response based on validation result.
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "error");
        return;
    }

    // You can get request body using bodyStr:
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");

    // Write your function here:
}


// Incoming webhook quotaExceeded contains data about exceeding chat limitations on the Developer plan. 
// Parameters: [typeWebhook: string, instanceData: object, timestamp: integer, quotaData: object]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/StatusInstanceChanged/        
void UserAdapter::onQuotaExceeded(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If a webhook is invalid, you could log a reason.
    // Server respondes to client before UserAdapter function. Response based on validation result.
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "error");
        return;
    }

    // You can get request body using bodyStr:
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");

    // Write your function here:
}

// Requests with unknown webhook type will be handled here.
void UserAdapter::onUnknownTypeWebhook(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // No error given to request, if it's unknown
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");
    
    // Write your function here:
}