#pragma once

#include <string>
#include <optional>
#include <vector>
#include <map>
#include <exception>

#include "test_lib_export.hpp"

class TEST_LIB_EXPORT TestUtils {

    private:

        inline static std::map<std::string, std::string> userInput = {};

    public:

        static void init(const std::vector<std::string>& value);

        static std::optional<std::string> getUserInput(const std::string& key);

        static std::vector<std::string> toStringVector(const std::exception& value, std::vector<std::string> previous = {});

        static std::vector<std::string> toStackTrace(const std::exception& value);

        static std::string toString(const std::exception& value);

        static std::string toUpper(const std::string& value);

        static std::string toLower(const std::string& value);

};
