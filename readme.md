# exqudens-cpp-embedded-usb

- Uses board: NUCLEO-H7A3ZI-Q with MCU: STM32H7A3ZI
- Requires: `cmake`, `ninja`, `STM32CubeMX`, `openocd`

##### How To Generate

```
cmake -DUSE_CUBE=ON --preset windows.ninja.gcc-arm.debug.static
```

##### How To Build

```
cmake --preset windows.ninja.gcc-arm.debug.static
cmake --build --preset windows.ninja.gcc-arm.debug.static --target clean
cmake --build --preset windows.ninja.gcc-arm.debug.static --target exqudens-cpp-embedded-usb-app
```

##### How To Upload

```
cmake --preset windows.ninja.gcc-arm.debug.static
cmake --build --preset windows.ninja.gcc-arm.debug.static --target clean
cmake --build --preset windows.ninja.gcc-arm.debug.static --target exqudens-cpp-embedded-usb-app
cmake --build --preset windows.ninja.gcc-arm.debug.static --target cmake-flash-write
```

##### Tests

Device VID: `0x0483 (1155)`
Device PID: `0x5741 (22337)`
File: `src/main/c/exqudens/embedded/usb/USB_DEVICE/App/usbd_desc.c`

Assign driver:
1. Open `Control Panel` and switch view to `Small icons` (Top right corner).
2. Find and right click `Device Manager` select `Run as administrator`.
3. The device should be in `Other devices` group with name `STM32 Virtual USBPort`.
4. Right click device and select `Update driver`.
5. In opened window select `Browse my computer for drivers`.
6. In opened window select `Let me pick from a list of available drivers on my computer`.
7. In opened window select `Universal Serial Bus devices` (Bottom) and click `Next`.
8. In opened window select `WinUsb Device` for `Manufacturer` (Left list) and `WinUsb Device` for `Model` (Right list) and click `Next`.
9. In opened window select `Yes` for `Update Driver Warning`.
10. In opened window click `Close`.
11. The device now should be in `Universal Serial Bus devices` group with name `STM32 Virtual USBPort`.
12. Close `Device Manager`.
