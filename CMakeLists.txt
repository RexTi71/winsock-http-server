cmake_minimum_required(VERSION 4.3)
project(webserver)

set(CMAKE_CXX_STANDARD 20)


add_executable(webserver src/main.cpp
        src/sockets/sockets.cpp
        src/sockets/web_socket_exception.cpp
        src/parsers/http_request.cpp
        src/parsers/http_response.cpp
)
target_link_libraries(webserver PRIVATE ws2_32)
