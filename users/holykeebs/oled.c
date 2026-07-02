#include "pointing.h"
#include "quantum.h"
#include <string.h>

#ifdef HK_BONGO_ENABLE
#include "bongocat.h"
#endif

static const char* pointer_kind_to_string(hk_pointer_kind kind) {
    switch (kind) {
        case POINTER_KIND_NONE:
            return "NONE \xB1";
        case POINTER_KIND_PIMORONI_TRACKBALL:
            return "PIM  \xB1";
        case POINTER_KIND_TRACKPOINT:
            return "TP   \xB1";
        case POINTER_KIND_CIRQUE35:
            return "CR35 \xB1";
        case POINTER_KIND_CIRQUE40:
            return "CR40 \xB1";
        case POINTER_KIND_TPS43:
            return "TPS43\xB1";
        case POINTER_KIND_PMW3360:
            return "PMW  \xB1";
        default:
            return "?????\xB1";
    }
}

static const char BL = '\xB0'; // Blank indicator character
static const char LFSTR_ON[] PROGMEM = "\xB2\xB3";
static const char LFSTR_OFF[] PROGMEM = "\xB4\xB5";

static const char *format_3d(int8_t d) {
    static char buf[10] = {0};
    char        lead   = ' ';
    if (d < 0) {
        d    = -d;
        lead = '-';
    }
    sprintf(buf, "%c%2d", lead, d);
    return buf;
}

static const char *format_2d(int8_t d) {
    static char buf[10] = {0};
    if (d > 99) {
        d = 99;
    }
    sprintf(buf, "%02d",d);
    return buf;
}

static char to_1x(uint8_t x) {
    x &= 0x0f;
    return x < 10 ? x + '0' : x + 'a' - 10;
}

static const char* format_sensitivity(float f) {
    static char buf[10] = {0};
    if (f < 0) {
        sprintf(buf, "err");
    } else if (f >= 100) {
        // Hardware-CPI devices (PMW3360) store the raw CPI here, e.g. 800.
        sprintf(buf, "%d", (int)f);
    } else {
        // Software movement multiplier, e.g. 1.5.
        sprintf(buf, "%2.1f", f);
    }
    return buf;
}

void hk_oled_render_pointer_state(void) {
    // Output example:
    //
    //  TPS43: -12  34   0   0
    //  CUR D: 1.0/5  ON LK:VT

    // 1st line, pointing device kind, mouse x, y, h, and v.
    oled_write_P(pointer_kind_to_string(g_hk_state.main.pointer_kind), false);

    oled_write(format_3d(g_hk_state.display.last_mouse.x), false);
    oled_write(format_3d(g_hk_state.display.last_mouse.y), false);
    oled_write(format_3d(g_hk_state.display.last_mouse.h), false);
    oled_write_ln(format_3d(g_hk_state.display.last_mouse.v), false);

    // 2nd line, cursor mode, default sensitivity, drag scroll mode, scroll lock mode, and scroll throttle.
    if (g_hk_state.setting_default_sensitivity) {
        oled_write_P(PSTR("CUR D\xB1"), false);
        oled_write(format_sensitivity(g_hk_state.main.pointer_default_sensitivity), false);
    } else if (g_hk_state.setting_sniping_sensitivity) {
        oled_write_P(PSTR("CUR S\xB1"), false);
        oled_write(format_sensitivity(g_hk_state.main.pointer_sniping_sensitivity), false);
    } else {
        switch (g_hk_state.main.cursor_mode)
        {
            case CURSOR_MODE_DEFAULT:
                oled_write_P(PSTR("CUR D\xB1"), false);
                oled_write(format_sensitivity(g_hk_state.main.pointer_default_sensitivity), false);
                break;
            case CURSOR_MODE_SNIPING:
                oled_write_P(PSTR("CUR S\xB1"), false);
                oled_write(format_sensitivity(g_hk_state.main.pointer_sniping_sensitivity), false);
                break;
            default:
                oled_write_P(PSTR("CUR ?\xB1"), false);
                break;
        }
    }

    oled_write_char('/', false);
    // scroll throttle:
    oled_write(format_2d(g_hk_state.main.pointer_scroll_throttle), false);
    oled_write_char(' ', false);

    // drag scroll mode: on/off
    if (g_hk_state.main.drag_scroll) {
        oled_write_P(LFSTR_ON, false);
    } else {
        oled_write_P(LFSTR_OFF, false);
    }

    // scroll lock mode: "VT" (vertical), "HN" (horiozntal), and "NO" (free)
    switch (g_hk_state.main.scroll_lock) {
        case SCROLL_LOCK_VERTICAL:
            oled_write_ln_P(PSTR(" L:VT"), false);
            break;
        case SCROLL_LOCK_HORIZONTAL:
            oled_write_ln_P(PSTR(" L:HO"), false);
            break;
        default:
            oled_write_ln_P(PSTR(" L:NO"), false);
            break;
    }
}

void hk_oled_render_keyinfo(void) {
    // Format: `Key :  R{row}  C{col} K{kc} {name}{name}{name}`
    //
    // Where `kc` is lower 8 bit of keycode.
    // Where `name`s are readable labels for pressing keys, valid between 4 and 56.
    //
    // `row`, `col`, and `kc` indicates the last processed key,
    // but `name`s indicate unreleased keys in best effort.
    //
    // For example:
    //
    //     Key  :  R2  C3 K06 abc
    //     TPS43:   0   0   0   0

    // "Key" Label.
    oled_write_P(PSTR("Key  \xB1"), false);

    // Row and column.
    oled_write_char('\xB8', false);
    oled_write_char(to_1x(g_hk_state.display.last_pos.row), false);
    oled_write_char('\xB9', false);
    oled_write_char(to_1x(g_hk_state.display.last_pos.col), false);

    // Keycode.
    oled_write_P(PSTR("\xBA\xBB"), false);
    oled_write_char(to_1x(g_hk_state.display.last_kc >> 4), false);
    oled_write_char(to_1x(g_hk_state.display.last_kc), false);

    // Keys currently pressed.
    oled_write_P(PSTR(" "), false);
    oled_write(g_hk_state.display.pressing_keys, false);
}

void hk_oled_render_layerinfo(void) {
    // Format: `Layer:{layer state}`
    //
    // Output example:
    //
    //     Layer:-23------------
    //
    oled_write_P(PSTR("Layer\xB1"), false);
    for (uint8_t i = 1; i < 8; i++) {
        oled_write_char((layer_state_is(i) ? to_1x(i) : BL), false);
    }
    oled_write_char(' ', false);

#    ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
    oled_write_P(PSTR("\xC2\xC3"), false);
    if (get_auto_mouse_enable()) {
        oled_write_P(LFSTR_ON, false);
    } else {
        oled_write_P(LFSTR_OFF, false);
    }

    oled_write(format_3d(get_auto_mouse_timeout()), false);
    oled_write_char('0', false);
#    else
    oled_write_P(PSTR("\xC2\xC3\xB4\xB5 --"), false);
#    endif
}

// The default info-panel layout: key info, pointer state, and layer/auto-mouse
// status. Rendered on the master, and on the peripheral when it mirrors the master
// (the weak hk_oled_render_secondary below).
static void hk_oled_render_info_panels(void) {
    hk_oled_render_keyinfo();
    hk_oled_render_pointer_state();
    hk_oled_render_layerinfo();
}

#ifdef HK_BONGO_ENABLE
// Render either bongocat or the info panels for one OLED, wiping it once on each
// transition. The two layouts cover different pixels, so without the clear the
// previous one bleeds through. *shown remembers what's currently drawn (pass a
// separate instance per OLED).
static void hk_oled_render_bongo_or_panels(bool show_bongo, bool *shown) {
    if (*shown != show_bongo) {
        *shown = show_bongo;
        oled_clear();
    }
    if (show_bongo) {
        hk_oled_render_bongo();
    } else {
        hk_oled_render_info_panels();
    }
}
#endif

// Shown when USB is plugged into the wrong half. With SPLIT_USB_DETECT the USB
// side always becomes the master, but these builds pin the master to the
// pointing-device side (MASTER_SIDE in rules.mk), so a correctly-plugged master
// always has its pointing device locally.
//
// Rendered as a train-destination-sign marquee: one vertically centered line,
// scrolling right-to-left and wrapping around the edge. The cycle is the message
// plus a gap so the wrap reads as a loop, and each frame draws a display-wide
// window into it (a software marquee: the SSD1306 hardware scroll would be free,
// but the driver can't update the buffer while it runs).
#define HK_OLED_MARQUEE_STEP_MS 200

static void hk_oled_render_connect_msg(void) {
    static const char msg[]     = "Connect USB to other half     ";
    static const uint8_t cycle  = sizeof(msg) - 1;
    static uint8_t  offset      = 0;
    static uint32_t last_step   = 0;

    if (timer_elapsed32(last_step) > HK_OLED_MARQUEE_STEP_MS) {
        last_step = timer_read32();
        offset    = (offset + 1) % cycle;
    }

    // For an even number of rows a single line can't sit exactly on the center;
    // the lower-middle row (row 2 of 4 on a 128x32) reads centered on hardware.
    oled_set_cursor(0, oled_max_lines() / 2);
    for (uint8_t i = 0; i < oled_max_chars(); i++) {
        oled_write_char(msg[(offset + i) % cycle], false);
    }
}

// True when USB is in the wrong half: this half is the master, a pointing device
// is configured for the master side, but none is present locally — it's sitting
// on the other half. The device status is fixed by pointing_device_init before
// the first render, so no settling time is needed. Skipped for boards with
// runtime ball detection (HK_SPLIT_DETECT_POINTING): there a master without a
// local device is legitimate.
static bool hk_oled_wrong_half(void) {
#if defined(POINTING_DEVICE_ENABLE) && !defined(HK_SPLIT_DETECT_POINTING)
    return g_hk_state.main.pointer_kind != POINTER_KIND_NONE && !hk_local_pointing_present();
#else
    return false;
#endif
}

// Secondary (peripheral) OLED content. Weak default mirrors the master's info
// panels (using the split-synced state), or shows its own bongocat when toggled on
// with shift+HK_BONGO_TOGGLE. Boards override this to show something else
// (keyball61plus draws the Keyball logo). oled_task_user only calls this once the
// state is initialized, so no init guard is needed here.
__attribute__((weak)) void hk_oled_render_secondary(void) {
#ifdef HK_BONGO_ENABLE
    static bool shown = false;
    hk_oled_render_bongo_or_panels(g_hk_state.display.show_bongo_peripheral, &shown);
#else
    hk_oled_render_info_panels();
#endif
}

bool oled_task_user(void) {
    // Until this half's state is initialized, neither render path has valid
    // content, so keep the OLED blank (not power-on garbage). This is a
    // peripheral before its first state sync — which arrives with the first
    // keypress or pointer motion, so an idle boot can sit here a while; that's
    // normal, not an error.
    if (!g_hk_state.init) {
        oled_clear();
        return true;
    }

    if (is_keyboard_master()) {
        // USB in the wrong half: this master should have its pointing device
        // locally but doesn't. Hint instead of the panels (wipe on each
        // transition so the layouts don't overlap).
        static bool showing_wrong_half = false;
        bool wrong_half = hk_oled_wrong_half();
        if (wrong_half != showing_wrong_half) {
            showing_wrong_half = wrong_half;
            oled_clear();
        }
        if (wrong_half) {
            hk_oled_render_connect_msg();
            return true;
        }
#ifdef HK_BONGO_ENABLE
        static bool shown = false;
        hk_oled_render_bongo_or_panels(g_hk_state.display.show_bongo_main, &shown);
#else
        hk_oled_render_info_panels();
#endif
    } else {
        hk_oled_render_secondary();
    }
    return true;
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    // The off-hand (peripheral) OLED is mounted inverted relative to the master on
    // these split boards, so flip it 180° by default. Compose with the board's base
    // rotation (^ OLED_ROTATION_180) rather than forcing a value, so a board whose
    // master OLED is itself rotated stays consistent. This is the long-standing
    // default and matches the per-keymap stock-OLED behavior; a board/keymap can
    // still override oled_init_user for an unusual mounting.
    if (!is_keyboard_master()) {
        return rotation ^ OLED_ROTATION_180;
    }
    return rotation;
}
