#pragma once

#include <cstdint>

#include "exqudens/embedded/usb/export.hpp"

namespace exqudens {

    class EXQUDENS_EMBEDDED_USB_EXPORT TaskService {

        private:

            bool stopped = false;

        public:

            int32_t run();

            void stop();

    };

}
