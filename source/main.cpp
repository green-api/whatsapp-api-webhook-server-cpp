#include <iostream>
#include "webhook.h"
#include "config.h"

int main(int argc, char** argv)
{
    std::map<std::string, std::string> config = greenapi::Config::LoadServerConfig("config.json");
    greenapi::Webhook webhook{config["Address"], config["Pattern"], config["WebhookToken"]};
    webhook.StartServer();
    return 0;
}