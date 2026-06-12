#include <cstring>

#include "exqudens/embedded/usb/util/UsbUtils.hpp"

namespace exqudens {

    bool UsbUtils::getGreen() {
        return green;
    }

    bool UsbUtils::getYellow() {
        return yellow;
    }

    bool UsbUtils::getRed() {
        return red;
    }

    uint32_t UsbUtils::usbCallback(std::array<uint8_t, 1024>& buffer, uint32_t size) {
        std::array<char, 1024> str = {};
        std::memcpy(str.data(), buffer.data(), size);

        if (std::strcmp("green_0", str.data()) == 0) {
            green = false;
        } else if (std::strcmp("green_1", str.data()) == 0) {
            green = true;
        } else if (std::strcmp("yellow_0", str.data()) == 0) {
            yellow = false;
        } else if (std::strcmp("yellow_1", str.data()) == 0) {
            yellow = true;
        } else if (std::strcmp("red_0", str.data()) == 0) {
            red = false;
        } else if (std::strcmp("red_1", str.data()) == 0) {
            red = true;
        }

        for (uint32_t i = 0; i < size; i++) {
            if (buffer.at(i) >= 'a' && buffer.at(i) <= 'z') {
                buffer.at(i) -= ('a' - 'A');
            }
        }
        return size;
    }

}
