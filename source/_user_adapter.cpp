#include "user_adapter.h"

//  Статус устройства
void UserAdapter::onDeviceInfo(greenapi::Response& body) {
    greenapi::Logger::Log("onDeviceInfo called", "info");
}
// Входящий звонок
void UserAdapter::onIncomingCall(greenapi::Response& body) {
    greenapi::Logger::Log("onIncomingCall called", "info");
}
//  Входящее сообщение
void UserAdapter::onIncomingMessageReceived(greenapi::Response& body) {
    greenapi::Logger::Log("onIncomingMessageReceived called", "info");
}
//  Отправленное сообщение через API
void UserAdapter::onOutgoingAPIMessageReceived(greenapi::Response& body) {
    greenapi::Logger::Log("onOutgoingAPIMessageReceived called", "info");
}
//  Статус отправленного сообщения
void UserAdapter::onOutgoingMessageStatus(greenapi::Response& body) {
    greenapi::Logger::Log("onOutgoingMessageStatus called", "info");
}
//  Отправленное с телефона сообщение
void UserAdapter::onOutgoingMessageReceived(greenapi::Response& body) {
    greenapi::Logger::Log("onOutgoingMessageReceived called", "info");
}
//  Статус инстанса
void UserAdapter::onStateInstanceChanged(greenapi::Response& body) {
    greenapi::Logger::Log("onStateInstanceChanged called", "info");
}
//  Статус сокета инстанса
void UserAdapter::onStatusInstanceChanged(greenapi::Response& body) {
    greenapi::Logger::Log("onStatusInstanceChanged called", "info");
}
//  Неизвестный вебхук
void UserAdapter::onUnknownTypeWebhook(greenapi::Response& body) {
    greenapi::Logger::Log("onUnknownTypeWebhook called", "info");
}