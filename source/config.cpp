#include "config.h"

using namespace greenapi;

const std::map<std::string, std::string> Config::LoadServerConfig(const std::string &filename) {
    std::map<std::string, std::string> config = {
        {"Address", ":5000"},
        {"Pattern", "/"},
        {"WebhookToken", ""},
        {"LoggingType", "console"},
        {"LoggerFilename", "log.txt"}
    };

    std::vector<std::string> directories = {
        "build/" + filename,
        "build/bin/" + filename,
        "bin/" + filename,
        filename,
        "./../" + filename,
    };

    std::string filenamePath;
    bool found {false};
    for (const auto& p : directories) {
        if (std::filesystem::exists(p)) {
            filenamePath = p;
            found = true;
            break;
        }
    }

    if (!found) {
        Logger::Log("Unable to find config file: " + filename, "error");
        return config;
    }

    std::ifstream f(filenamePath);
    if (!f.good()) {
         f.close();
         Logger::Log("Unable to open config file: " + filename, "error");
         return config;
    }
    std::ostringstream buffer;
    buffer << f.rdbuf();
    std::string cfgStr = buffer.str();

    if (nlohmann::json::accept(cfgStr)) {
        nlohmann::json cfg = nlohmann::json::parse(cfgStr);
        if (cfg.contains("Address")) {
            config["Address"] = cfg["Address"];
        }
        if (cfg.contains("Pattern")) {
            config["Pattern"] = cfg["Pattern"];
        }
        if (cfg.contains("WebhookToken")) {
            config["WebhookToken"] = cfg["WebhookToken"];
        }
        if (cfg.contains("LoggingType")) {
            config["LoggingType"] = cfg["LoggingType"];
            greenapi::logCfgType = config["LoggingType"];
        }
        if (cfg.contains("LoggerFilename")) {
            config["LoggerFilename"] = cfg["LoggerFilename"];
            greenapi::logCfgFilename = config["LoggerFilename"];
        }
    }
    f.close();
    
    if (config["LoggingType"] != "console" && config["LoggingType"] != "logger") {
        config["LoggingType"] = "console";
    }
    if (config["WebhookToken"].rfind("Basic ", 0) == 0) {
        config["WebhookToken"] = config["WebhookToken"].substr(6);
    } else if (config["WebhookToken"].rfind("Bearer ", 0) == 0) {
        config["WebhookToken"] = config["WebhookToken"].substr(7);
    }

    return config;
}

nlohmann::json Config::OpenSchemas() {
    nlohmann::json schemas;

    std::vector<std::string> directories = {
        "jsonSchema",
        "./../jsonSchema",
        "build/jsonSchema",
        "build/bin/jsonSchema",
        "bin/jsonSchema"
    };

    std::filesystem::path jsonSchemaDir;
    bool found = false;

    for (const auto& dir : directories) {
        if (std::filesystem::exists(dir)) {
            jsonSchemaDir = dir;
            found = true;
            break;
        }
    }

    if (!found) {
        Logger::Log("Unable to find jsonSchema directory", "error");
        return schemas;
    }

    for (const auto& entry : std::filesystem::directory_iterator(jsonSchemaDir)) {
        if (entry.path().extension() == ".json") {
            std::ifstream f(entry.path());
            if (f.good()) {
                std::ostringstream buffer;
                buffer << f.rdbuf();
                std::string validationRaw = buffer.str();

                if (!nlohmann::json::accept(validationRaw)) {
                    Logger::Log("Validation file content error in " + entry.path().string(), "error");
                } else {
                    nlohmann::json parsedJson = nlohmann::json::parse(validationRaw);
                    if (!parsedJson.contains("properties")) {
                        Logger::Log("No properties in JSON: " + entry.path().string(), "error");
                    }
                    if (schemas.is_null()) {
                        schemas = parsedJson;
                    } else {
                        if (schemas.contains("properties") && parsedJson.contains("properties")) {
                            schemas["properties"].update(parsedJson["properties"]);
                        }
                    }
                }
            } else {
                Logger::Log("Unable to open JSON file: " + entry.path().string(), "error");
            }
        }
    }
    return schemas;
}