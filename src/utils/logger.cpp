//
// Created by Dom on 8.11.2026.
//

#include "logger.h"

#include <windows.h>

namespace {

void log(const Level& level, const std::string& message) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, level);
    std::cout << getLevelName(level);
    SetConsoleTextAttribute(h,7);
    std::cout << " : " << message << std::endl;
}

std::string getLevelName(const Level &level) {
    switch (level){
        case Debug: return "DEBUG";
        case Info: return "INFO";
        case Warn: return "WARN";
        case Error: return "ERROR";

    }
}

}
namespace logger {
    void debug(const std::string& message) {
        log(Debug, message);
    }
    void info(const std::string& message) {
        log(Info, message);
    }
    void warn(const std::string& message) {
        log(Warn, message);
    }
    void error(const std::string& message) {
        log(Error, message);
    }
}