//
// Created by Dom on 6.11.2026.
//

#ifndef WEBSERVER_HTTP_REQUEST_H
#define WEBSERVER_HTTP_REQUEST_H
#include <string>
#include <map>

namespace http_request {

    struct request {
        std::string method;
        std::string url;
        std::string version;
        std::map<std::string,std::string> headers;
        std::string body;
    };

    void parseRequest(std::string& request);
}
#endif //WEBSERVER_HTTP_REQUEST_H
