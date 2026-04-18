#pragma once

#include <cstdint>
#include <array>

#include "exqudens/embedded/usb/export.hpp"
#include "exqudens/embedded/usb/hal/IHardware.hpp"

namespace exqudens {

    class EXQUDENS_EMBEDDED_USB_EXPORT Application {

        private:

            inline static IHardware* hardware = nullptr;

        public:

            static int run();

            static IHardware* getHardware();

            static uint32_t usbCallback(std::array<uint8_t, 1024>& buffer, uint32_t size);

            Application() = delete;

            ~Application() = delete;

    };

}
