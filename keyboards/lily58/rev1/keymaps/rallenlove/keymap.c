 /* Copyright 2020 Naoki Katahira
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 2 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  */

#include QMK_KEYBOARD_H
#include <stdio.h>
#include "users/holykeebs/holykeebs.h"

#define QK_C_EEPROM QK_CLEAR_EEPROM

//extern uint8_t is_master;

// Home row mods, GACS order: GUI / Alt / Ctrl / Shift walking inward from the
// pinky, mirrored on the other hand. Same order the mod rows on NAV, NUM,
// MOUSE and FUN use, so a mod sits under the same finger everywhere.
//
// Left-hand mod keycodes on both hands (LSFT_T on J, not RSFT_T) to match
// those layers, and so a right-hand hold can't turn into AltGr on a non-mac
// host.
//
// These are for cross-hand chords. Same-hand ones -- Cmd+C, Cmd+V, Ctrl+A --
// are what the thumb Cmd/Alt and the outer-column Ctrl/Shift are still there
// for; CHORDAL_HOLD deliberately taps a home row mod chorded with its own
// hand. See config.h for the rest of the tap-hold reasoning.
#define HM_A LGUI_T(KC_A)
#define HM_S LALT_T(KC_S)
#define HM_D LCTL_T(KC_D)
#define HM_F LSFT_T(KC_F)
#define HM_J LSFT_T(KC_J)
#define HM_K LCTL_T(KC_K)
#define HM_L LALT_T(KC_L)
#define HM_SCLN LGUI_T(KC_SCLN)

enum layer_number {
  _QWERTY = 0,
  _NAV,
  _MOUSE,
  _SYM,
  _NUM,
  _FUN,
  _ADJUST,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* QWERTY
 *
 * The home row is GACS mods (see above): tap for the letter, hold for the mod.
 *
 * The leftmost column stacks the two macOS window-cycling chords: GESC sends `
 * when Cmd (or Shift) is held, so Cmd+GESC cycles windows within an app and
 * Cmd+Tab directly below it cycles apps. Pressed alone GESC is still Escape.
 *
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | GESC |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  \   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      | GUI  | Alt  | Ctrl | Shift|      |                    |      | Shift| Ctrl | Alt  | GUI  |      |
 * |LCTRL |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|CW_TOGG|    |QK_REP |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | Alt  |  Cmd |  Num | /Tab    /       \ Mouse\  | Sym  |  Fun | RGUI |
 *                   |      |      | /Spc |/  /Nav /         \ /Bspc\  \/Ent  \/Del  |      |
 *                   `-------------------''-------'           '------''--------------------'
 */

 [_QWERTY] = LAYOUT(
  QK_GESC,  KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
  KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
  KC_LCTL,  HM_A,   HM_S,    HM_D,    HM_F,    KC_G,                     KC_H,    HM_J,    HM_K,    HM_L,    HM_SCLN, KC_QUOT,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, CW_TOGG,  QK_REP,   KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,
        KC_LALT, KC_LGUI, LT(_NUM, KC_SPC), LT(_NAV, KC_TAB),   LT(_MOUSE, KC_BSPC), LT(_SYM, KC_ENT), LT(_FUN, KC_DEL), KC_RGUI
),
/* NAV
 *
 * Every layer held by a thumb repeats the GACS mods as plain mods on the free
 * hand's home row. The base layer's home row mods are out of reach while a
 * thumb key is held -- that hand is on the layer's own keys, or the mod-tap is
 * a same-hand chord with the thumb and settles as a tap -- so Cmd+Left,
 * Shift+PgDn, Cmd+click and Cmd+1 all need the mods on the layer itself.
 *
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    |      | Home | PgDn | PgUp | End  |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      | GUI  | Alt  | Ctrl | Shift|      |-------.    ,-------|      | Left | Down |  Up  |Right |      |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      |  Ins |  Del |      |      |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | Alt  |  Cmd |  Num | /Tab    /       \ Mouse\  | Sym  |  Fun | RGUI |
 *                   |      |      | /Spc |/  /Nav /         \ /Bspc\  \/Ent  \/Del  |      |
 *                   `-------------------''-------'           '------''--------------------'
 */
[_NAV] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                        _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______, _______,                        _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  _______,
  _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, _______,                        _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______,
  _______, _______, _______, _______, _______, _______, _______,  _______,     _______, KC_INS,  KC_DEL,  _______, _______, _______,
                             _______, _______, _______, _______,      _______,  _______, _______, _______
),
/* MOUSE
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      | GUI  | Alt  | Ctrl | Shift|      |-------.    ,-------|      |WheelU|WheelD|      |      |      |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |D_MODE|Btn4  |Btn5  |      |      |S_MODE|-------|    |-------|      |      |      |      |      |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   |      | Mid  |Right |  Left  /       \ Mouse\  | Sym  |  Fun | RGUI |
 *                   |      |click |click |/click /         \ /Bspc\  \/Ent  \/Del  |      |
 *                   `-------------------''-------'           '------''--------------------'
 */
[_MOUSE] = LAYOUT(
  _______,   _______, _______, _______, _______, _______,                        _______, _______, _______, _______, _______, _______,
  _______,   _______, _______, _______, _______, _______,                        _______, _______, _______, _______, _______, _______,
  _______,   KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, _______,                        _______, MS_WHLU, MS_WHLD, _______, _______, _______,
  HK_D_MODE, MS_BTN4, MS_BTN5, _______, _______, HK_S_MODE, _______,  _______,     _______, _______, _______, _______, _______, _______,
                             _______, MS_BTN3, MS_BTN2, MS_BTN1,      _______,  _______, _______, _______
),
/* SYM
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  ~   |   !  |   @  |   #  |   $  |   %  |                    |   ^  |   &  |   *  |   (  |   )  |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------|      |   -  |   _  |   [  |   ]  |  `   |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      |   =  |   +  |   {  |   }  |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | Alt  |  Cmd |  Num | /Tab    /       \ Mouse\  | Sym  |  Fun | RGUI |
 *                   |      |      | /Spc |/  /Nav /         \ /Bspc\  \/Ent  \/Del  |      |
 *                   `-------------------''-------'           '------''--------------------'
 */
[_SYM] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                        _______, _______, _______, _______, _______, _______,
  KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                        KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______,
  _______, _______, _______, _______, _______, _______,                        _______, KC_MINS, KC_UNDS, KC_LBRC, KC_RBRC, KC_GRV,
  _______, _______, _______, _______, _______, _______, _______,  _______,     _______, KC_EQL,  KC_PLUS, KC_LCBR, KC_RCBR, _______,
                             _______, _______, _______, _______,      _______,  _______, _______, _______
),
/* NUM (tenkey layout, 0 on the thumb)
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |   /  |   *  |   -  |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    |      |   7  |   8  |   9  |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      | GUI  | Alt  | Ctrl | Shift|      |-------.    ,-------|      |   4  |   5  |   6  |   +  |      |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      |   1  |   2  |   3  |   .  |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | Alt  |  Cmd |  Num | /Tab    /       \ Mouse\  |  0   |  Fun | RGUI |
 *                   |      |      | /Spc |/  /Nav /         \ /Bspc\  \      \/Del  |      |
 *                   `-------------------''-------'           '------''--------------------'
 */
[_NUM] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                        _______, _______, KC_SLSH, KC_ASTR, KC_MINS, _______,
  _______, _______, _______, _______, _______, _______,                        _______, KC_7,    KC_8,    KC_9,    _______, _______,
  _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, _______,                        _______, KC_4,    KC_5,    KC_6,    KC_PLUS, _______,
  _______, _______, _______, _______, _______, _______, _______,  _______,     _______, KC_1,    KC_2,    KC_3,    KC_DOT,  _______,
                             _______, _______, _______, _______,      _______,  KC_0, _______, _______
),
/* FUN (F-keys plus media; volume on the home row, transport below)
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |                    |  F7  |  F8  |  F9  | F10  | F11  | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      | GUI  | Alt  | Ctrl | Shift|      |-------.    ,-------|      |VolDn |VolUp | Mute |      |      |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      | Prev | Next | Play | Stop |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | Alt  |  Cmd |  Num | /Tab    /       \ Mouse\  | Sym  |  Fun | RGUI |
 *                   |      |      | /Spc |/  /Nav /         \ /Bspc\  \/Ent  \/Del  |      |
 *                   `-------------------''-------'           '------''--------------------'
 */
[_FUN] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                     _______, _______, _______, _______, _______, _______,
  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                       KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
  _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, _______,                     _______, KC_VOLD, KC_VOLU, KC_MUTE, _______, _______,
  _______, _______, _______, _______, _______, _______,   _______, _______, _______, KC_MPRV, KC_MNXT, KC_MPLY, KC_MSTP, _______,
                             _______, _______, _______,  _______, _______,  _______, _______, _______
),

/* ADJUST
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------|      |      |      |      |      |      |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      |      |      |      |      |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   |LOWER | LGUI | Alt  | /Space  /       \Enter \  |BackSP| RGUI |RAISE |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */
  [_ADJUST] = LAYOUT(
    XXXXXXX,      XXXXXXX,    XXXXXXX,    XXXXXXX,      XXXXXXX, XXXXXXX,                        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    QK_BOOT,      HK_DUMP,    HK_SAVE,    HK_RESET,     XXXXXXX, HK_C_SCROLL,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,
    QK_C_EEPROM,  HK_P_SET_D, HK_P_SET_S, HK_P_SET_THR, XXXXXXX, HK_S_MODE_T,                    KC_UP,   KC_DOWN, XXXXXXX, XXXXXXX, XXXXXXX, QK_C_EEPROM,
    KC_LSFT,      XXXXXXX,    XXXXXXX,    XXXXXXX,      XXXXXXX, HK_D_MODE_T, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                                   _______, _______, _______, _______,  _______, _______, _______, _______
  )
};

layer_state_t layer_state_set_user(layer_state_t state) {
  state = update_tri_layer_state(state, _NUM, _FUN, _ADJUST);
  return state;
}

// The thumb layer-taps want HOLD_ON_OTHER_KEY_PRESS; the home row mods must
// not have it, or every cross-hand rolled bigram out of A/S/D/F/J/K/L/; would
// fire a mod chord. Mod-taps fall through to PERMISSIVE_HOLD instead, which
// waits for the nested release. config.h has the full picture.
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
  return IS_QK_LAYER_TAP(keycode);
}

// GOTCHA: if an edit to this file seems to have no effect after flashing, the
// board is still running the previous keymap out of EEPROM.
//
// VIA_ENABLE means keycodes are read from EEPROM's dynamic keymap, not from
// flash. VIA only re-initializes it from flash when via_eeprom_is_valid() fails,
// and that magic is derived from QMK_BUILDDATE's *date* alone -- so a rebuild on
// the same day as the last flash still looks valid and the old keycodes persist.
// Symptoms are confusing rather than dead: an old LT() thumb key keeps sending
// its old layer, so mods silently drop out of chords.
//
// To force a reload: hold Tab+Del (_ADJUST) and press the leftmost home-row key
// (QK_CLEAR_EEPROM). Note that also resets the holykeebs trackball settings; to
// reload only the keymap, temporarily add a via_init_kb() calling
// via_eeprom_set_valid(false) -- but not keyboard_post_init_user, which the
// holykeebs userspace already defines.

#if defined(OLED_ENABLE) && !defined(HK_OLED_ENABLE)

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master())
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  return rotation;
}

static void render_logo(void) {
    static const char PROGMEM logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
        0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00
    };

    oled_write_P(logo, false);
}
char keylog_str[24] = {};
char keylogs_str[21] = {};
int keylogs_str_idx = 0;

const char code_to_name[60] = {
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'R', 'E', 'B', 'T', '_', '-', '=', '[', ']', '\\',
    '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};

void set_keylog(uint16_t keycode, keyrecord_t *record) {
  char name = ' ';
  if (keycode < 60) {
    name = code_to_name[keycode];
  }

  // update keylog
  snprintf(keylog_str, sizeof(keylog_str), "%dx%d, k%2d : %c",
           record->event.key.row, record->event.key.col,
           keycode, name);

  // update keylogs
  if (keylogs_str_idx == sizeof(keylogs_str) - 1) {
    keylogs_str_idx = 0;
    for (int i = 0; i < sizeof(keylogs_str) - 1; i++) {
      keylogs_str[i] = ' ';
    }
  }

  keylogs_str[keylogs_str_idx] = name;
  keylogs_str_idx++;
}

const char *read_keylog(void) {
  return keylog_str;
}

const char *read_keylogs(void) {
  return keylogs_str;
}
//new

bool oled_task_user(void) {
  #ifdef OLED_FLIP
  bool condition = !is_keyboard_master();
  #else
  bool condition = is_keyboard_master();
  #endif
  if (condition) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
    case _QWERTY:
        oled_write_ln_P(PSTR("Default"), false);
        break;
    case _NAV:
        oled_write_ln_P(PSTR("Nav"), false);
        break;
    case _MOUSE:
        oled_write_ln_P(PSTR("Mouse"), false);
        break;
    case _SYM:
        oled_write_ln_P(PSTR("Sym"), false);
        break;
    case _NUM:
        oled_write_ln_P(PSTR("Num"), false);
        break;
    case _FUN:
        oled_write_ln_P(PSTR("Fun"), false);
        break;
    case _ADJUST:
        oled_write_ln_P(PSTR("Adjust"), false);
        break;
    default:
        oled_write_ln_P(PSTR("Undefined"), false);
    }

    oled_write_ln(read_keylog(), false);
    oled_write_ln(read_keylogs(), false);

  } else {
      render_logo();
  }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
#ifdef OLED_ENABLE
    set_keylog(keycode, record);
#endif
    // set_timelog();
  }
  return true;
}

#endif // OLED_ENABLE
