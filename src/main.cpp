#include <iostream>

#include "parsers/http_request.h"
#include "parsers/http_response.h"
#include "services/resource_manager.h"
#include "sockets/server_manager.h"
#include "sockets/web_socket_exception.h"
#include "utils/logger.h"

int main() {
    //std::string req = "POST / HTTP/3\r\nHost: 127.0.0.1:6702\r\nUser-Agent: curl/8.21.0\r\nAccept: */*\r\n\r\nWitaj swiecie co u ciebie?";
    // ServerManager server_manager;
    // server_manager.startServer();

    //std::string json_string = "[{\"id\": 1,\"username\": \"jacob123\",\"password\": \"1234\",\"age\": 19},{\"id\": 2,\"username\": \"nicole52\",\"password\": \"hardpassword12\",\"age\": 25}]";
    std::map<std::string, std::string> param;
    param["id"] = "1";
    std::map<std::string, std::string> headers;
    std::string body = "{\"id\":3,\"username\":\"adam52\",\"password\":\"helloworld\",\"age\":35}";
    // param["age"] = "25";
    http_request::request request_struct{http_request::Delete,"/users.json",param,"1.1",headers};
    resource_manager::UsersRepository users_repository;
    std::cout << users_repository.handleRequest(request_struct);
    // std::cout<<resource_manager::(request_struct);
    // std::string req = "GET /hello.html HTTP/1.1\r\nHost: 127.0.0.1:6702\r\nUser-Agent: curl/8.21.0\r\nAccept: */*\r\n\r\n";
    // http_request::request request_struct = http_request::parseRequest(req);
    // http_response::response response_struct = http_response::createResponse(request_struct);
    // std::cout << response_struct.toString();
    return 0;
}
