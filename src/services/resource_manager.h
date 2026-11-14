//
// Created by Dom on 12.11.2026.
//

#ifndef WEBSERVER_RESOURCE_MANAGER_H
#define WEBSERVER_RESOURCE_MANAGER_H

#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include "../parsers/http_request.h"
#include "../parsers/http_response.h"

using json = nlohmann::json;

namespace resource_manager {

    struct result {
        http_response::StatusCode status_code;
        std::string body;
    };

    ///Class controling http requests to users repository
    class UsersRepository {
    public:
        UsersRepository();

        /**
         * @brief Dispatches the request to the handler matching its HTTP method.
         * @param request_struct Parsed request (method, url, query, version...)
         * @return result containing status code and string body, created from given request or error message
         */
        result handleRequest(const http_request::request& request_struct);
    private:
        const std::string FILE_PATH_ROOT = "../resources";
        std::string file_path;
        result getRequest(const http_request::request& request_struct);
        result postRequest(const http_request::request& request_struct);
        result deleteRequest(const http_request::request& request_struct);
        result putRequest(const http_request::request& request_struct);
        result patchRequest(const http_request::request& request_struct);
        json getJsonFromFile() const;
    };
}
#endif //WEBSERVER_RESOURCE_MANAGER_H
