#pragma once

#include <cstdint>
#include <array>

#include "exqudens/embedded/usb/export.hpp"

namespace exqudens {

    class EXQUDENS_EMBEDDED_USB_EXPORT UsbUtils {

        private:

            inline static bool green = false;
            inline static bool yellow = false;
            inline static bool red = false;

        public:

            static bool getGreen();

            static bool getYellow();

            static bool getRed();

            static uint32_t usbCallback(std::array<uint8_t, 1024>& buffer, uint32_t size);

            UsbUtils() = delete;

            ~UsbUtils() = delete;

    };

}
