//
// Created by Dom on 6.11.2026.
//

#ifndef WEBSERVER_WEB_SOCKET_EXCEPTION_H
#define WEBSERVER_WEB_SOCKET_EXCEPTION_H
#include <exception>
#include <string>

class WebSocketException : public std::exception {
public:
    explicit WebSocketException(const std::string& message):msg(message){}

    virtual const char* what() const noexcept override {
        return msg.c_str();
    }

private:
    std::string msg;
};
#endif //WEBSERVER_WEB_SOCKET_EXCEPTION_H
