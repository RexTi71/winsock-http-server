//
// Created by Dom on 6.11.2026.
//

#include "http_request.h"
namespace {
    std::map<std::string,std::string> parseHeaders(std::string& request) {

        std::map<std::string,std::string> headers;
        const std::string ENDLINE_PATTERN("\r\n");
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
}
namespace http_request {

void parseRequest(std::string& request) {

    //GET / HTTP/1.1
    //Host: 127.0.0.1:6702
    //User-Agent: curl/8.21.0
    //Accept: */*

    //TODO: zapobieganie blednym requestem, nie zawsze dostane to co chce
    http_request::request request_struct;

    const int FIRST_SPACE = request.find(' ');
    request_struct.method = request.substr(0, FIRST_SPACE);

    const int SECOND_SPACE = request.find(' ', FIRST_SPACE + 1);
    request_struct.url = request.substr(FIRST_SPACE + 1, SECOND_SPACE - FIRST_SPACE - 1);

    const int VERSION_INDEX = request.find_last_of("HTTP/") + 1;
    constexpr unsigned __int8 VERSION_LENGTH = 3;
    request_struct.version = request.substr(VERSION_INDEX,VERSION_LENGTH);

    request_struct.headers = parseHeaders(request);
    std::string response;
    // if (method == "GET") {
    //     std::string file_path = ".." + url;
    //     std::ifstream file(file_path, std::ios::binary | std::ios::ate);
    //     if (file) {
    //         std::streamsize file_size = file.tellg();
    //         file.seekg(0, std::ios::beg);
    //
    //         std::string body(file_size, '\0');
    //         file.read(body.data(), file_size);
    //
    //
    //         response = std::format("HTTP/1.1 200 OK\r\nContent-Length:{}\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n",(long)file_size);
    //         response += body;
    //     }else {
    //         response = "HTTP/1.1 404 NOT FOUND\r\nContent-Type: text/plain\r\n\r\nResource not found\r\n\r\n";
    //     }
    //     printf("Sent back: %s",response.c_str());
    // }
}
}
