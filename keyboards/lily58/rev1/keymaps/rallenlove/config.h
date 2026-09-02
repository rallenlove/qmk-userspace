/*
This is the c configuration file for the keymap

Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

//#define USE_MATRIX_I2C

/* Select hand configuration
 *
 * Don't set MASTER_LEFT/MASTER_RIGHT/EE_HANDS here -- users/holykeebs/config.h
 * #undefs all three and re-derives them from the MASTER_SIDE build variable
 * (default right, i.e. -DHK_MASTER_RIGHT). Anything set here is silently
 * discarded. USB goes to the right half, and that half's EEPROM is the one
 * holding the authoritative dynamic keymap.
 */

// Holding a tap-hold key straight after tapping it repeats the tap instead of
// engaging the hold; 0 disables that, so a home row mod is always a mod.
#define QUICK_TAP_TERM 0
#define TAPPING_TERM 180

// Home row mods live on the base layer, so the tap-hold settings below are
// what make ordinary typing survive. Three mechanisms, in the order they get
// a say on a mod-tap that hasn't settled yet:
//
// 1. FLOW_TAP_TERM: pressed within 150ms of the previous key, it's a tap, full
//    stop. Mid-word rolls never reach the rules below, which is what keeps
//    fast typing free of stray mods (and free of tap latency).
// 2. CHORDAL_HOLD: chorded with a key on the *same* hand, it's a tap. Kills
//    the awkward same-hand mod chords rather than misfiring them. Note this
//    only applies within the tapping term -- deliberately holding past 180ms
//    still gets the mod, so same-hand chords are slow but not impossible.
// 3. Opposite hands is the real chord case, and how it settles differs by key:
//
//    - Thumb layer-taps: HOLD_ON_OTHER_KEY_PRESS, held the instant the other
//      hand presses anything. Needed for chords like holding the Mouse thumb
//      key and clicking with the other hand's thumb.
//    - Home row mods: PERMISSIVE_HOLD only, so a hold needs the other key to
//      be pressed *and released* first (a nested press). Cross-hand rolls out
//      of a home row mod are ordinary typing -- "so", "he", "if" -- and
//      HOLD_ON_OTHER_KEY_PRESS would turn every one of them into a mod chord.
//
//    Hence the _PER_KEY form; get_hold_on_other_key_press() in keymap.c is
//    what draws that line.
#define CHORDAL_HOLD
#define FLOW_TAP_TERM 150
#define PERMISSIVE_HOLD
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY

// VIA's dynamic keymap defaults to 4 layers; we have 7.
#define DYNAMIC_KEYMAP_LAYER_COUNT 7

// QK_GESC sends ` instead of Escape whenever Shift or GUI is held, which is what
// puts Cmd+` (cycle windows within an app) on the leftmost column above Cmd+Tab.
//
// Alt has to be carved out, though: without this, Cmd+Alt+Esc -- macOS's Force
// Quit -- would send Cmd+Alt+` and silently stop working. Deliberately NOT
// defining GRAVE_ESC_SHIFT_OVERRIDE or GRAVE_ESC_GUI_OVERRIDE, since either one
// would send Escape for the very chords we want ` from (Cmd+Shift+` reverse-
// cycles windows).
#define GRAVE_ESC_ALT_OVERRIDE

// Underglow
/*
#undef RGBLIGHT_LED_COUNT
#define RGBLIGHT_LED_COUNT 14    // Number of LEDs
#define RGBLIGHT_SLEEP
*/
