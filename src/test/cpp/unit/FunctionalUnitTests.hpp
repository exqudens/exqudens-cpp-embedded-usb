#pragma once

#include <string>
#include <exception>
#include <algorithm>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <exqudens/Log.hpp>

#include "TestUtils.hpp"
#include "exqudens/embedded/usb/Application.hpp"

class FunctionalUnitTests: public testing::Test {

    public:

        inline static const char* LOGGER_ID = "FunctionalUnitTests";

};

TEST_F(FunctionalUnitTests, test1) {
    try {
        std::string testGroup = testing::UnitTest::GetInstance()->current_test_info()->test_suite_name();
        std::string testCase = testing::UnitTest::GetInstance()->current_test_info()->name();
        EXQUDENS_LOG_INFO(LOGGER_ID) << "bgn";

        std::string expected = "ABC123";
        EXQUDENS_LOG_INFO(LOGGER_ID) << "expected: '" << expected << "'";
        std::string input = "abc123";
        EXQUDENS_LOG_INFO(LOGGER_ID) << "input: '" << input << "'";
        std::array<uint8_t, 1024> buffer = {};
        std::copy(input.begin(), input.end(), buffer.data());
        exqudens::Application::usbCallback(buffer, (uint32_t) input.size());
        std::string actual = std::string(buffer.begin(), buffer.end()).c_str();
        EXQUDENS_LOG_INFO(LOGGER_ID) << "actual: '" << actual << "'";

        ASSERT_EQ(expected, actual);

        EXQUDENS_LOG_INFO(LOGGER_ID) << "end";
    } catch (const std::exception& e) {
        std::string errorMessage = TestUtils::toString(e);
        std::cout << LOGGER_ID << " ERROR: " << errorMessage << std::endl;
        FAIL() << errorMessage;
    }
}
