#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "quantum.h"

// Whether THIS half currently has a working pointing device — used by the
// peripheral info RPC and the runtime ball detection (HK_SPLIT_DETECT_POINTING).
//
// The two QMK bases this userspace builds on diverge here:
//  - Newer bases (qmk_firmware) expose the generic pointing_device_get_status().
//  - Older bases (vial-qmk) lack it, so a board on such a base defines
//    HK_NO_POINTING_DEVICE_STATUS and we use the PMW3360-specific pmw33xx_init_ok
//    global instead. Detection is dual-PMW3360-only, so that's always valid there.
#ifdef HK_NO_POINTING_DEVICE_STATUS
#    include "drivers/sensors/pmw33xx_common.h"
static inline bool hk_local_pointing_present(void) {
    return pmw33xx_init_ok;
}
#else
#    include "pointing_device.h"
static inline bool hk_local_pointing_present(void) {
    return pointing_device_get_status() == POINTING_DEVICE_STATUS_SUCCESS;
}
#endif

typedef enum {
    POINTER_KIND_NONE,
    POINTER_KIND_PIMORONI_TRACKBALL,
    POINTER_KIND_TRACKPOINT,
    POINTER_KIND_CIRQUE35,
    POINTER_KIND_CIRQUE40,
    POINTER_KIND_TPS43,
    POINTER_KIND_TPS65,
    POINTER_KIND_PMW3360,
} hk_pointer_kind;

typedef enum {
    SCROLL_LOCK_OFF,
    SCROLL_LOCK_HORIZONTAL,
    SCROLL_LOCK_VERTICAL,
} hk_scroll_lock;

typedef enum {
    CURSOR_MODE_DEFAULT,
    CURSOR_MODE_SNIPING,
} hk_cursor_mode;

typedef struct PACKED {
    hk_pointer_kind pointer_kind : 4;
    hk_cursor_mode cursor_mode : 2;
    hk_scroll_lock scroll_lock : 2;
    bool drag_scroll : 1;
    bool scroll_direction_inverted : 1;
    float pointer_default_sensitivity;
    float pointer_sniping_sensitivity;
    uint8_t pointer_scroll_throttle;
} hk_pointer_state_t;

#define HK_OLED_MAX_PRESSING_KEYCODES 6

typedef struct PACKED {
    // Per-half bongocat toggle: _main is the master's OLED, _peripheral the other
    // half's. HK_BONGO_TOGGLE flips _main; shift+HK_BONGO_TOGGLE flips _peripheral
    // (the same shift-targets-the-peripheral convention as the pointing config).
    bool           show_bongo_main;
    bool           show_bongo_peripheral;
    uint16_t       last_kc;
    keypos_t       last_pos;
    report_mouse_t last_mouse;

    // Buffer to indicate pressing keys.
    char pressing_keys[HK_OLED_MAX_PRESSING_KEYCODES + 1];
} hk_display_state_t;

typedef struct PACKED {
    bool init : 1;
    bool dirty : 1;

    bool is_main_side : 1;
    bool setting_default_sensitivity : 1;
    bool setting_sniping_sensitivity : 1;
    bool setting_scroll_throttle : 1;

    hk_pointer_state_t main;
    hk_pointer_state_t peripheral;

    hk_display_state_t display;
} hk_state_t;

#ifdef HK_SPLIT_SYNC_STATE
    _Static_assert(sizeof(hk_state_t) <= RPC_M2S_BUFFER_SIZE, "State too big to fit in split sync buffer size.");
#endif

extern hk_state_t g_hk_state;
