//
// Created by Dom on 6.11.2026.
//

#include "server_manager.h"

#include <format>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "web_socket_exception.h"
#include "../parsers/http_request.h"
#include "../parsers/http_response.h"
#include "../utils/logger.h"

#define PORT "6702"
#define BUFFER_LEN 4096

ServerManager::ServerManager() {
     error_code = WSAStartup(MAKEWORD(2,2),&wsa_data);
     if (error_code != 0) {
          throw WebSocketException(std::format("Could not start socket, error code: {}", error_code));
     }
}
ServerManager::~ServerManager() {
     WSACleanup();
}

void ServerManager::startServer() {
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

     closeConnection(client_socket);
     closesocket(client_socket);
}

void ServerManager::bindSocket(addrinfo* result) {
     error_code = bind(listen_socket, result->ai_addr, static_cast<int>(result->ai_addrlen));
     if (error_code == SOCKET_ERROR) {
          freeaddrinfo(result);
          throwWsaMessage("Couldn't bind socket to address");
     }
     freeaddrinfo(result);
}

void ServerManager::listenOnIpAddress() {
     if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR) {
          throwWsaMessage("Couldn't listen on server socket");
     }
     logger::info(std::format("Server is listening on port: {}", PORT));
}

void ServerManager::acceptingConnection(SOCKET& client_socket) {
     sockaddr_in connecting_address;
     socklen_t connecting_adress_len = sizeof(connecting_address);
     while (true) {
          client_socket = accept(listen_socket, (sockaddr*) &connecting_address, &connecting_adress_len);

          if (client_socket == INVALID_SOCKET) {
               logger::warn("Couldn't setup connection handling on socket");
          }
          logger::info(std::format("Received connection from address: {}", inet_ntoa(connecting_address.sin_addr)));
          receiveRequest(client_socket);
          closesocket(client_socket);
     }
}

void ServerManager::throwWsaMessage(std::string message){
     int wsa_err = WSAGetLastError();
     logger::error(message + std::to_string(wsa_err));
     throw WebSocketException(std::format("{}, error code: {}",message, wsa_err));
}

void ServerManager::receiveRequest(SOCKET& client_socket) {
     char rec_buf[BUFFER_LEN] = {};

     do{
          error_code = recv(client_socket, rec_buf, BUFFER_LEN, 0);

          if (error_code == SOCKET_ERROR) {
               closesocket(client_socket);
               throwWsaMessage("Couldn't receive client request");
          }else if (error_code == 0) {
               logger::info("Connection closed successfully!");
               break;
          }

          logger::info(std::format("Received:\n{}",rec_buf));
          std::string request = rec_buf;

          http_request::request request_struct = http_request::parseRequest(request);
          http_response::response response_struct = http_response::createResponse(request_struct);

          std::string response_string = response_struct.toString();
          logger::info("Sent back:\n" + response_string);
          error_code = send(client_socket, response_string.data(),static_cast<int>(response_string.size()), 0);

          if (error_code == SOCKET_ERROR) {
               closesocket(client_socket);
               throwWsaMessage("Couldn't send response to client");
          }
     }while (error_code > 0);
}

void ServerManager::closeConnection(SOCKET& client_socket) {
     error_code = shutdown(client_socket, SD_SEND);
     if (error_code == SOCKET_ERROR) {
          closesocket(client_socket);
          throwWsaMessage("Couldn't close connection gracefully");
     }
}

