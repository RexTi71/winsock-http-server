//
// Created by Dom on 6.11.2026.
//

#include "http_response.h"
#include "../services/resource_manager.h"

#define HTTP_VER "1.1"
namespace http_response {
    std::string getStatusCodeName(const StatusCode& status_code) {
        switch (status_code) {
            case Ok: return "OK";
            case Accepted: return "ACCEPTED";
            case Bad_Request: return "BAD REQUEST";
            case Unathorized: return "UNATHORIZED";
            case Forbidden: return "FORBIDDEN";
            case Not_Found: return "NOT FOUND";
            default: return "";
        }
    }
    std::string response::toString() {
        std::string result = std::format("HTTP/{} {} {}\r\n",this->version,
            static_cast<int>(this->status_code),
            getStatusCodeName(this->status_code));

        if (this->headers.empty()) {
            result += "\r\n" + this->body;
            return result;
        }

        std::string headers = "";
        for (auto header : this->headers) {
            headers += header.first + ": " + header.second + "\r\n";
        }
        result += headers + "\r\n" + this->body;
        return result;
    }

    response createResponse(const http_request::request& request_struct, resource_manager::Repository& repository) {
        response response_struct;
        std::map<std::string, std::string> headers;
        response_struct.version = HTTP_VER;

        resource_manager::result request_result = repository.handleRequest(request_struct);

        response_struct.body = request_result.body;
        response_struct.status_code = request_result.status_code;

        if (response_struct.body[0] == '[' || response_struct.body[0] == '{') {
            headers["Content-Type"] = "application/json";
        }else if (response_struct.body[0] == '<') {
            headers["Content-Type"] = "text/html; charset=UTF-8";
        }else {
            headers["Content-Type"] = "text/plain; charset=UTF-8";
        }

        headers["Content-Length"] = std::to_string(response_struct.body.size());
        response_struct.headers = headers;
        return response_struct;
    }
}
