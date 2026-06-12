#include <cstdlib>

#include "exqudens/embedded/usb/service/TaskService.hpp"
#include "exqudens/embedded/usb/hal/HardwareFactory.hpp"
#include "exqudens/embedded/usb/util/UsbUtils.hpp"

namespace exqudens {

    int32_t TaskService::run() {
        IHardware* hardware = HardwareFactory::getHardware();
        int32_t result = hardware ? EXIT_SUCCESS : EXIT_FAILURE;

        if (result != EXIT_SUCCESS) {
            return result;
        }

        result = hardware->mainInit();

        if (result != EXIT_SUCCESS) {
            return result;
        }

        hardware->setGreenLedState(UsbUtils::getGreen());
        hardware->setYellowLedState(UsbUtils::getYellow());
        hardware->setRedLedState(UsbUtils::getRed());
        hardware->setUsbReceiveCallback(&UsbUtils::usbCallback);
        stopped = false;

        while (!stopped) {
            hardware->setGreenLedState(UsbUtils::getGreen());
            hardware->setYellowLedState(UsbUtils::getYellow());
            hardware->setRedLedState(UsbUtils::getRed());
        }

        return result;
    }

    void TaskService::stop() {
        stopped = true;
    }

}
