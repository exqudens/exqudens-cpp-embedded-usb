#include <cstdlib>

#include "exqudens/embedded/usb/Application.hpp"

#ifdef USE_HAL_DRIVER
#include <optional>

#include "exqudens/embedded/usb/hal/Hardware.hpp"
#endif

namespace exqudens {

    int Application::run() {
        int result = EXIT_SUCCESS;

#ifdef USE_HAL_DRIVER
        std::optional<Hardware> optionalHardware = {};

        if (!hardware) {
            optionalHardware.emplace(Hardware());
            hardware = &optionalHardware.value();
        }
#endif

        if (!hardware) {
            result = EXIT_FAILURE;
            return result;
        }

        hardware->setUsbReceiveCallback(&Application::usbCallback);

        if (!hardware->getUsbReceiveCallback()) {
            result = EXIT_FAILURE;
            return result;
        }

        result = hardware->mainInit();

        if (result != EXIT_SUCCESS) {
            return result;
        }

        while (true) {
            if (!hardware->getGreenLedState()) {
                hardware->setGreenLedState(true);
                hardware->delay(500);
                hardware->setGreenLedState(false);
                hardware->delay(500);
            }
            if (!hardware->getYellowLedState()) {
                hardware->setYellowLedState(true);
                hardware->delay(500);
                hardware->setYellowLedState(false);
                hardware->delay(500);
            }
            if (!hardware->getRedLedState()) {
                hardware->setRedLedState(true);
                hardware->delay(500);
                hardware->setRedLedState(false);
                hardware->delay(500);
            }

            hardware->delay(2000);
        }

        hardware = nullptr;

        return result;
    }

    IHardware* Application::getHardware() {
        return hardware;
    }

    uint32_t Application::usbCallback(std::array<uint8_t, 1024>& buffer, uint32_t size) {
        for (uint32_t i = 0; i < size; i++) {
            if (buffer[i] >= 'a' && buffer[i] <= 'z') {
                buffer[i] -= ('a' - 'A');
            }
        }
        return size;
    }

}
