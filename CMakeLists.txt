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

file(GLOB_RECURSE SOURCES src/*.cpp)
list(FILTER SOURCES EXCLUDE REGEX "main\\.cpp$")

add_library(core ${SOURCES})
target_link_libraries(core PUBLIC nlohmann_json::nlohmann_json)
if(WIN32)
    target_link_libraries(core PUBLIC ws2_32)
endif()

add_executable(webserver src/main.cpp)
target_link_libraries(webserver PRIVATE core)

enable_testing()
add_executable(tests tests/parsers/http_response_test.cpp)
target_link_libraries(tests PRIVATE core GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)