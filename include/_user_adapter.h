#ifndef USER_ADAPTER_H
#define USER_ADAPTER_H
#include <iostream>
#include "response.h"
#include "logger.h"

// If your request hang up, please check your function is getting only existing fields in JSON.
// Example:
//      const auto val = body.bodyJson["not-existing-value"];
// will cause debug assert (Debug) or abort function execution (Release)
// Check for contains() if value may not be exists, example:
//      const auto val = body.bodyJson.contains("not-existing-value") ? body.bodyJson["not-existing-value"] : nullptr;

class UserAdapter {
    public:
        // Incoming message. Parameters: 
        // [typeWebhook: string, instanceData: object, timestamp: integer, idMessage: string, senderData: object, messageData: object]
        // View documentation here:
        // https://green-api.com/en/docs/api/receiving/notifications-format/incoming-message/Webhook-IncomingMessageReceived/
        // Returns: [true], if error; [false], if no error
        #define ON_INCOMING_MESSAGE_RECEIVED_EXISTS
        static bool onIncomingMessageReceived(greenapi::Response& body);

        // Outcoming message (from phone). Parameters:
        // [typeWebhook: string, instanceData: object, timestamp: integer, idMessage: string, senderData: object, messageData: object]
        // View documentation here:
        // https://green-api.com/en/docs/api/receiving/notifications-format/outgoing-message/OutgoingMessage/
        // Returns: [true], if error; [false], if no error
        #define ON_OUTGOING_MESSAGE_RECEIVED_EXISTS
        static bool onOutgoingMessageReceived(greenapi::Response& body);

        // Outcoming message (via API). Parameters:
        // [typeWebhook: string, instanceData: object, timestamp: integer, idMessage: string, senderData: object, messageData: object]
        // View documentation here:
        // https://green-api.com/en/docs/api/receiving/notifications-format/outgoing-message/OutgoingApiMessage/
        // Returns: [true], if error; [false], if no error
        #define ON_OUTGOING_API_MESSAGE_RECEIVED_EXISTS
        static bool onOutgoingAPIMessageReceived(greenapi::Response& body);
        
        // Status of a previously sent message: sent, delivered, read, etc. Parameters:
        // [typeWebhook: string, chatId: string, instanceData: object, timestamp: integer, idMessage: string, status: string, description: string, sendByApi: boolean]
        // View documentation here:
        // https://green-api.com/en/docs/api/receiving/notifications-format/outgoing-message/OutgoingMessageStatus/
        // Returns: [true], if error; [false], if no error
        #define ON_OUTGOING_MESSAGE_STATUS_EXISTS
        static bool onOutgoingMessageStatus(greenapi::Response& body);

        // Instance authorization status data. Parameters:
        // [typeWebhook: string, instanceData: object, timestamp: integer, stateInstance: string]
        // View documentation here:
        // https://green-api.com/en/docs/api/receiving/notifications-format/StateInstanceChanged/
        // Returns: [true], if error; [false], if no error
        #define ON_STATE_INSTANCE_CHANGED_EXISTS
        static bool onStateInstanceChanged(greenapi::Response& body);

        // Information about the contact's avatar based on the requested phone number. Parameters:
        // [typeWebhook: string, instanceData: object, timestamp: integer, deviceData: object]
        // View documentation here:
        // https://green-api.com/en/docs/api/receiving/notifications-format/AvatarInfo/
        // Returns: [true], if error; [false], if no error
        #define ON_DEVICE_INFO_EXISTS
        static bool onDeviceInfo(greenapi::Response& body);

        // Appears when there is an incoming call and contains information about the initiator and recipient of the call. Parameters:
        // [from: string, typeWebhook: string, instanceData: object, status: string, timestamp: integer, idMessage: string]
        // View documentation here:
        // https://green-api.com/en/docs/api/receiving/notifications-format/IncomingCall/
        // Returns: [true], if error; [false], if no error
        #define ON_INCOMING_CALL_EXISTS
        static bool onIncomingCall(greenapi::Response& body);

        // Socket state data (whether instance is ready to send/receive messages). Parameters:
        // [typeWebhook: string, instanceData: object, timestamp: integer, statusInstance: string]
        // View documentation here:
        // https://green-api.com/en/docs/api/receiving/notifications-format/StatusInstanceChanged/
        // Returns: [true], if error; [false], if no error
        #define ON_STATUS_INSTANCE_CHANGED_EXISTS
        static bool onStatusInstanceChanged(greenapi::Response& body);

        // Incoming webhook quotaExceeded contains data about exceeding chat limitations on the Developer plan. 
        // Parameters: [typeWebhook: string, instanceData: object, timestamp: integer, quotaData: object]
        // View documentation here:
        // https://green-api.com/en/docs/api/receiving/notifications-format/StatusInstanceChanged/
        // Returns: [true], if error; [false], if no error
        #define ON_QUOTA_EXCEEDED_EXISTS
        static bool onQuotaExceeded(greenapi::Response& body);

        // Handle error here: output error description and make other error handling
        // Returns: [true], if error; [false], if no error
        #define ON_ERROR_VALIDATION_EXISTS
        static bool onErrorValidation(greenapi::Response& body);

        // Requests with unknown webhook type will be handled here.
        // Returns: [true], if error; [false], if no error
        #define ON_UNKNOWN_TYPEWEBHOOK_EXISTS
        static bool onUnknownTypeWebhook(greenapi::Response& body);
    private:
        UserAdapter() {}
};

#endif