#pragma once

#include <cstdint>
#include <array>
#include <functional>

#include "exqudens/embedded/usb/hal/export.hpp"

namespace exqudens {

    class EXQUDENS_EMBEDDED_USB_HAL_EXPORT IHardware {

        public:

            virtual std::array<char, 32> getVersion() = 0;

            virtual int32_t mainInit() = 0;

            virtual uint32_t getTick() = 0;

            virtual void delay(uint32_t milliSeconds) = 0;

            virtual void setGreenLedState(bool state) = 0;

            virtual bool getGreenLedState() = 0;

            virtual void setYellowLedState(bool state) = 0;

            virtual bool getYellowLedState() = 0;

            virtual void setRedLedState(bool state) = 0;

            virtual bool getRedLedState() = 0;

            virtual void setUsbReceiveCallback(const std::function<uint32_t(std::array<uint8_t, 1024>&, uint32_t)>& value) = 0;

            virtual std::function<uint32_t(std::array<uint8_t, 1024>&, uint32_t)> getUsbReceiveCallback() = 0;

            virtual ~IHardware() = default;

    };

}
