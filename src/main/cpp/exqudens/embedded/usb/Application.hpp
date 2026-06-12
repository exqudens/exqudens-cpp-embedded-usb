#pragma once

#include <cstdint>

#include "exqudens/embedded/usb/export.hpp"

namespace exqudens {

    class EXQUDENS_EMBEDDED_USB_EXPORT Application {

        public:

            static int32_t run();

            Application() = delete;

            ~Application() = delete;

    };

}
