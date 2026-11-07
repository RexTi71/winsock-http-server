//
// Created by Dom on 6.11.2026.
//

#ifndef WEBSERVER_HTTP_RESPONSE_H
#define WEBSERVER_HTTP_RESPONSE_H
#include <string>
#include <fstream>
#include <format>
#include "http_request.h"

namespace http_response {
    //TODO:dodac wiecej kodow
    enum StatusCode {
        Ok = 200,
        Accepted = 202,
        Bad_Request = 400,
        Unathorized = 401,
        Forbidden = 403,
        Not_Found = 404
    };
    std::string getStatusCodeName(const StatusCode& status_code);
    struct response {
        std::string version;
        StatusCode status_code;
        std::map<std::string, std::string> headers;
        std::string body;

        std::string toString();
    };
    response createResponse(const http_request::request& request_struct);

}
#endif //WEBSERVER_HTTP_RESPONSE_H
