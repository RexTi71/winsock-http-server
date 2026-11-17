//
// Created by Dom on 6.11.2026.
//

#ifndef WEBSERVER_SOCKETS_H
#define WEBSERVER_SOCKETS_H
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "web_socket_exception.h"

/// Handles socket creation, http requests and responses
class ServerManager {
public:
    ServerManager();
    ~ServerManager();


    /**Creates, binds sockets, listens for any requests and responds
     * @throws WebSocketException if any of creation steps fails
    */
    void startServer();
private:
    int error_code;
    WSAData wsa_data;
    SOCKET listen_socket = INVALID_SOCKET;
    void bindSocket(addrinfo* result);
    void acceptingConnection(SOCKET& client_socket);
    void listenOnIpAddress();
    void throwWsaMessage(std::string message);
    void receiveRequest(SOCKET& client_socket);
    void closeConnection(SOCKET& client_socket);
};

#endif //WEBSERVER_SOCKETS_H
