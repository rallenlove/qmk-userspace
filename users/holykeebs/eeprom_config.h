#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "eeconfig.h"

// Increment when the structure below changes, so old saves reset cleanly
// instead of being deserialized as garbage.
#define HK_EEPROM_CONFIG_VERSION 104

typedef union PACKED {
    uint8_t raw[EECONFIG_USER_DATA_SIZE];
    struct {
        struct {
            // The kinds the rest of this block was saved for. Settings are stored
            // per side (main/peripheral), not per device, so a block written by a
            // different POINTING_DEVICE build describes the wrong hardware: at best
            // a multiplier tuned for another device, at worst the 0 that a side
            // without a device saves, which multiplies that side's motion to
            // nothing. keyboard_post_init_user resets the block when these don't
            // match what the running firmware detected.
            hk_pointer_kind main_pointer_kind : 4;
            hk_pointer_kind peripheral_pointer_kind : 4;

            hk_cursor_mode main_cursor_mode : 2;
            bool main_drag_scroll : 1;
            bool main_scroll_direction_inverted : 1;
            hk_scroll_lock main_scroll_lock : 2;
            int16_t main_default_sensitivity;
            int16_t main_sniping_sensitivity;
            uint8_t main_scroll_throttle;

            hk_cursor_mode peripheral_cursor_mode : 2;
            bool peripheral_drag_scroll : 1;
            bool peripheral_scroll_direction_inverted : 1;
            hk_scroll_lock peripheral_scroll_lock : 2;
            int16_t peripheral_default_sensitivity;
            int16_t peripheral_sniping_sensitivity;
            uint8_t peripheral_scroll_throttle;
        } pointing;

        bool check            : 1;
        bool bongo_main       : 1;
        bool bongo_peripheral : 1;
        bool aml_enable       : 1;
        uint8_t version;
        uint16_t aml_timeout;
    };
} hk_eeprom_config_t;

_Static_assert(sizeof(hk_eeprom_config_t) <= EECONFIG_USER_DATA_SIZE, "User EECONFIG block is not large enough.");

extern hk_eeprom_config_t hk_eeprom_config;
