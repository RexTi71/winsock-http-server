//
// Created by Dom on 8.11.2026.
//

#include "logger.h"

#include <windows.h>
#include <chrono>
#define DEFAULT_TEXT_COLOR 7

namespace {

void log(const Level& level, const std::string& message) {
    const HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    auto now = std::chrono::floor<std::chrono::milliseconds>(
                   std::chrono::system_clock::now());
    std::string timestamp = std::format("{:%Y-%m-%d %H:%M:%S}", now);
    SetConsoleTextAttribute(h, level);
    std::cout << timestamp << " " << getLevelName(level);
    SetConsoleTextAttribute(h,DEFAULT_TEXT_COLOR);
    std::cout << " : " << message << std::endl;
}

std::string getLevelName(const Level &level) {
    switch (level){
        case Debug: return "[DEBUG]";
        case Info: return "[INFO]";
        case Warn: return "[WARN]";
        case Error: return "[ERROR]";
        default: return "";
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