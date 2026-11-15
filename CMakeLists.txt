cmake_minimum_required(VERSION 4.3)
project(webserver)

include(FetchContent)
set(CMAKE_CXX_STANDARD 20)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

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

enable_testing()
add_executable(tests tests/parsers/http_response_test.cpp)
target_link_libraries(tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)