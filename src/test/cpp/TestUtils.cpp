#include <cstddef>
#include <cctype>
#include <stdexcept>
#include <filesystem>
#include <algorithm>
#include <sstream>
#include <fstream>

#include "TestUtils.hpp"

#define CALL_INFO std::string(__FUNCTION__) + "(" + std::filesystem::path(__FILE__).filename().string() + ":" + std::to_string(__LINE__) + ")"

void TestUtils::init(const std::vector<std::string>& value) {
    try {
        std::vector<std::string> keys = {
            "string"
        };
        for (size_t i = 1; i < value.size(); i++) {
            for (size_t j = 0; j < keys.size(); j++) {
                std::string prefix = std::string("--user-input-") + keys.at(j) + "=";
                if (value.at(i).starts_with(prefix)) {
                    std::string v = value.at(i).substr(prefix.size());
                    userInput[keys.at(j)] = v;
                    break;
                }
            }
        }
    } catch (...) {
        std::throw_with_nested(std::runtime_error(CALL_INFO));
    }
}

std::optional<std::string> TestUtils::getUserInput(const std::string& key) {
    try {
        std::optional<std::string> result = {};
        if (userInput.contains(key)) {
            result.emplace(userInput.at(key));
        }
        return result;
    } catch (...) {
        std::throw_with_nested(std::runtime_error(CALL_INFO));
    }
}

std::vector<std::string> TestUtils::toStringVector(
        const std::exception& value,
        std::vector<std::string> previous
) {
    previous.emplace_back(value.what());
    try {
        std::rethrow_if_nested(value);
        return previous;
    } catch (const std::exception& e) {
        return toStringVector(e, previous);
    } catch (...) {
        if (previous.empty()) {
            previous.emplace_back(CALL_INFO + ": Empty stack!");
        }
        return previous;
    }
}

std::vector<std::string> TestUtils::toStackTrace(const std::exception& value) {
    try {
        std::vector<std::string> elements = toStringVector(value);
        if (elements.size() > 1) {
            std::ranges::reverse(elements);
        }
        return elements;
    } catch (...) {
        std::throw_with_nested(std::runtime_error(CALL_INFO));
    }
}

std::string TestUtils::toString(const std::exception& value) {
    try {
        std::vector<std::string> stackTrace = toStackTrace(value);
        std::ostringstream out;
        for (size_t i = 0; i < stackTrace.size(); i++) {
            out << stackTrace[i];
            if (i < stackTrace.size() - 1) {
                out << std::endl;
            }
        }
        return out.str();
    } catch (...) {
        std::throw_with_nested(std::runtime_error(CALL_INFO));
    }
}

std::string TestUtils::toUpper(const std::string& value) {
    try {
        std::string str = value;
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::toupper(c); });
        return str;
    } catch (...) {
        std::throw_with_nested(std::runtime_error(CALL_INFO));
    }
}

std::string TestUtils::toLower(const std::string& value) {
    try {
        std::string str = value;
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
        return str;
    } catch (...) {
        std::throw_with_nested(std::runtime_error(CALL_INFO));
    }
}

#undef CALL_INFO
