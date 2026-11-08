#include <iostream>

#include "parsers/http_request.h"
#include "parsers/http_response.h"
#include "sockets/sockets.h"
#include "sockets/web_socket_exception.h"
#include "utils/logger.h"

int main() {
    //Na razie jest to zebym mogl zakonczyc polaczenie bez problemow
    //Ale jak bedzie http dodane to usunac to
    // try {
    //     SocketManager socket_manager;
    //     socket_manager.createSocket();
    // }catch (const WebSocketException& ex) {
    //     std::cerr << ex.what() << "\n";
    //     return 1;
    // }
    //std::string req = "POST / HTTP/3\r\nHost: 127.0.0.1:6702\r\nUser-Agent: curl/8.21.0\r\nAccept: */*\r\n\r\nWitaj swiecie co u ciebie?";
    std::string req = "GET /hello.html HTTP/1.1\r\nHost: 127.0.0.1:6702\r\nUser-Agent: curl/8.21.0\r\nAccept: */*\r\n\r\n";
    http_request::request request_struct = http_request::parseRequest(req);
    http_response::response response_struct = http_response::createResponse(request_struct);
    std::cout << response_struct.toString();
    return 0;
}
