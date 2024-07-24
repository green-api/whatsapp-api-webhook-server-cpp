#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>

#include "logger.h"

namespace greenapi {
    class Config {
        public:
            static const std::map<std::string, std::string> LoadServerConfig(const std::string &filename);
            static nlohmann::json OpenSchemas();
        private:
            Config() {}
    };
}


#endif