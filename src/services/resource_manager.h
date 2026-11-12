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
    constexpr std::string FILE_PATH_ROOT = "../resources";
    std::string getRequest(http_request::request& request_struct);
    std::string postRequest(http_request::request& request_struct);
    std::string deleteRequest(http_request::request& request_struct);
    std::string patchRequest(http_request::request& request_struct);
    std::string putRequest(http_request::request& request_struct);
}
#endif //WEBSERVER_RESOURCE_MANAGER_H
