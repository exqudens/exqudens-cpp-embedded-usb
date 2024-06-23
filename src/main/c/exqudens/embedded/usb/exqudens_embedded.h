#ifndef EXQUDENS_EMBEDDED_H
#define EXQUDENS_EMBEDDED_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern void exqudens_embedded_usb_transfer(uint8_t* buffer, uint32_t* length);

#ifdef __cplusplus
}
#endif

#endif // EXQUDENS_EMBEDDED_SERIAL_H
