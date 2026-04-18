#pragma once

#include <array>

#include "exqudens/embedded/usb/hal/export.hpp"

namespace exqudens {

    class EXQUDENS_EMBEDDED_USB_HAL_EXPORT HardwareUtils {

        public:

            static std::array<char, 32> getVersion();

            HardwareUtils() = delete;

            ~HardwareUtils() = delete;

    };

}
