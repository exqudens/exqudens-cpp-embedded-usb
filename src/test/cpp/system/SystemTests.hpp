#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <exqudens/Log.hpp>

#include "TestUtils.hpp"
#include "exqudens/usb/ClientFactory.hpp"

class SystemTests: public testing::Test {

    public:

        inline static const char* LOGGER_ID = "SystemTests";

};

TEST_F(SystemTests, test1) {
    try {
        std::string testGroup = testing::UnitTest::GetInstance()->current_test_info()->test_suite_name();
        std::string testCase = testing::UnitTest::GetInstance()->current_test_info()->name();
        EXQUDENS_LOG_INFO(LOGGER_ID) << "bgn";

        std::shared_ptr<exqudens::usb::IClient> client = exqudens::usb::ClientFactory::createShared();
        EXQUDENS_LOG_INFO(LOGGER_ID) << "client.initialized: " << (bool) client;

        ASSERT_TRUE((bool) client);

        std::vector<std::map<std::string, unsigned short>> devices = client->listDevices();
        EXQUDENS_LOG_INFO(LOGGER_ID) << "devices.size: " << devices.size();

        ASSERT_FALSE(devices.empty());

        std::map<std::string, unsigned short> device = {};
        for (const auto& d : devices) {
            if (d.at("vendor") == 0x0484 && d.at("product") == 0x5741) {
                device = d;
                break;
            }
        }
        EXQUDENS_LOG_INFO(LOGGER_ID) << "device.size: " << device.size();

        ASSERT_FALSE(device.empty());

        client->open(device);
        EXQUDENS_LOG_INFO(LOGGER_ID) << "client.isOpen: " << client->isOpen();

        ASSERT_TRUE(client->isOpen());

        // exchange-bgn
        std::string userInputString = TestUtils::getUserInput("string").value_or("hello");
        EXQUDENS_LOG_INFO(LOGGER_ID) << "userInputString: '" << userInputString << "'";

        std::vector<unsigned char> buffer = std::vector<unsigned char>(userInputString.begin(), userInputString.end());
        EXQUDENS_LOG_INFO(LOGGER_ID) << "buffer.size: " << buffer.size();

        ASSERT_EQ(userInputString.size(), buffer.size());

        size_t exchanged = client->bulkWrite(buffer, 1, 500);
        EXQUDENS_LOG_INFO(LOGGER_ID) << "exchanged: " << exchanged;

        ASSERT_EQ(userInputString.size(), exchanged);

        buffer = client->bulkRead(1, 500, 1024);
        EXQUDENS_LOG_INFO(LOGGER_ID) << "buffer.size: " << buffer.size();

        ASSERT_EQ(userInputString.size(), buffer.size());

        std::string actual = std::string(buffer.begin(), buffer.end()).c_str();
        EXQUDENS_LOG_INFO(LOGGER_ID) << "actual: '" << actual << "'";

        std::string expected = TestUtils::toUpper(userInputString);
        EXQUDENS_LOG_INFO(LOGGER_ID) << "expected: '" << expected << "'";

        ASSERT_EQ(expected, actual);
        // exchange-end

        client->close();
        EXQUDENS_LOG_INFO(LOGGER_ID) << "client.isOpen: " << client->isOpen();

        ASSERT_FALSE(client->isOpen());

        EXQUDENS_LOG_INFO(LOGGER_ID) << "end";
    } catch (const std::exception& e) {
        std::string errorMessage = TestUtils::toString(e);
        std::cout << LOGGER_ID << " ERROR: " << errorMessage << std::endl;
        FAIL() << errorMessage;
    }
}
