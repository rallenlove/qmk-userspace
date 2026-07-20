// Hand detection from the fitted pointing device (HK_HAND_FROM_POINTER).
//
// keyball61plus's PCB has matrix-grid hand detection (a hardwired lROW2/lCOL3
// intersection through lD33 on the left half), but the path crosses two diodes
// in series — lD33 plus the shared duplex column diode lD37 — so the sense pin
// only falls to about two diode drops (~1.0-1.4V), right at the RP2040's
// Schmitt input threshold, and the read misfires on production boards. Until a
// PCB revision lands the hand diode directly on the column pin, boards define
// HK_HAND_FROM_POINTER and detect the hand from the fitted PMW3360 instead:
// sensor present = right half.
//
// This couples handedness to the sold variant (ball on the right half only).
// A dual-ball or left-ball build must not use this; go back to the grid (on a
// fixed PCB rev) or EE_HANDS.

#ifdef HK_HAND_FROM_POINTER

#include "quantum.h"
#include "drivers/sensors/pmw33xx_common.h"

// split_pre_init resolves handedness once, before pointing_device_init runs,
// so the probe brings up SPI itself and polls: the PMW3360 needs ~50ms from
// power-up before its registers read back reliably, and boot can get here
// sooner. A missing sensor leaves MISO floating, hence requiring both the
// product ID and its complement — noise can't plausibly produce the pair. The
// no-sensor (left) half pays the full poll window at boot, well inside the 3s
// split watchdog.
bool is_keyboard_left_impl(void) {
    spi_init();
    for (uint8_t attempt = 0; attempt < 20; attempt++) {
        if (pmw33xx_read(0, REG_Product_ID) == 0x42 && pmw33xx_read(0, REG_Inverse_Product_ID) == 0xBD) {
            return false;
        }
        wait_ms(10);
    }
    return true;
}

#endif
