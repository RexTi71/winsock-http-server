//
// Created by Dom on 17.11.2026.
//

#include "resource_manager_test.h"
#include "../../src/services/resource_manager.h"

#include <gtest/gtest.h>
namespace {

class FakeFileManager: public resource_manager::FileManager {
public:
    bool should_throw = false;
    void setFilePath(const std::string& file_path) override {};

    resource_manager::result getRequest(const http_request::request& request_struct)override {
        throwException();
        return resource_manager::result{http_response::Ok,"Get"};
    }
    resource_manager::result postRequest(const http_request::request& request_struct) override {
        throwException();
        return resource_manager::result{http_response::Created,"Post"};
    }
    resource_manager::result deleteRequest(const http_request::request& request_struct) override {
        throwException();
        return resource_manager::result{http_response::Ok,"Delete"};
    }
    resource_manager::result putRequest(const http_request::request& request_struct) override {
        throwException();
        return resource_manager::result{http_response::Ok,"Put"};
    }
    resource_manager::result patchRequest(const http_request::request& request_struct) override {
        throwException();
        return resource_manager::result{http_response::Ok,"Patch"};
    }
private:
    void throwException() const{
        if (should_throw) {
            throw std::runtime_error("Error");
        }
    }
};
struct  ResultParam {
    http_request::Method method;
    resource_manager::result expected;
};
class UserRepositoryTest : public ::testing::Test {
protected:
    FakeFileManager file_manager;
    resource_manager::UsersRepository users_repository{file_manager};
};
class UserRepositoryFixture: public testing::TestWithParam<ResultParam> {
protected:
    FakeFileManager file_manager;
    resource_manager::UsersRepository users_repository{file_manager};
};
}
TEST_P(UserRepositoryFixture, ReturnsResultFromHandler) {
    auto params = GetParam();
    const http_request::request request_struct{.method = params.method};
    const resource_manager::result actual_result = this->users_repository.handleRequest(request_struct);
    EXPECT_EQ(actual_result.status_code, params.expected.status_code);
    EXPECT_EQ(actual_result.body, params.expected.body);
}

INSTANTIATE_TEST_SUITE_P(Methods, UserRepositoryFixture, testing::Values(
ResultParam{http_request::Method::Get, {http_response::Ok, "Get"}},
ResultParam{http_request::Post, {http_response::Created, "Post"}},
ResultParam{http_request::Delete,{http_response::Ok, "Delete"}},
ResultParam{http_request::Put,{http_response::Ok, "Put"}},
ResultParam{http_request::Patch, {http_response::Ok, "Patch"}},
ResultParam{http_request::Unknown, {http_response::Not_Found, "Unknown Method"}}
),
[](const testing::TestParamInfo<ResultParam>& info) {
    std::string name = info.param.expected.body;
    //TestParamInfo doesn't accept whitespace
    std::erase(name, ' ');
    return name;
    }
);

TEST_F(UserRepositoryTest, ReturnsInternalErrorOnException) {
    this->file_manager.should_throw = true;
    http_request::request request_struct;
    request_struct.method = http_request::Get;
    const resource_manager::result expected_result{.status_code = http_response::Internal_Server_Error, .body = "Internal error"};
    resource_manager::result actual_result = this->users_repository.handleRequest(request_struct);

    EXPECT_EQ(actual_result.status_code, expected_result.status_code);
    EXPECT_EQ(actual_result.body, expected_result.body);
}