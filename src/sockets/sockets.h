//
// Created by Dom on 6.11.2026.
//

#ifndef WEBSERVER_SOCKETS_H
#define WEBSERVER_SOCKETS_H
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

class SocketManager {
public:
    SocketManager();
    ~SocketManager();
    void createSocket();
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
