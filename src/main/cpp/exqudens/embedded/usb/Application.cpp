#include <cstdlib>

#include "exqudens/embedded/usb/Application.hpp"
#include "exqudens/embedded/usb/service/TaskService.hpp"

#ifdef USE_HAL_DRIVER
#include "exqudens/embedded/usb/hal/Hardware.hpp"
#include "exqudens/embedded/usb/hal/HardwareFactory.hpp"
#endif

namespace exqudens {

    int32_t Application::run() {
#ifdef USE_HAL_DRIVER
        Hardware hardware;
        HardwareFactory::setHardware(&hardware);
#endif
        TaskService taskService;
        int32_t result = taskService.run();

        return result;
    }

}
