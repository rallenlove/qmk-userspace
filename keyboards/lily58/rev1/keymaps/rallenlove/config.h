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

#define QUICK_TAP_TERM 0
#define TAPPING_TERM 150

// Resolve a layer-tap thumb key held on one hand + a key pressed on the
// other hand as a hold immediately, instead of deferring the decision.
// Needed for chords like holding the Mouse thumb key and clicking with
// the other hand's thumb.
//
// CHORDAL_HOLD alone only forces the *same-hand* case to a tap; the
// opposite-hands case needs HOLD_ON_OTHER_KEY_PRESS (or PERMISSIVE_HOLD)
// to actually settle as a hold. Together: same-hand rolls tap, opposite-
// hand chords hold immediately.
#define CHORDAL_HOLD
#define HOLD_ON_OTHER_KEY_PRESS

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
