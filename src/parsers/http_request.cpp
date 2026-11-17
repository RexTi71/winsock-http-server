//
// Created by Dom on 6.11.2026.
//

#include "http_request.h"

#include <stdexcept>

namespace {
    void extractParameters(const size_t parameters_start, const std::string& request,
    std::map<std::string,std::string>& parameters) {
        size_t pos = parameters_start + 1;

        while (pos < request.size()) {
            const size_t equal = request.find('=', pos);
            if (equal == std::string::npos) break;

            const size_t amp = request.find('&', equal);
            const size_t end = (amp == std::string::npos) ? request.size() : amp;

            std::string name  = request.substr(pos, equal - pos);
            std::string value = request.substr(equal + 1, end - (equal + 1));
            parameters[name] = value;

            if (amp == std::string::npos) break;
            pos = amp + 1;
        }
    }

    constexpr std::string ENDLINE_PATTERN("\r\n");
    std::map<std::string,std::string> parseHeaders(std::string& request) {

        //Result variable initialization
        std::map<std::string,std::string> headers;
        size_t line_end_index = request.find(ENDLINE_PATTERN);
        size_t semicolomn_index = -1;

        //Iteration over headers
        do{
            semicolomn_index = request.find(':',line_end_index);

            if (semicolomn_index == std::string::npos) {
                break;
            }

            std::string header_name = request.substr(line_end_index + ENDLINE_PATTERN.length(),
                semicolomn_index - (line_end_index + ENDLINE_PATTERN.length()));
            line_end_index = request.find(ENDLINE_PATTERN, semicolomn_index);

            //We take notice of whitespace on header Host: 127.0.0.1...
            const std::string header_value = request.substr(semicolomn_index + 2, line_end_index - (semicolomn_index + 2));
            headers[header_name] = header_value;
        }while(semicolomn_index != std::string::npos);
        return headers;
    }
    std::string parseBody(const std::string& request) {
        const size_t HEADERS_END = request.find("\r\n\r\n");
        if (HEADERS_END == std::string::npos) return "";
        return request.substr(HEADERS_END + 4);
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

        http_request::request request_struct;

        //Method
        const size_t FIRST_SPACE = request.find(' ');
        request_struct.method = convertStrToMethodName(request.substr(0, FIRST_SPACE));

        //Query parameters
        const size_t SECOND_SPACE = request.find(' ', FIRST_SPACE + 1);
        std::string target = request.substr(FIRST_SPACE + 1, SECOND_SPACE - FIRST_SPACE - 1);

        size_t query_start = target.find('?');
        if (query_start != std::string::npos) {
            extractParameters(query_start, target, request_struct.parameters);
            target.erase(query_start);
        }

        //Url of request
        request_struct.url = target;

        //Http version
        const size_t VERSION_INDEX = request.find("HTTP/") + 5;
        constexpr unsigned __int8 VERSION_LENGTH = 3;
        request_struct.version = request.substr(VERSION_INDEX,VERSION_LENGTH);

        request_struct.headers = parseHeaders(request);

        if (request_struct.method == Post ||
            request_struct.method == Put ||
            request_struct.method == Patch) {
            request_struct.body = parseBody(request);
            }
        return request_struct;
    }
}


