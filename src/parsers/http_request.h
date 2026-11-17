//
// Created by Dom on 6.11.2026.
//

#ifndef WEBSERVER_HTTP_REQUEST_H
#define WEBSERVER_HTTP_REQUEST_H
#include <string>
#include <map>

#include "http_request.h"

namespace http_request {
    enum Method {
        Get,
        Post,
        Delete,
        Put,
        Patch,
        Unknown
    };
    /// converts enums name to a string
    std::string getMethodName(const Method& method);
    Method convertStrToMethodName(const std::string& input);

    struct request {
        Method method;
        std::string url;
        std::map<std::string, std::string> parameters;
        std::string version;
        std::map<std::string,std::string> headers;
        std::string body;
    };

    /**
     * @brief Converts given request to a structured type
     * @return request struct
     */
    request parseRequest(std::string& request);
}
#endif //WEBSERVER_HTTP_REQUEST_H
