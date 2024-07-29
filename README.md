# whatsapp-api-webhook-server-cpp

![](https://img.shields.io/badge/license-CC%20BY--ND%204.0-green)

## Support links

[![Support](https://img.shields.io/badge/support@green--api.com-D14836?style=for-the-badge&logo=gmail&logoColor=white)](mailto:support@green-api.com)
[![Support](https://img.shields.io/badge/Telegram-2CA5E0?style=for-the-badge&logo=telegram&logoColor=white)](https://t.me/greenapi_support_ru_bot)
[![Support](https://img.shields.io/badge/WhatsApp-25D366?style=for-the-badge&logo=whatsapp&logoColor=white)](https://wa.me/447458038017)

## Guides & News

[![Guides](https://img.shields.io/badge/YouTube-%23FF0000.svg?style=for-the-badge&logo=YouTube&logoColor=white)](https://www.youtube.com/@greenapi-en)
[![News](https://img.shields.io/badge/Telegram-2CA5E0?style=for-the-badge&logo=telegram&logoColor=white)](https://t.me/green_api)
[![News](https://img.shields.io/badge/WhatsApp-25D366?style=for-the-badge&logo=whatsapp&logoColor=white)](https://whatsapp.com/channel/0029VaLj6J4LNSa2B5Jx6s3h)

- [Документация на русском языке](https://github.com/green-api/whatsapp-api-webhook-server-cpp/blob/master/README.ru.md).


`whatsapp-api-webhook-server-cpp` — webhook server for integration with WhatsApp messenger using the API service [green-api.com](https://green-api.com/). 
You should get a registration token and an account ID in your [personal cabinet](https://console.green-api.com/). 
There is a free developer account tariff.

- [whatsapp-api-webhook-server-cpp](#whatsapp-api-webhook-server-cpp)
  - [Support links](#support-links)
  - [Guides \& News](#guides--news)
  - [API](#api)
  - [Receiving Webhook Token](#receiving-webhook-token)
  - [Building the app](#building-the-app)
    - [Windows](#windows)
    - [Linux](#linux)
    - [Docker](#docker)
  - [Running the app](#running-the-app)
  - [User Adapter](#user-adapter)
  - [Examples](#examples)
  - [JSON Validation](#json-validation)
  - [Webhooks documentation](#webhooks-documentation)
  - [Library dependencies](#library-dependencies)
  - [Licence](#licence)

## API

The documentation for webhooks could be find [here](https://green-api.com/en/docs/api/#receiving). The app is a handler for it, so the documentation at the link above also applies.

## Receiving Webhook Token

To receive a Webhook Token and be able to send requests into this server, the WhatsApp account in the phone app must be authorized. To authorize the account, go to your [cabinet](https://console.green-api.com/) and scan the QR code using the WhatsApp app.

## Building the app

This app uses C++ 17, CMake 3.5, supports Linux (GCC) and Windows (Visual Studio 2019/2022) compilers.

Before building you should create these files (or copy existing ones if you have):
- ```include/user_adapter.h```
- ```source/user_adapter.cpp```

If you don't have required fiels, create them by renaming with removing underscore from ```include/_user_adapter.h``` and ```source/_user_adapter.h``` files. 

**This project will not build if you won't do it**.

### Windows

To build the app you need:

- [git](https://gitforwindows.org/) - a cross-platform utility used on this project for downloading libraries.
- [CMake](https://cmake.org/download/) - a cross-platform utility for automatically building software from source code.
- Compiler [Microsoft Visual C++ (MSVC)](https://visualstudio.microsoft.com/ru/downloads/) for C++ applications.

```cmake``` and ```git``` must be accessible by PATH.

Building is done by using the ```build.bat``` scenario (or ```.\build.bat``` if Powershell used):

```bash
git clone --branch=master --depth=1 https://github.com/green-api/whatsapp-api-webhook-server-cpp
cd whatsapp-api-webhook-server-cpp
build.bat
```

Application is built at Release config by default.

After successful build you can build it using ```build.bat``` or 

```bash
cmake --build build --config=Release
```

### Linux

To build the app you need git, g++, cmake:

```bash
sudo apt-get install git g++ cmake
```

```cmake``` and ```git``` must be accessible by bash.

Building is done by using the ```build.sh``` scenario:

```bash
git clone --branch=master --depth=1 https://github.com/green-api/whatsapp-api-webhook-server-cpp
cd whatsapp-api-webhook-server-cpp
.\build.sh
```

Application is built at Release config by default.

After successful build you can build it using ```.\build.sh``` or 

```bash
cmake --build build --config=Release
```

### Docker

To run this project with Docker, you need Docker and Docker Compose.

Clone the repository:

```bash
git clone --branch=master --depth=1 https://github.com/green-api/whatsapp-api-webhook-server-cpp
cd whatsapp-api-webhook-server-cpp
```

Before building you should create these files (or copy existing ones if you have):
- ```include/user_adapter.h```
- ```source/user_adapter.cpp```

If you don't have required fiels, create them by renaming with removing underscore from ```include/_user_adapter.h``` and ```source/_user_adapter.h``` files. 

By default, ```port 5000``` is exposed. If you would like to change it, you need:
- Change ```Address``` field in ```config.json``` to your desired port;
- Change ```ports``` field in ```compose.yaml``` to your desired port;

Run Docker image with Docker Compose:

```bash
docker compose up --build whatsapp-api-webhook-server-cpp
```

Server will be started after building automatically.


## Running the app

The application binary is placed in ```build/bin/```.

This project use ```config.json``` to set these variables:
- Address (default: ```:5000```). Server will be launched on this port. Requests should be sent to this port. [Instance setup](https://green-api.com/en/docs/api/receiving/technology-webhook-endpoint/#webhookUrl);
- Pattern (default: ```/```). Part of URI after port: <Address><Pattern>. All requests sent to wrong Pattern will be rejected. By default server will handle requests on URI = localhost:5000/. [Instance setup](https://green-api.com/en/docs/api/receiving/technology-webhook-endpoint/#webhookUrl);
- WebhookToken (default: **none**). Authorization token, must be equal to a token declared in your green-api instance (none dy default). [Instance setup](https://green-api.com/en/docs/api/receiving/technology-webhook-endpoint/#webhookUrl);
- LoggingType (default: ```console```). Available values: ```logger```, ```console```. With ```logger``` all logs will be saved to file, with ```console``` all logs will be printed with std::cout;
- LoggerFilename (default: ```log.txt```). Filename for logger's file.

After starting the application, a server using values from config will be started. If no config exists, default values will be used.

You can use [Postman collection](https://green-api.com/en/docs/postman-collection/) for server testing.

## User Adapter

The user adapter located in:
- ```sources/user_adapter.cpp```
- ```include/user_adapter.h```

If these files does not exist, create them by renaming ```sources/_user_adapter.cpp``` and  ```include/_user_adapter.h```.

User Adapter contains your handlers for incoming webhooks. It works according to the following algorithm:
1. Request to the server is received by ```webhook``` class;
2. ```webhook``` class creates ```Response``` object and transmits request body to the ```validator``` class.
3. After validation, ```Response``` object transmits into ```UserAdapter``` handler, based on request's body ```webToken```.
4. ```User Adapter``` function returns ```true``` if error or ```false``` if no error. Based in this value, server will return 200 OK or 400 Bad Request status.

The structure of ```Response``` object (response.h):

```
struct Response {
```
-    ```bool error = true; ``` — error is true if incoming webhook failed to validate, false otherwise;
-    ```std::string typeWebhook = ""; — ``` webhookType taken from request body;
-    ```std::string bodyStr = ""; ``` — contains request body if error = false, otherwise contains validation error description;
-    ```nlohmann::json bodyJson = ""; — ``` body of incoming request;
```
};
```

4. UserAdapter function defines as:

```
#define ON_WEBHOOK_TYPE_EXISTS
static bool onWebhookType(greenapi::Response& body);
```

A #define is used by ```webhook``` class. You could safely remove unnessesary for you UserAdapter functions.

5. UserAdapter function example:

In this example, handler will be called by webhook with type ```IncomingMessageReceived```. Using the structure ```Response``` above, you could check for validate of request (```body.error```), work with webhook json structure (```body.bodyJson```) or get access to webhook raw body (```body.bodyStr```).

```
bool UserAdapter::onIncomingMessageReceived(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If you encountered errors while hanlding, you should return true.
    // It will change response status to 400 Bad Request with immediate return of the HTTP request result
    // 
    // if (<error>) {
    //    return true;
    //}

    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");

    // Write your handler here:

    // Return false if no error, after this 200 OK response will be returned
    return false;
}
```

## Examples

Examples are available in [examples.cpp](./examples/example.cpp).

## JSON Validation

JSON schemas for webhooks validation are placed in ```jsonSchema``` directory and copied to build directory while running build script. You could add any ```.json``` files to ```jsonSchema```, they will be loaded into a program while it starting.

JSON schemas have this structure:

```
{
  "$id": "schemas",
  "$schema": "https://json-schema.org/draft/2020-12/schema",
  "type": "object",
  "properties": {
    "yourNameOfObject": {
      "$schema": "https://json-schema.org/draft/2020-12/schema",
      "type": "object",
      "properties": {
        "sampleField": {
          "type": "string"
        },
        "sampleRef_Field": {
          "$ref": "#/properties/commonSchemaComponents/properties/senderData"
        }
      },
      "required": [
        "typeWebhook",
      ],
      "additionalProperties": true
    },
    "yourOtherObject": {
        ...
    }
  }
}
```

To make your ```.json``` file working, any ```.json``` file should contain only one object with ```"properties"```. All to-be-validated objects should be on ```"properties"``` object. Otherwise, your json file will be ignored.

## Webhooks documentation

[https://green-api.com/docs/api/](https://green-api.com/en/docs/api/receiving/notifications-format/).

## Library dependencies

- [poco](https://github.com/pocoproject/poco) — for HTTP server.
- [nlohmann-json](https://github.com/nlohmann/json) — to work with JSON.
- [json-schema-validator](https://github.com/pboettch/json-schema-validator) — for JSON validation.

## Licence

Licences under [
Creative Commons Attribution-NoDerivatives 4.0 International (CC BY-ND 4.0)
](https://creativecommons.org/licenses/by-nd/4.0/).
[LICENSE](../LICENSE).
