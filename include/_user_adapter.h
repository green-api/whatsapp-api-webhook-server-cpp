#ifndef USER_ADAPTER_H
#define USER_ADAPTER_H
#include <iostream>
#include "response.h"
#include "logger.h"

class UserAdapter {
    public:
        // Incoming message. Parameters: 
        // [typeWebhook: string, instanceData: object, timestamp: integer, idMessage: string, senderData: object, messageData: object]
        // View documentation here:
        // https://green-api.com/en/docs/api/receiving/notifications-format/incoming-message/Webhook-IncomingMessageReceived/
        #define ON_INCOMING_MESSAGE_RECEIVED_EXISTS
        static void onIncomingMessageReceived(greenapi::Response& body);

        // Outcoming message (from phone). Parameters:
        // [typeWebhook: string, instanceData: object, timestamp: integer, idMessage: string, senderData: object, messageData: object]
        // View documentation here:
        // https://green-api.com/en/docs/api/receiving/notifications-format/outgoing-message/OutgoingMessage/
        #define ON_OUTGOING_MESSAGE_RECEIVED_EXISTS
        static void onOutgoingMessageReceived(greenapi::Response& body);

        // Outcoming message (via API). Parameters:
        // [typeWebhook: string, instanceData: object, timestamp: integer, idMessage: string, senderData: object, messageData: object]
        // View documentation here:
        // https://green-api.com/en/docs/api/receiving/notifications-format/outgoing-message/OutgoingApiMessage/
        #define ON_OUTGOING_API_MESSAGE_RECEIVED_EXISTS
        static void onOutgoingAPIMessageReceived(greenapi::Response& body);
        
        // Status of a previously sent message: sent, delivered, read, etc. Parameters:
        // [typeWebhook: string, chatId: string, instanceData: object, timestamp: integer, idMessage: string, status: string, description: string, sendByApi: boolean]
        // View documentation here:
        // https://green-api.com/en/docs/api/receiving/notifications-format/outgoing-message/OutgoingMessageStatus/
        #define ON_OUTGOING_MESSAGE_STATUS_EXISTS
        static void onOutgoingMessageStatus(greenapi::Response& body);

        // Instance authorization status data. Parameters:
        // [typeWebhook: string, instanceData: object, timestamp: integer, stateInstance: string]
        // View documentation here:
        // https://green-api.com/en/docs/api/receiving/notifications-format/StateInstanceChanged/
        #define ON_STATE_INSTANCE_CHANGED_EXISTS
        static void onStateInstanceChanged(greenapi::Response& body);

        // Information about the contact's avatar based on the requested phone number. Parameters:
        // [typeWebhook: string, instanceData: object, timestamp: integer, deviceData: object]
        // View documentation here:
        // https://green-api.com/en/docs/api/receiving/notifications-format/AvatarInfo/
        #define ON_DEVICE_INFO_EXISTS
        static void onDeviceInfo(greenapi::Response& body);


        // Appears when there is an incoming call and contains information about the initiator and recipient of the call. Parameters:
        // [from: string, typeWebhook: string, instanceData: object, status: string, timestamp: integer, idMessage: string]
        // View documentation here:
        // https://green-api.com/en/docs/api/receiving/notifications-format/IncomingCall/
        #define ON_INCOMING_CALL_EXISTS
        static void onIncomingCall(greenapi::Response& body);

        // Socket state data (whether instance is ready to send/receive messages). Parameters:
        // [typeWebhook: string, instanceData: object, timestamp: integer, statusInstance: string]
        // View documentation here:
        // https://green-api.com/en/docs/api/receiving/notifications-format/StatusInstanceChanged/
        #define ON_STATUS_INSTANCE_CHANGED_EXISTS
        static void onStatusInstanceChanged(greenapi::Response& body);

        // Requests with unknown webhook type will be handled here.
        #define ON_UNKNOWN_TYPEWEBHOOK_EXISTS
        static void onUnknownTypeWebhook(greenapi::Response& body);
    private:
        UserAdapter() {}
};

#endif