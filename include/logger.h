#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iostream>
#include <regex>

namespace greenapi {
    extern std::string logCfgType;
    extern std::string logCfgFilename;
    
    class Logger {
        public:
            static void Log(const std::string &message, const std::string &level);
        private:
            static const std::string getCurrentTime();
            static const std::string hideStringPart(const std::string& input, size_t start, size_t length);
            Logger() {}
    };
}

#endif