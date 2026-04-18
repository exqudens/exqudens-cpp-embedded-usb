#pragma once

#include <optional>

#include "exqudens/embedded/usb/hal/IHardware.hpp"

namespace exqudens {

    class EXQUDENS_EMBEDDED_USB_HAL_EXPORT Hardware : public IHardware {

        private:

            std::optional<int32_t> mainInitResult = {};
            std::function<uint32_t(std::array<uint8_t, 1024>&, uint32_t)> usbReceiveCallback = {};

        public:

            std::array<char, 32> getVersion() override;

            int32_t mainInit() override;

            uint32_t getTick() override;

            void delay(uint32_t milliSeconds) override;

            void setGreenLedState(bool state) override;

            bool getGreenLedState() override;

            void setYellowLedState(bool state) override;

            bool getYellowLedState() override;

            void setRedLedState(bool state) override;

            bool getRedLedState() override;

            void setUsbReceiveCallback(const std::function<uint32_t(std::array<uint8_t, 1024>&, uint32_t)>& value) override;

            std::function<uint32_t(std::array<uint8_t, 1024>&, uint32_t)> getUsbReceiveCallback() override;

            ~Hardware() override;

    };

}
