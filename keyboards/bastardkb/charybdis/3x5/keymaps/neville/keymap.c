/**
* Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
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

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    include "timer.h"
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#ifdef POINTING_DEVICE_ENABLE
#    include "drivers/sensors/cirque_pinnacle.h"
#endif

enum charybdis_keymap_layers {
    LAYER_BASE = 0,
    LAYER_LOWER,
    LAYER_RAISE,
    LAYER_DUAL,
};

#define LOWER MO(LAYER_LOWER)
#define RAISE MO(LAYER_RAISE)
#define PT_SPC LT(LAYER_DUAL, KC_SPC)
#define PT_DOT LT(LAYER_DUAL, KC_DOT)

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[LAYER_BASE] = LAYOUT(
    KC_Q,      KC_W,       KC_E,        KC_R,              KC_T,              KC_Y,      KC_U,        KC_I,        KC_O,              KC_P, \
    SFT_T(KC_A),      KC_S,       KC_D,        KC_F,       ALT_T(KC_G),      RALT_T(KC_H),      KC_J,        KC_K,        KC_L,   RSFT_T(KC_QUOT), \
    CTL_T(KC_Z),      KC_X,       KC_C,        KC_V,      LGUI_T(KC_B),      RCMD_T(KC_N),      KC_M,     KC_COMM,      KC_DOT,            KC_ENT, \
    RAISE,    RAISE,   LOWER,           KC_BTN1,   KC_BTN2
),

[LAYER_RAISE] = LAYOUT( 
    KC_ESC,      KC_7,       KC_8,         KC_9,           KC_GRV,           KC_LPRN,    KC_RPRN,     KC_MINS,     KC_EQL,           KC_BSPC, \
    SFT_T(KC_TAB),      KC_4,       KC_5,         KC_6,          KC_LALT,            KC_DLR,    KC_AMPR,       KC_AT,    KC_SCLN,   RSFT_T(KC_QUOT), \
    CTL_T(KC_0),      KC_1,       KC_2,         KC_3,           KC_SPC,           KC_ASTR,    KC_EXLM,     KC_BSLS,    KC_SLSH,            KC_ENT, \
    XXXXXXX,   XXXXXXX,    _______,           KC_BTN1,   KC_BTN2
),

[LAYER_LOWER] = LAYOUT(
    KC_F9,    KC_F10,     KC_F11,       KC_F12,         KC_TILD,           KC_LBRC,    KC_RBRC,    KC_UNDS,     KC_PLUS,           KC_DEL, \
    SFT_T(KC_F5),     KC_F6,      KC_F7,        KC_F8,         KC_LCMD,           KC_LCBR,    KC_RCBR,    KC_HASH,     KC_COLN,          KC_RSFT, \
    CTL_T(KC_F1),     KC_F2,      KC_F3,        KC_F4,          KC_SPC,           KC_PERC,    KC_CIRC,    KC_PIPE,     KC_QUES,           KC_ENT, \
    _______,        _______,   XXXXXXX,           KC_BTN1,    KC_BTN2
),

[LAYER_DUAL] = LAYOUT(
    KC_BRID,   KC_BRIU,   C(KC_UP),    LAG(KC_D),        RGB_VAI,            KC_MRWD,     KC_MPLY,   KC_MFFD,      KC_VOLD,         KC_VOLU, \
    SFT_T(KC_CAPS),   RGB_SAD,    RGB_SAI,      RGB_HUD,        RGB_HUI,            DPI_MOD,    DPI_RMOD,     KC_UP,      KC_PGUP,         KC_PGDN, \
    SNIPING,   S_D_MOD,   S_D_RMOD,        DT_UP,        DT_DOWN,            DT_PRNT,     KC_LEFT,   KC_DOWN,     KC_RIGHT,         DRG_TOG, \
    _______,    _______,    _______,            KC_BTN1,     KC_BTN2
),
};
// clang-format on

#ifdef POINTING_DEVICE_ENABLE

void keyboard_post_init_user(void) {
    pointing_device_set_cpi(PMW3360_CPI_DEFAULT);  // trackball sensitivity
    // Cirque scale setter not available in your QMK build
}

// Merge right (trackball) + left (trackpad)
report_mouse_t pointing_device_task_combined_user(report_mouse_t right, report_mouse_t left) {
    if (right.x > -2 && right.x < 2) right.x = 0;
    if (right.y > -2 && right.y < 2) right.y = 0;

    right.x += left.x / 2;
    right.y += left.y / 2;

    right.h += left.h;
    right.v += left.v;
    right.buttons |= left.buttons;
    return right;
}

#endif // POINTING_DEVICE_ENABLE

#ifdef RGB_MATRIX_ENABLE
void rgb_matrix_update_pwm_buffers(void);
#endif

enum combos {
    RAISE_LOWER_LAYER_DUAL,
    BTN1_BTN2_BTN3,
    DOT_COMM_DRGSCRL,
};

const uint16_t PROGMEM raise_lower_layer_dual[] = { RAISE, LOWER, COMBO_END};
const uint16_t PROGMEM btn1_btn2_btn3[] = { KC_BTN1, KC_BTN2, COMBO_END};
const uint16_t PROGMEM dot_ent_drgscrl[] = { KC_DOT, KC_COMM, COMBO_END};

combo_t key_combos[] = {
    COMBO(raise_lower_layer_dual, MO(LAYER_DUAL)),
    COMBO(btn1_btn2_btn3, KC_BTN3),
    COMBO(dot_ent_drgscrl, DRGSCRL),
};
