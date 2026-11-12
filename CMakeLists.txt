cmake_minimum_required(VERSION 4.3)
project(webserver)

include(FetchContent)
set(CMAKE_CXX_STANDARD 20)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(webserver src/main.cpp
        src/sockets/server_manager.cpp
        src/sockets/web_socket_exception.cpp
        src/parsers/http_request.cpp
        src/parsers/http_response.cpp
        src/utils/logger.cpp
        src/services/resource_manager.cpp
)
target_link_libraries(webserver PRIVATE ws2_32)
target_link_libraries(webserver PRIVATE nlohmann_json::nlohmann_json)
