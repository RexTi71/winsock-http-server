//
// Created by Dom on 6.11.2026.
//

#include "sockets.h"

#include <format>
#include <cstdio>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "web_socket_exception.h"

#define PORT "6702"
#define BUFFER_LEN 1024

SocketManager::SocketManager() {
     error_code = WSAStartup(MAKEWORD(2,2),&wsa_data);
     if (error_code != 0) {
          throw WebSocketException(std::format("Could Not Start Socket, error code: {}", error_code));
     }
}
SocketManager::~SocketManager() {
     WSACleanup();
}

void SocketManager::createSocket() {
     //Tworzenie informacji o socket
     addrinfo* result = nullptr;
     addrinfo hints{};
     hints.ai_flags = AI_PASSIVE;
     hints.ai_family = AF_INET;
     hints.ai_socktype = SOCK_STREAM;
     hints.ai_protocol = IPPROTO_TCP;

     error_code = getaddrinfo(nullptr, PORT, &hints, &result);
     if (error_code != 0) {
          throw WebSocketException(std::format("Couldn't translate address info, error code: {}", error_code));
     }

     listen_socket = socket(result->ai_family,
          result->ai_socktype,
          result->ai_protocol);
     if (listen_socket == INVALID_SOCKET) {
          freeaddrinfo(result);
          throwWsaMessage("Couldn't create listen socket on given parameters");
     }
     bindSocket(result);
     listenOnIpAddress();

     SOCKET client_socket = INVALID_SOCKET;
     acceptingConnection(client_socket);

     char rec_buf[BUFFER_LEN];
     do{
          error_code = recv(client_socket, rec_buf, BUFFER_LEN, 0);
          printf("Received: %s",rec_buf);
          error_code = send(client_socket, rec_buf,error_code, 0);
          printf("Sent back: %s", rec_buf);
     }while (error_code > 0);

     closesocket(client_socket);
}

void SocketManager::bindSocket(addrinfo* result) {
     error_code = bind(listen_socket, result->ai_addr, static_cast<int>(result->ai_addrlen));
     if (error_code == SOCKET_ERROR) {
          freeaddrinfo(result);
          throwWsaMessage("Couldn't bind socket to address");
     }
     freeaddrinfo(result);
}

void SocketManager::listenOnIpAddress() {
     if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR) {
          throwWsaMessage("Couldn't listen on server socket");
     }
}

void SocketManager::acceptingConnection(SOCKET& client_socket) {

     client_socket = accept(listen_socket, nullptr, nullptr);

     if (client_socket == INVALID_SOCKET) {
          throwWsaMessage("Couldn't setup connection handling on socket");
     }
     closesocket(listen_socket);
}

void SocketManager::throwWsaMessage(std::string message){
     int wsa_err = WSAGetLastError();
     throw WebSocketException(std::format("{},error code: {}",message, wsa_err));
}

