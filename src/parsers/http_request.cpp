//
// Created by Dom on 6.11.2026.
//

#include "http_request.h"

#include <stdexcept>

namespace {
    const std::string ENDLINE_PATTERN("\r\n");
    std::map<std::string,std::string> parseHeaders(std::string& request) {

        std::map<std::string,std::string> headers;
        int line_end_index = request.find(ENDLINE_PATTERN);
        int semicolomn_index = -1;

        do{
            semicolomn_index = request.find(':',line_end_index);

            if (semicolomn_index == request.npos) {
                break;
            }

            std::string header_name = request.substr(line_end_index + ENDLINE_PATTERN.length(),
                semicolomn_index - (line_end_index + ENDLINE_PATTERN.length()));
            line_end_index = request.find(ENDLINE_PATTERN, semicolomn_index);
            //Uwzgledniamy spacje przy headerach Host: 127.0.0.1...
            std::string header_value = request.substr(semicolomn_index + 2, line_end_index - (semicolomn_index + 2));
            headers[header_name] = header_value;
        }while(semicolomn_index != request.npos);
        return headers;
    }
    std::string parseBody(std::string& request) {
        const unsigned __int8 BODY_START_INDEX = request.find_last_of(ENDLINE_PATTERN) + ENDLINE_PATTERN.length() - 1;
        return request.substr(BODY_START_INDEX);
    }
}
namespace http_request {
    std::string getMethodName(const Method& method) {
        switch (method) {
            case Get: return "GET";
            case Post: return "POST";
            case Delete: return "DELETE";
            case Put: return "PUT";
            case Patch: return "PATCH";
            default: return "";
        }
    }

    Method convertStrToMethodName(const std::string& input) {

        if (input == "GET") {
            return Get;
        }if (input == "POST") {
            return Post;
        }if (input == "DELETE") {
            return Delete;
        }if (input == "PUT") {
            return Put;
        }if (input == "PATCH") {
            return Patch;
        }
        throw std::runtime_error("Failed to convert string");

    }
    request parseRequest(std::string& request) {


        //TODO: zapobieganie blednym requestem, nie zawsze dostane to co chce
        http_request::request request_struct;

        const int FIRST_SPACE = request.find(' ');
        request_struct.method = convertStrToMethodName(request.substr(0, FIRST_SPACE));

        const int SECOND_SPACE = request.find(' ', FIRST_SPACE + 1);
        request_struct.url = request.substr(FIRST_SPACE + 1, SECOND_SPACE - FIRST_SPACE - 1);

        const int VERSION_INDEX = request.find("HTTP/") + 5;
        constexpr unsigned __int8 VERSION_LENGTH = 3;
        request_struct.version = request.substr(VERSION_INDEX,VERSION_LENGTH);

        request_struct.headers = parseHeaders(request);

        if (request_struct.method == Post) {
            request_struct.body = parseBody(request);
        }
        return request_struct;
    }
}


