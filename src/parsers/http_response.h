//
// Created by Dom on 6.11.2026.
//

#ifndef WEBSERVER_HTTP_RESPONSE_H
#define WEBSERVER_HTTP_RESPONSE_H
#include <string>
#include <fstream>
#include <format>
#include "http_request.h"

namespace resource_manager {
    class IRepository;
}

namespace http_response {
    enum StatusCode {
        Ok = 200,
        Created = 201,
        Accepted = 202,
        Bad_Request = 400,
        Unathorized = 401,
        Forbidden = 403,
        Not_Found = 404,
        Internal_Server_Error = 500
    };
    /// converts enums name to a string
    std::string getStatusCodeName(const StatusCode& status_code);
    struct response {
        std::string version;
        StatusCode status_code;
        std::map<std::string, std::string> headers;
        std::string body;

        std::string toString();
    };

    /**
     * @brief Handles creation of a viable Http response with default headers
     * @param request_struct Parsed request (method, url, query, version...)
     * @param repository Context for repository handler
     * @return response struct containing(version, status code, headers and body)
     */
    response createResponse(const http_request::request& request_struct, resource_manager::IRepository& repository);

}
#endif //WEBSERVER_HTTP_RESPONSE_H
