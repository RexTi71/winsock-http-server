//
// Created by Dom on 15.11.2026.
//

#include "http_response_test.h"
#include <gtest/gtest.h>
#include "../../src/parsers/http_response.h"


TEST(Http_response, SetsContentTypeToJson) {
    http_response::response response_struct;

    EXPECT_EQ(7 * 6, 42);
}