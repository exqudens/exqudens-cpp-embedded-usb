#include <string.h>

#include "exqudens/embedded/usb/hal/Hardware.hpp"
#include "exqudens/embedded/usb/hal/HardwareUtils.hpp"
#include "exqudens/embedded/usb/Application.hpp"
#include "main.h"
#include "usbd_cdc_if.h"

extern "C" void CDC_Receive_HS_Callback(uint8_t* buffer, uint32_t* length) {
    uint32_t size = *length;
    exqudens::IHardware* hardware = exqudens::Application::getHardware();
    std::function<uint32_t(std::array<uint8_t, 1024>&, uint32_t)> usbReceiveCallback = {};

    if (hardware) {
        usbReceiveCallback = hardware->getUsbReceiveCallback();
    }

    if (usbReceiveCallback) {
        std::array<uint8_t, 1024> data = {};
        if (size > data.size()) {
            size = (uint32_t) data.size();
        }
        memcpy(data.data(), buffer, size);
        size = usbReceiveCallback(data, size);
        if (size > data.size()) {
            size = (uint32_t) data.size();
        }
        memcpy(buffer, data.data(), size);
    }

    CDC_Transmit_HS(buffer, size);
}

namespace exqudens {

    std::array<char, 32> Hardware::getVersion() {
        return HardwareUtils::getVersion();
    }

    int32_t Hardware::mainInit() {
        if (!mainInitResult) {
            mainInitResult = main_init();
        }
        return mainInitResult.value();
    }

    uint32_t Hardware::getTick() {
        return HAL_GetTick();
    }

    void Hardware::delay(uint32_t milliSeconds) {
        HAL_Delay(milliSeconds);
    }

    void Hardware::setGreenLedState(bool state) {
        if (state) {
            HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
        }
    }

    bool Hardware::getGreenLedState() {
        return (bool) HAL_GPIO_ReadPin(LD1_GPIO_Port, LD1_Pin);
    }

    void Hardware::setYellowLedState(bool state) {
        if (state) {
            HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
        }
    }

    bool Hardware::getYellowLedState() {
        return (bool) HAL_GPIO_ReadPin(LD2_GPIO_Port, LD2_Pin);
    }

    void Hardware::setRedLedState(bool state) {
        if (state) {
            HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
        }
    }

    bool Hardware::getRedLedState() {
        return (bool) HAL_GPIO_ReadPin(LD3_GPIO_Port, LD3_Pin);
    }

    void Hardware::setUsbReceiveCallback(const std::function<uint32_t(std::array<uint8_t, 1024>&, uint32_t)>& value) {
        usbReceiveCallback = value;
    }

    std::function<uint32_t(std::array<uint8_t, 1024>&, uint32_t)> Hardware::getUsbReceiveCallback() {
        return usbReceiveCallback;
    }

    Hardware::~Hardware() = default;

}
