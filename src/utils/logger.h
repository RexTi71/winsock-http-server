//
// Created by Dom on 8.11.2026.
//

#ifndef WEBSERVER_LOGGER_H
#define WEBSERVER_LOGGER_H
#include <string>
#include <iostream>
#include <windows.h>
namespace {

enum Level {Debug = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, Info = FOREGROUND_GREEN, Warn = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, Error = FOREGROUND_RED };
std::string getLevelName(const Level& level);
void log(const Level& level, const std::string& message);
}
namespace logger {
    void debug(const std::string& message);
    void info(const std::string& message);
    void warn(const std::string& message);
    void error(const std::string& message);
}
#endif //WEBSERVER_LOGGER_H
