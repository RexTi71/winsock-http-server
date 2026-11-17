//
// Created by Dom on 15.11.2026.
//

#include <gtest/gtest.h>
#include "../../src/parsers/http_response.h"
#include "../../src/services/resource_manager.h"
namespace {

class FakeRepository : public resource_manager::IRepository {
public:
    resource_manager::result next;
    resource_manager::result handleRequest(const http_request::request&) override {
        return next;
    }
};

class ContentTypeTest
    : public testing::TestWithParam<std::pair<std::string, std::string>> {};

}


TEST_P(ContentTypeTest, PicksContentTypeFromBody) {
    FakeRepository repo;
    repo.next = { .status_code = http_response::Ok, .body = GetParam().first };

    const auto res = http_response::createResponse({}, repo);

    EXPECT_EQ(res.headers.at("Content-Type"), GetParam().second);
}

INSTANTIATE_TEST_SUITE_P(Bodies, ContentTypeTest, testing::Values(
    std::pair<std::string, std::string>{"[",  "application/json"},
    std::pair<std::string, std::string>{"{",  "application/json"},
    std::pair<std::string, std::string>{"<",  "text/html; charset=UTF-8"},
    std::pair<std::string, std::string>{" ",  "text/plain; charset=UTF-8"},
    std::pair<std::string, std::string>{"",   "text/plain; charset=UTF-8"}
));


TEST(CreateResponse, CopiesStatusCodeFromRepository) {
    FakeRepository repo;
    repo.next = { .status_code = http_response::Not_Found, .body = "x" };

    const auto res = http_response::createResponse({}, repo);

    EXPECT_EQ(res.status_code, http_response::Not_Found);
}

TEST(CreateResponse, CopiesBodyFromRepository) {
    FakeRepository repo;
    repo.next = { .status_code = http_response::Ok, .body = "hello" };

    const auto res = http_response::createResponse({}, repo);

    EXPECT_EQ(res.body, "hello");
}

TEST(CreateResponse, SetsContentLengthToBodySize) {
    FakeRepository repo;
    repo.next = { .status_code = http_response::Ok, .body = "hello" };

    const auto res = http_response::createResponse({}, repo);

    EXPECT_EQ(res.headers.at("Content-Length"), "5");
}

TEST(CreateResponse, SetsHttpVersion) {
    FakeRepository repo;

    const auto res = http_response::createResponse({}, repo);

    EXPECT_EQ(res.version, "1.1");
}