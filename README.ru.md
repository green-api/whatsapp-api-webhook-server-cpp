# whatsapp-api-webhook-server-cpp


![](https://img.shields.io/badge/license-CC%20BY--ND%204.0-green)

## Поддержка

[![Support](https://img.shields.io/badge/support@green--api.com-D14836?style=for-the-badge&logo=gmail&logoColor=white)](mailto:support@green-api.com)
[![Support](https://img.shields.io/badge/Telegram-2CA5E0?style=for-the-badge&logo=telegram&logoColor=white)](https://t.me/greenapi_support_ru_bot)
[![Support](https://img.shields.io/badge/WhatsApp-25D366?style=for-the-badge&logo=whatsapp&logoColor=white)](https://wa.me/447458038017)

## Руководства и новости

[![Guides](https://img.shields.io/badge/YouTube-%23FF0000.svg?style=for-the-badge&logo=YouTube&logoColor=white)](https://www.youtube.com/@green-api)
[![News](https://img.shields.io/badge/Telegram-2CA5E0?style=for-the-badge&logo=telegram&logoColor=white)](https://t.me/green_api)
[![News](https://img.shields.io/badge/WhatsApp-25D366?style=for-the-badge&logo=whatsapp&logoColor=white)](https://whatsapp.com/channel/0029VaHUM5TBA1f7cG29nO1C)

- [Documentation in English](https://github.com/green-api/whatsapp-api-client-cpp/blob/master/README.md).


`whatsapp-api-webhook-server-cpp` — вебхук сервер для интеграции с мессенджером WhatsApp messenger через API сервиса [green-api.com](https://green-api.com/). 
Для работы с сервером нужно получить регистрационный токен и ID аккаунта в [личном кабинете](https://console.green-api.com/). Есть бесплатный тариф аккаунта разработчика.

- [whatsapp-api-webhook-server-cpp](#whatsapp-api-webhook-server-cpp)
  - [Поддержка](#поддержка)
  - [Руководства и новости](#руководства-и-новости)
  - [API](#api)
  - [Получение Webhook Token](#получение-webhook-token)
  - [Сборка приложения](#сборка-приложения)
    - [Windows](#windows)
    - [Linux](#linux)
  - [Запуск приложения](#запуск-приложения)
  - [User Adapter](#user-adapter)
  - [Примеры](#примеры)
  - [Документация по методам вебхуков](#документация-по-методам-вебхуков)
  - [Валидация JSON](#валидация-json)
  - [Зависимости приложения](#зависимости-приложения)
  - [Лицензия](#лицензия)

## API

Документация к вебхукам находится по [ссылке](https://green-api.com/docs/api/receiving/notifications-format/). Приложение является обработчиком вебхуков, поэтому документация по ссылке выше применима и к самому приложению.

## Получение Webhook Token

Чтобы получить Webhook Token и иметь возможность отправлять запросы на этот сервер, необходимо авторизовать учетную запись WhatsApp в приложении для телефона. Чтобы авторизовать учетную запись, зайдите в свой [кабинет](https://console.green-api.com/) и отсканируйте QR-код с помощью приложения WhatsApp.

## Сборка приложения

Это приложение использует C++ 17, CMake 3.5, поддерживает компиляторы Linux (GCC) и Windows (Visual Studio 2019/2022).

Перед сборкой вам необходимо создать эти файлы:
- ```include/user_adapter.h```
- ```source/user_adapter.cpp```

Переименуйте следующие файлы, удалив нижнее подчеркивание из названия: ```include/_user_adapter.h``` и ```source/_user_adapter.h```.

**Проект не будет собран, если это не будет сделано**.

### Windows

Для сборки приложения необходимо:

- [git](https://gitforwindows.org/) - кроссплатформенная утилита, используемая в этом проекте для загрузки библиотек.
- [CMake](https://cmake.org/download/) - кроссплатформенная утилита для автоматического создания ПО из исходного кода.
- Компилятор [Microsoft Visual C++ (MSVC)](https://visualstudio.microsoft.com/ru/downloads/) для C++ приложений.

```cmake``` и ```git``` должны быть доступны в PATH.

Сборка выполняется запуском скрипта ```build.bat``` (или ```.\build.bat``` для Powershell):

```bash
git clone --branch=master --depth=1 https://github.com/green-api/whatsapp-api-webhook-server-cpp
cd whatsapp-api-webhook-server-cpp
build.bat
```

Приложение по умолчанию собирается с типом конфигурации Release.

После успешной сборки, повторная сборка приложения доступна через скрипт ```build.bat``` или

```bash
cmake --build build --config=Release
```

### Linux

Для сборки приложения необходимы git, g++, cmake:

```bash
sudo apt-get install git g++ cmake
```

```cmake``` и ```git``` должны быть доступны в Bash.

Сборка выполняется запуском скрипта ```build.sh```:

```bash
git clone --branch=master --depth=1 https://github.com/green-api/whatsapp-api-webhook-server-cpp
cd whatsapp-api-webhook-server-cpp
.\build.sh
```

Приложение по умолчанию собирается с типом конфигурации Release.

После успешной сборки, повторная сборка приложения доступна через скрипт ```.\build.sh``` или

```bash
cmake --build build --config=Release
```

## Запуск приложения

Исполняемый файл приложения находится в ```build/bin/```.

Сервером используется ```config.json``` для установки следующих значений:
- Address (по умолчанию: ```:5000```). Сервер будет запущен на этом порту. Запросы должны отправляться на этот порт. [Настройка инстанса](https://green-api.com/en/docs/api/reception/technology-webhook-endpoint/#webhookUrl);
- Pattern (по умолчанию: ```/```). Часть URI после порта: [Address][Pattern]. Все запросы, отправленные на неверный pattern, будут отклонены. По умолчанию сервер обрабатывает запросы по URI = ```localhost:5000/```. [Настройка инстанса](https://green-api.com/en/docs/api/reception/technology-webhook-endpoint/#webhookUrl);
- WebhookToken (по умолчанию: **отсутствует**). Токен авторизации в заголовке приходящего запроса должен совпадать с токеном в вашем инстансе green-api (по умолчанию отсутствует). [Настройка инстанса](https://green-api.com/en/docs/api/reception/technology-webhook-endpoint/#webhookUrl);
- LoggingType (по умолчанию: ```console```). Доступные значения: ```logger```, ```console```. При ```logger``` все логи будут сохраняться в файл, при ```console``` все логи будут выводиться с помощью std::cout;
- LoggerFilename (по умолчанию: ```log.txt```). Имя файла логгера.

После запуска приложения будет запущен сервер, использующий значения из конфига. Если конфиг отсутствует, будут использоваться значения по умолчанию.

Вы можете использовать [Коллекцию Postman](https://green-api.com/docs/postman-collection/) для тестирования сервера.

## User Adapter

Адаптер для пользователя находится в следующих файлах:
- ```sources/user_adapter.cpp```
- ```include/user_adapter.h```

Если этих файлов не существует, создайте их, переименовав ```sources/_user_adapter.cpp``` и  ```include/_user_adapter.h```.

Адаптер для пользователя содержит ваши обработчики вебхуков. Программа работает по следующему алгоритму:
1. Запрос к серверу принимает класс ```webhook```;
2. Класс ```webhook``` создает объект ```Response``` и передает тело запроса классу ```Validator```.
3. После проверки, объект ```Response``` передается в обработчик ```UserAdapter``` на основе ```webToken``` в теле запроса.

Структура объекта ```Response``` (response.h):

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

4. Функции в UserAdapter описываются как:

```
#define ON_WEBHOOK_TYPE_EXISTS
static void onWebhookType(greenapi::Response& body);
```

#define используется классом ```webhook```. Вы можете безопасно удалить неиспользуемые вами функции в UserAdapter.

5. Пример функции UserAdapter:

В данном примере, обработчик будет вызван вебхуком с типом ```IncomingMessageReceived```. С помощью структуры ```Response```, описанной выше, вы можете проверить результат валидации запроса (```body.error```), обратиться к json структуре вебхука (```body.bodyJson```) или получить доступ к телу запроса (```body.bodyStr```).

```
void UserAdapter::onIncomingMessageReceived(greenapi::Response& body) {
    const auto typeWebhook = body.bodyJson["typeWebhook"];
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "info");
        return;
    }
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");
    // Write your handler here:

}
```

## Примеры

Примерны доступны в [examples.cpp](./examples/example.cpp).

## Документация по методам вебхуков

[https://green-api.com/docs/api/](https://green-api.com/en/docs/api/receiving/notifications-format/).

## Валидация JSON

JSON схемы для проверки вебхуков расположены в директории ```jsonSchema``` и копируются в директорию сборки при запуске скрипта сборки. Вы можете добавлять любые файлы ```.json``` в ```jsonSchema```, они будут загружены в программу при ее запуске.

JSON схемы имеют следующую структуру:

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

Для работоспособности вашего ```.json```файла, любой ```.json``` файл должен содержать только один объект с ```"properties"```. Все объекты для валидации должны находиться в объекте ```"properties"```. В противном случае файл JSON будет проигнорирован.

## Зависимости приложения

- [poco](https://github.com/pocoproject/poco) — для HTTP сервера.
- [nlohmann-json](https://github.com/nlohmann/json) — для работы с JSON.
- [json-schema-validator](https://github.com/pboettch/json-schema-validator) — для JSON валидации.

## Лицензия

Лицензировано на условиях [
Creative Commons Attribution-NoDerivatives 4.0 International (CC BY-ND 4.0)
](https://creativecommons.org/licenses/by-nd/4.0/).
[LICENSE](../LICENSE).