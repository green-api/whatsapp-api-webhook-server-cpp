#include "logger.h"

namespace greenapi {
    std::string logCfgType {"console"};
};

void greenapi::Logger::Log(const std::string &message, const std::string &level) {
    std::string message_cleared;
    message_cleared.reserve(message.size());
    for (std::string::const_iterator it = message.begin(); it != message.end(); ++it) {
        if ((*it) != '\r' && (*it) != '\n') {
            message_cleared += (*it);
        }
    }
    
    std::ofstream logfile;
    if (greenapi::logCfgType == "logger") {
        logfile.open("log.txt", std::ios::app);
        if (!logfile.is_open()) {
            std::cout << "Unable to open log.txt file. Falling back to console output.\n";
            logCfgType = "console";
        }
    }

    if (level == "error") {
		std::string messageLog = "{ \"timestamp\": \"" + Logger::getCurrentTime() + "\", \"level\": \"" + level + "\", \"message\": \"" + message_cleared + "\" }";
		std::cout << messageLog << std::endl;
        if (greenapi::logCfgType == "logger") {
            logfile << messageLog << std::endl;
        }
    } else {
        std::string messageLog = "{ \"timestamp\": \"" + Logger::getCurrentTime() + "\", \"level\": \"" + level + "\", \"message\": \"" + message_cleared + "\" }";
        if (greenapi::logCfgType == "console") {
            std::cout << messageLog << std::endl;
        } else if (greenapi::logCfgType == "logger") {
            logfile << messageLog << std::endl;
        }
    }
    logfile.close();
}

const std::string greenapi::Logger::getCurrentTime() {
	auto now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	std::tm now_tm;
    #if defined(_MSC_VER) || defined(__MINGW32__)
    		localtime_s(&now_tm, &now_time);
    #else
    		localtime_r(&now_time, &now_tm);
    #endif
	char buffer[20];
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &now_tm);
	return std::string(buffer);
}

const std::string greenapi::Logger::hideStringPart(const std::string& input, size_t start, size_t length) {
	if (start > input.size() || start + length > input.size()) {
        std::cout << "Logger::hideStringPart: Invalid start or length. start = " << start <<  ", length = " << length << "\n";
        return "";
	}
	std::string result = input;
	for (size_t i = start; i < start + length; ++i) {
		result[i] = '*';
	}
	return result;
}