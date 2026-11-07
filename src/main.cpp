#include <iostream>

#include "parsers/http_request.h"
#include "sockets/sockets.h"
#include "sockets/web_socket_exception.h"

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
    std::string req = "GET / HTTP/1.1\r\nHost: 127.0.0.1:6702\r\nUser-Agent: curl/8.21.0\r\nAccept: */*\r\n\r\n";
    http_request::parseRequest(req);
    return 0;
}
