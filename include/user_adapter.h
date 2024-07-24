#ifndef USER_ADAPTER_H
#define USER_ADAPTER_H
#include <iostream>
#include "response.h"
#include "logger.h"

class UserAdapter {
    public:
        //  Статус устройства
        static void onDeviceInfo(greenapi::Response& body);

        // Входящий звонок
        static void onIncomingCall(greenapi::Response& body);

        //  Входящее сообщение
        static void onIncomingMessageReceived(greenapi::Response& body);

        //  Отправленное сообщение через API
        static void onOutgoingAPIMessageReceived(greenapi::Response& body);

        //  Статус отправленного сообщения
        static void onOutgoingMessageStatus(greenapi::Response& body);

        //  Отправленное с телефона сообщение
        static void onOutgoingMessageReceived(greenapi::Response& body);

        //  Статус инстанса
        static void onStateInstanceChanged(greenapi::Response& body);

        //  Статус сокета инстанса
        static void onStatusInstanceChanged(greenapi::Response& body);

        //  Неизвестный вебхук
        static void onUnknownTypeWebhook(greenapi::Response& body);
    private:
        UserAdapter() {}
};

#endif