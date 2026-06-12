# exqudens-cpp-embedded-usb

-   Uses board: `NUCLEO-H7A3ZI-Q`
-   Uses MCU: `STM32H7A3ZI`
-   Requires: `cmake`, `ninja`, `STM32CubeMX`, `openocd`

## how-to-configure

01. `cmake --preset windows.ninja.gcc-arm.debug.static`

## how-to-build

01. `cmake --preset windows.ninja.gcc-arm.debug.static`
01. `cmake --build --preset windows.ninja.gcc-arm.debug.static --target clean`
01. `cmake --build --preset windows.ninja.gcc-arm.debug.static --target exqudens-cpp-embedded-usb-app`


## how-to-upload

01. `cmake --preset windows.ninja.gcc-arm.debug.static`
01. `cmake --build --preset windows.ninja.gcc-arm.debug.static --target clean`
01. `cmake --build --preset windows.ninja.gcc-arm.debug.static --target exqudens-cpp-embedded-usb-app`
01. `cmake --build --preset windows.ninja.gcc-arm.debug.static --target cmake-flash-write`

## how-to-test

Device VID: `0x0484 (1156)`
Device PID: `0x5741 (22337)`
File: `src/main/c/exqudens/embedded/usb/USB_DEVICE/App/usbd_desc.c`

**assign-driver:**

01. Open `Control Panel` and switch view to `Small icons` (Top right corner).
01. Find and right click `Device Manager` select `Run as administrator`.
01. The device should be in `Other devices` group with name `STM32 Virtual USBPort`.
01. Right click device and select `Update driver`.
01. In opened window select `Browse my computer for drivers`.
01. In opened window select `Let me pick from a list of available drivers on my computer`.
01. In opened window select `Universal Serial Bus devices` (Bottom) and click `Next`.
01. In opened window select `WinUsb Device` for `Manufacturer` (Left list) and `WinUsb Device` for `Model` (Right list) and click `Next`.
01. In opened window select `Yes` for `Update Driver Warning`.
01. In opened window click `Close`.
01. The device now should be in `Universal Serial Bus devices` group with name `STM32 Virtual USBPort`.
01. Close `Device Manager`.

**bash:**

01. `test-app --gtest_filter=SystemTests.test1 --user-input-string=green_1`
01. `test-app --gtest_filter=SystemTests.test1 --user-input-string=green_0`
01. `test-app --gtest_filter=SystemTests.test1 --user-input-string=yellow_1`
01. `test-app --gtest_filter=SystemTests.test1 --user-input-string=yellow_0`
01. `test-app --gtest_filter=SystemTests.test1 --user-input-string=red_1`
01. `test-app --gtest_filter=SystemTests.test1 --user-input-string=red_0`
