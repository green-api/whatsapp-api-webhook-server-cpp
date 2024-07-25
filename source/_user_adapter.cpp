#include "user_adapter.h"

// Incoming message. Parameters: 
// [typeWebhook: string, instanceData: object, timestamp: integer, idMessage: string, senderData: object, messageData: object]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/incoming-message/Webhook-IncomingMessageReceived/     
void UserAdapter::onIncomingMessageReceived(greenapi::Response& body) {
    const auto typeWebhook = body.bodyJson["typeWebhook"];
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "info");
        return;
    }
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");
    // Write your handler here:

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
    // Write your handler here:

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
    // Write your handler here:

}

// Status of a previously sent message: sent, delivered, read, etc. Parameters:
// [typeWebhook: string, chatId: string, instanceData: object, timestamp: integer, idMessage: string, status: string, description: string, sendByApi: boolean]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/outgoing-message/OutgoingMessageStatus/
void UserAdapter::onOutgoingMessageStatus(greenapi::Response& body) {
    const auto typeWebhook = body.bodyJson["typeWebhook"];
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "info");
        return;
    }
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");
    // Write your handler here:

}

// Instance authorization status data. Parameters:
// [typeWebhook: string, instanceData: object, timestamp: integer, stateInstance: string]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/StateInstanceChanged/
void UserAdapter::onStateInstanceChanged(greenapi::Response& body) {
    const auto typeWebhook = body.bodyJson["typeWebhook"];
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "info");
        return;
    }
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");
    // Write your handler here:

}

// Information about the contact's avatar based on the requested phone number. Parameters:
// [typeWebhook: string, instanceData: object, timestamp: integer, deviceData: object]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/AvatarInfo/
void UserAdapter::onDeviceInfo(greenapi::Response& body) {
    const auto typeWebhook = body.bodyJson["typeWebhook"];
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "info");
        return;
    }
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");
    // Write your handler here:

}

// Appears when there is an incoming call and contains information about the initiator and recipient of the call. Parameters:
// [from: string, typeWebhook: string, instanceData: object, status: string, timestamp: integer, idMessage: string]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/IncomingCall/
void UserAdapter::onIncomingCall(greenapi::Response& body) {
    const auto typeWebhook = body.bodyJson["typeWebhook"];
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "info");
        return;
    }
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");
    // Write your handler here:

}

// Socket state data (whether instance is ready to send/receive messages). Parameters:
// [typeWebhook: string, instanceData: object, timestamp: integer, statusInstance: string]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/StatusInstanceChanged/
void UserAdapter::onStatusInstanceChanged(greenapi::Response& body) {
    const auto typeWebhook = body.bodyJson["typeWebhook"];
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "info");
        return;
    }
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");
    // Write your handler here:

}

// Requests with unknown webhook type will be handled here.
void UserAdapter::onUnknownTypeWebhook(greenapi::Response& body) {
    const auto typeWebhook = body.bodyJson["typeWebhook"];
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "info");
        return;
    }
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");
    // Write your handler here:
    
}