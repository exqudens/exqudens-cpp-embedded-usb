#pragma once

#include <array>

#include "exqudens/embedded/usb/hal/IHardware.hpp"

namespace exqudens {

    class EXQUDENS_EMBEDDED_USB_HAL_EXPORT HardwareFactory {

        private:

            inline static IHardware* hardware = nullptr;

        public:

            static std::array<char, 32> getVersion();

            static void setHardware(IHardware* value);

            static IHardware* getHardware();

            HardwareFactory() = delete;

            ~HardwareFactory() = delete;

    };

}
