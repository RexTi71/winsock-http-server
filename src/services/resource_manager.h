//
// Created by Dom on 12.11.2026.
//

#ifndef WEBSERVER_RESOURCE_MANAGER_H
#define WEBSERVER_RESOURCE_MANAGER_H

#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include "../parsers/http_request.h"

using json = nlohmann::json;

namespace resource_manager {
    class UsersRepository {
    public:
        UsersRepository();
        std::string handleRequest(const http_request::request& request_struct);
    private:
        const std::string FILE_PATH_ROOT = "../resources";
        std::string file_path;
        std::string getRequest(const http_request::request& request_struct);
        std::string postRequest(const http_request::request& request_struct);
        std::string deleteRequest(const http_request::request& request_struct);
        std::string patchRequest(const http_request::request& request_struct);
        std::string putRequest(const http_request::request& request_struct);
        json getJsonFromFile();
    };
}
#endif //WEBSERVER_RESOURCE_MANAGER_H
