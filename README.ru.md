# whatsapp-api-webhook-server-cpp

## Поддержка

[![Support](https://img.shields.io/badge/support@green--api.com-D14836?style=for-the-badge&logo=gmail&logoColor=white)](mailto:support@green-api.com)
[![Support](https://img.shields.io/badge/Telegram-2CA5E0?style=for-the-badge&logo=telegram&logoColor=white)](https://t.me/greenapi_support_ru_bot)
[![Support](https://img.shields.io/badge/WhatsApp-25D366?style=for-the-badge&logo=whatsapp&logoColor=white)](https://wa.me/447458038017)

## Руководства и новости

[![Guides](https://img.shields.io/badge/YouTube-%23FF0000.svg?style=for-the-badge&logo=YouTube&logoColor=white)](https://www.youtube.com/@green-api)
[![News](https://img.shields.io/badge/Telegram-2CA5E0?style=for-the-badge&logo=telegram&logoColor=white)](https://t.me/green_api)
[![News](https://img.shields.io/badge/WhatsApp-25D366?style=for-the-badge&logo=whatsapp&logoColor=white)](https://whatsapp.com/channel/0029VaHUM5TBA1f7cG29nO1C)

- [Documentation in English](https://github.com/green-api/whatsapp-api-client-cpp/blob/master/README.md).


`whatsapp-api-webhook-server-cpp` — вебхук сервер для интеграции с мессенджером WhatsApp Messenger через API сервиса [green-api.com](https://green-api.com/). 
Для работы с сервером нужно получить регистрационный токен и ID аккаунта в [личном кабинете](https://console.green-api.com/). Есть бесплатный тариф аккаунта разработчика.

- [whatsapp-api-webhook-server-cpp](#whatsapp-api-webhook-server-cpp)
  - [Поддержка](#поддержка)
  - [Руководства и новости](#руководства-и-новости)
  - [API](#api)
  - [Получение Webhook Token](#получение-webhook-token)
  - [Сборка приложения](#сборка-приложения)
    - [Windows](#windows)
    - [Linux](#linux)
    - [Docker](#docker)
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

Чтобы получить Webhook Token и иметь возможность отправлять запросы на этот сервер, необходимо авторизовать учетную запись WhatsApp в приложении для телефона. Чтобы авторизовать учетную запись, зайдите в [личный кабинет](https://console.green-api.com/) и отсканируйте QR-код с помощью приложения WhatsApp.

## Сборка приложения

Это приложение использует C++ 17, CMake 3.5, поддерживает компиляторы Linux (GCC) и Windows (Visual Studio 2019/2022).

Перед сборкой вам необходимо создать следующий файл:

- ```source/user_adapter.cpp```

Если у вас нет требуемого файла, создайте его путем удаления нижнего подчеркивания из названия  ```source/_user_adapter.cpp`` файла.

Мы будем обновлять ```include/user_adapter.h``` и ```source/_user_adapter.cpp``` файлы по мере выпуска новых вебхуков. Если вы столкнулись с ошибкой сборки, где написано о том, что компилятор не смог найти требуемые функции из ```user_adapter```, то в этом случае вам необходимо добавить новые функции из ```source/_user_adapter.cpp``` в ваш ```source/user_adapter.cpp```.

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

Приложение по умолчанию собирается с типом конфигурации Release. Файл конфигурации ```config.json``` и директория ```jsonSchema``` копируются в директорию ```build\bin```. Приложение дает приоритет файлам ```build\bin```, загружая файлы с директории проекта, только если не может найти файлы в ```build\bin```.

После успешной сборки, повторная сборка приложения доступна через скрипт ```build.bat``` или

```bash
cmake --build build --config=Release
```

Исполняемый файл приложения находится в ```build/bin/```. 

Запуск приложения:
```bash
start build\bin\whatsapp-api-webhook-server-cpp.exe
```

При исполнении программы создается сервер на порте из конфигурации ```config.json``` (по умолчанию 5000). Подробное описание конфигурации доступно [здесь](#запуск-приложения).

### Linux

Для сборки приложения необходимы git, g++, cmake:

```bash
sudo apt-get install git g++ cmake
```

```cmake``` и ```git``` должны быть доступны в Bash.

Сделайте git clone для загрузки проекта и перейдите в директорию. Сборка скрипта выполняется запуском скрипта ```build.sh```:

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

Исполняемый файл приложения находится в ```build/bin/```. Файл конфигурации ```config.json``` и директория ```jsonSchema``` копируются в директорию ```build/bin```. Приложение дает приоритет файлам ```build/bin```, загружая файлы с директории проекта, только если не может найти файлы в ```build/bin```.

Запуск приложения:
```bash
./build/bin/whatsapp-api-webhook-server-cpp
```

При исполнении программы создается сервер на порте из конфигурации ```config.json``` (по умолчанию 5000). Подробное описание конфигурации доступно [здесь](#запуск-приложения).

### Docker

Для запуска сервера через Docker вам понадобится Docker и Docker Compose.

Вы можете установить [Docker Desktop](https://www.docker.com/products/docker-desktop/) для всех платформ или установить [Docker Engine](https://docs.docker.com/engine/install/) для Linux.

Сделайте git clone для загрузки проекта и перейдите в директорию:

```bash
git clone --branch=master --depth=1 https://github.com/green-api/whatsapp-api-webhook-server-cpp
cd whatsapp-api-webhook-server-cpp
```

Перед сборкой контейнера Docker вам необходимо создать следующий файл:

- ```source/user_adapter.cpp```

Если у вас нет требуемого файла, создайте его путем удаления нижнего подчеркивания из названия  ```source/_user_adapter.cpp`` файла.

Мы будем обновлять ```include/user_adapter.h``` и ```source/_user_adapter.cpp``` файлы по мере выпуска новых вебхуков. Если вы столкнулись с ошибкой сборки, где написано о том, что компилятор не смог найти требуемые функции из ```user_adapter```, то в этом случае вам необходимо добавить новые функции из ```source/_user_adapter.cpp``` в ваш ```source/user_adapter.cpp```

По умолчанию в образе открыт ```port 5000```. Если вы хотите изменить порт, тогда:

- Поменяйте поле ```Address``` в ```config.json``` на требуемый вами порт;

- Поменяйте поле ```ports``` в ```docker-compose.yaml``` на требуемый вами порт;

Запустите Docker образ с помощью Docker Compose. Используйте флаг ```--build```, если вы запускаете контейнер в первый раз или вы изменили файлы в проекте:

```bash
docker compose up --build
```

Сервер будет запущен автоматически после сборки проекта. Подробное описание конфигурации доступно [здесь](#запуск-приложения).

## Запуск приложения

Исполняемый файл приложения находится в ```build/bin/```.

Сервером используется ```config.json``` для установки следующих значений:

- Address (по умолчанию: ```:5000```). Сервер будет запущен на этом порту. Запросы должны отправляться на этот порт. [Настройка инстанса](https://green-api.com/docs/api/receiving/technology-webhook-endpoint/#webhookUrl);

- Pattern (по умолчанию: ```/```). Часть URI после порта: "Address""Pattern". Все запросы, отправленные на неверный pattern, будут отклонены. По умолчанию сервер обрабатывает запросы по URI = ```localhost:5000/```. [Настройка инстанса](https://green-api.com/docs/api/receiving/technology-webhook-endpoint/#webhookUrl);

- WebhookToken (по умолчанию: **отсутствует**). Токен авторизации в заголовке приходящего запроса должен совпадать с токеном в вашем инстансе green-api (по умолчанию отсутствует). [Настройка инстанса](https://green-api.com/docs/api/receiving/technology-webhook-endpoint/#webhookUrl);

- LogToFile (по умолчанию: ```false```). Данный флаг отвечает за создание файла и запись логов в него. Доступные значения: true, false.

- LogToConsole (по умолчанию: ```false```). Данный флаг отвечает за запись логов в консоль. Доступные значения: true, false.

- LoggerFilename (по умолчанию: ```log.txt```). Имя файла логгера.

После запуска приложения будет запущен сервер, использующий значения из конфига. Если конфиг отсутствует, будут использоваться значения по умолчанию.

Вы можете использовать [Коллекцию Postman](https://green-api.com/docs/postman-collection/) для тестирования сервера.

## User Adapter

Для указания пользовательских функций при получении уведомления используются файлы ```user_adapter```. В данных файлах вам требуется указать функции обработки уведомлений (например: запись уведомления в базу, отправка запроса в другой микросервис). В качестве шаблона можно переименовать файл ```source/_user_adapter.cpp```. Пример обработки всех типов уведомлений доступен в файле ```user_adapter_example.cpp``` в папке ```examples```.

Адаптер для пользователя находится в следующем файле:

- ```source/user_adapter.cpp```

Если у вас нет требуемого файла, создайте его путем удаления нижнего подчеркивания из названия  ```source/_user_adapter.cpp`` файла.

Мы будем обновлять ```include/user_adapter.h``` и ```source/_user_adapter.cpp``` файлы по мере выпуска новых вебхуков. Если вы столкнулись с ошибкой сборки, где написано о том, что компилятор не смог найти требуемые функции из ```user_adapter```, то в этом случае вам необходимо добавить новые функции из ```source/_user_adapter.cpp``` в ваш ```source/user_adapter.cpp```.

Адаптер для пользователя содержит ваши обработчики вебхуков. Программа работает по следующему алгоритму:

1. Запрос к серверу принимает класс ```webhook```;

2. Класс ```webhook``` создает объект ```Response``` и передает тело запроса классу ```Validator```;

3. После проверки, объект ```Response``` передается в обработчик ```User Adapter``` на основе ```webToken``` в теле запроса;

4. Функция обработки в ```User Adapter``` возвращает ```true``` в случае ошибки или ```false```, если обработка произошла без ошибок. На основе этого значения, сервер вернет или 200 OK или 400 Bad Request.


Структура объекта ```Response``` (response.h):

```cpp
struct Response {
    bool error = true; // true, если вебхук не прошел валидацию
    std::string typeWebhook = ""; // webhookType из тела запроса
    std::string bodyStr = "";  // содержит тело запроса, если error = false, иначе описание ошибки валидации
    nlohmann::json bodyJson = ""; //  тело пришедшего запроса
}
```

1. Функции в UserAdapter описываются как:

```cpp
static bool onWebhookType(greenapi::Response& body);
```

2. Пример функции UserAdapter:

В данном примере, обработчик будет вызван вебхуком с типом ```IncomingMessageReceived```. С помощью структуры ```Response```, описанной выше, вы можете проверить результат валидации запроса (```body.error```), обратиться к json структуре вебхука (```body.bodyJson```) или получить доступ к телу запроса (```body.bodyStr```).

```cpp
bool UserAdapter::onIncomingMessageReceived(greenapi::Response& body) {
    // Каждый запрос содержит typeWebhook. Если typeWebhook нет в запросе, запрос отклоняется сервером.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // Если вам нужно вернуть ошибку в процессе обработки запроса, верните true из этой функции.
    // Это изменит статус запроса на 400 Bad Request с немедленным возвратом результата HTTP запроса.
    // 
    // if (<error>) {
    //    return true;
    //}

    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");

    // Опишите ваш обработчик здесь:

    // Если нет ошибок, верните false. После этого сервер вернет статус запроса 200 OK
    return false;
}
```

## Примеры

Примерны доступны в [user_adapter_example.cpp](./examples/user_adapter_example.cpp).

## Документация по методам вебхуков

[https://green-api.com/docs/api/](https://green-api.com/docs/api/receiving/notifications-format/).

## Валидация JSON

JSON схемы для проверки вебхуков расположены в директории ```jsonSchema``` и копируются в директорию сборки при запуске скрипта сборки. Вы можете добавлять любые файлы ```.json``` в ```build/bin/jsonSchema```, они будут загружены в программу при ее запуске.

JSON схемы имеют следующую структуру:

```json
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
