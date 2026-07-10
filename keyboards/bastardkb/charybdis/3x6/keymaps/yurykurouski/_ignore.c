/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
calong with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _QWERTY 0
#define _LOWER 1
#define _RAISE 2
#define _ADJUST 3
#define _NUMPAD 4
#define _COLEMAK 5

#define DIM_BY 10
#define DIMMED(rgb_color) DIMMED_(rgb_color)
#define DIMMED_(r, g, b) ((r)/DIM_BY), ((g)/DIM_BY), ((b)/DIM_BY)

enum custom_keycodes {
    TD_ESC_CAPS,
    TAB_ESC,
    ENTER_TWO_PRESS,
    BACKSPACE_TWO_PRESS,
    REDO_TWO_PRESS,
    NUMPAD,
    SCROLL_MODE,
    TD_LANG_SWITCH,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_COLEMAK] = LAYOUT_split_3x6_3_ex2(
    //,--------------------------------------------------------------.                            ,--------------------------------------------------------------.
        KC_TAB,    KC_Q,    KC_W,    KC_F,    KC_P,    KC_G, TO(_QWERTY),                            KC_BRIU,    KC_J,    KC_L,    KC_U,    KC_Y,   KC_SCLN,  KC_BSPC,
    //|--------+--------+--------+--------+--------+--------+--------|                            |--------+--------+--------+--------+--------+--------+--------|
        KC_LSFT,    KC_A,    KC_R,    KC_S,    KC_T,    KC_D, KC_HOME,                            KC_END,    KC_H,    KC_N,    KC_E,    KC_I, KC_O, KC_QUOT,
    //|--------+--------+--------+--------+--------+--------+--------'                            `--------+--------+--------+--------+--------+--------+--------|
        KC_LOPT,   KC_Z,    KC_X,    KC_C,    KC_V,   KC_B,                                                  KC_K,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  TD(TD_ESC_CAPS),
    //|--------+--------+--------+--------+--------+--------+--------.                            .--------+--------+--------+--------+--------+--------+--------|
                                            KC_LGUI,  MO(_LOWER),  KC_SPC,                         KC_ENT, MO(_RAISE), KC_LCTL
                                        //`--------------------------'                            `--------------------------'
    ),

    [_QWERTY] = LAYOUT_split_3x6_3_ex2(
  //,--------------------------------------------------------------.                            ,--------------------------------------------------------------.
       KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T, KC_VOLU,                            LGUI(LSFT(KC_A)),    KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_BSPC,
  //|--------+--------+--------+--------+--------+--------+--------|                            |--------+--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G, KC_MUTE,                            KC_RIGHT,    KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------+--------'                            `--------+--------+--------+--------+--------+--------+--------|
      KC_LOPT,   KC_Z,    KC_X,    KC_C,    KC_V,   KC_B,                                                  KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  TD(TD_ESC_CAPS),
  //|--------+--------+--------+--------+--------+--------+--------.                            .--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI,  MO(_LOWER),  KC_SPC,                         KC_ENT, MO(_RAISE), KC_LCTL
                                      //`--------------------------'                            `--------------------------'
  ),

    [_LOWER] = LAYOUT_split_3x6_3_ex2(
  //,--------------------------------------------------------------.                            ,--------------------------------------------------------------.
       KC_TAB,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5, KC_MNXT,                            LCTL(KC_SPC),    KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_BSPC,
      KC_LSFT, LALT(KC_1), LALT(KC_2), LALT(KC_3), LALT(KC_4), RCTL(KC_UP), KC_MPRV,            LCTL(LOPT(KC_SPC)),    XXXXXXX, KC_UP,   XXXXXXX,XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|                            |--------+--------+--------+--------+--------+--------+--------|
  //|--------+--------+--------+--------+--------+--------+--------'                            `--------+--------+--------+--------+--------+--------+--------|
      KC_LCTL, LGUI(LSFT(KC_4)), XXXXXXX, KC_C, XXXXXXX, RCTL(KC_DOWN),                                    KC_LEFT, KC_DOWN, KC_RIGHT, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------.                            ,--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, _______,  KC_MPLY,                           KC_ENT, _______, KC_RGUI
                                      //`--------------------------'                            `--------------------------'
  ),

    [_RAISE] = LAYOUT_split_3x6_3_ex2(
  //,--------------------------------------------------------------.  ,--------------------------------------------------------------.
       KC_TAB, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC, KC_LCTL,    TG(_NUMPAD), KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
      KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LALT,    KC_RALT, KC_MINS,  KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS,  KC_GRV,
  //|--------+--------+--------+--------+--------+--------+--------'  `--------+--------+--------+--------+--------+--------+--------|
      KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, KC_TILD,
  //|--------+--------+--------+--------+--------+--------+--------.  ,--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI,  MO(_ADJUST),  KC_SPC,     KC_ENT, _______, KC_RGUI
                                      //`--------------------------'  `--------------------------'
  ),

    [_ADJUST] = LAYOUT_split_3x6_3_ex2(
  //,--------------------------------------------------------------.  ,--------------------------------------------------------------.
      QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
      RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------'  `--------+--------+--------+--------+--------+--------+--------|
      RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------.  ,--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, _______,  KC_SPC,     KC_ENT, _______, KC_RGUI
                                      //`--------------------------'  `--------------------------'
  ),

   [_NUMPAD] = LAYOUT_split_3x6_3_ex2(
  //,--------------------------------------------------------------.  ,--------------------------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    TG(_NUMPAD), KC_7, KC_8, KC_9, KC_SLSH, XXXXXXX, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, KC_4, KC_5, KC_6, KC_ASTR, KC_PLUS, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------'  `--------+--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_1, KC_2, KC_3, KC_MINS, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------.  ,--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, _______,  KC_SPC,     KC_0, KC_DOT, KC_EQL
                                      //`--------------------------'  `--------------------------'
  )
};

void keyboard_post_init_user(void) {
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(HSV_OFF);
}

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_ESC_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_ESC, KC_CAPS),
};

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    led_t led_state = host_keyboard_led_state();

    if (led_state.caps_lock) {
        rgb_matrix_set_color(43, RGB_RED);
    } else {
        rgb_matrix_set_color(43, RGB_OFF);
    }

    switch (get_highest_layer(layer_state)) {
        case _NUMPAD:
            rgb_matrix_set_color(21, RGB_RED);

            rgb_matrix_set_color(23, RGB_GREEN);
            rgb_matrix_set_color(24, RGB_GREEN);
            rgb_matrix_set_color(25, RGB_GREEN);
            rgb_matrix_set_color(26, RGB_GREEN);
            rgb_matrix_set_color(27, RGB_GREEN);
            rgb_matrix_set_color(28, RGB_GREEN);
            rgb_matrix_set_color(29, RGB_GREEN);
            rgb_matrix_set_color(30, RGB_GREEN);
            rgb_matrix_set_color(31, RGB_PURPLE);
            rgb_matrix_set_color(32, RGB_GREEN);
            rgb_matrix_set_color(33, RGB_GREEN);
            rgb_matrix_set_color(34, RGB_GREEN);
            rgb_matrix_set_color(35, RGB_PURPLE);
            rgb_matrix_set_color(36, RGB_PURPLE);
            rgb_matrix_set_color(37, RGB_PURPLE);
            rgb_matrix_set_color(39, RGB_PURPLE);
            break;

        case _RAISE:
            rgb_matrix_set_color_all(DIMMED(RGB_GREEN));
            rgb_matrix_set_color(21, RGB_RED);

            break;
        case _LOWER:
            rgb_matrix_set_color_all(DIMMED(RGB_PURPLE));

            rgb_matrix_set_color(24, RGB_GREEN);
            rgb_matrix_set_color(28, RGB_GREEN);
            rgb_matrix_set_color(29, RGB_GREEN);
            rgb_matrix_set_color(32, RGB_GREEN);
            break;
        case _ADJUST:
            rgb_matrix_set_color_all(DIMMED(RGB_AZURE));
            break;
        case _COLEMAK:
            rgb_matrix_set_color_all(DIMMED(RGB_AZURE));
            break;

        default: //  for any other layers, or the default layer
            rgb_matrix_set_color_all(RGB_OFF);
            break;
    }

    return true;
}

const uint16_t PROGMEM tab_esc_combo[] = {KC_TAB, KC_Q, COMBO_END};
const uint16_t PROGMEM backspace_two_press_combo[] = {KC_Q, KC_W, COMBO_END};
const uint16_t PROGMEM enter_two_press_combo[] = {KC_A, KC_S, COMBO_END};
const uint16_t PROGMEM redo_two_press_combo[] = {KC_LOPT, KC_Z, COMBO_END};

combo_t key_combos[] = {
  [TAB_ESC] = COMBO(tab_esc_combo, KC_ESC),
  [ENTER_TWO_PRESS] = COMBO(enter_two_press_combo, KC_ENT),
  [BACKSPACE_TWO_PRESS] = COMBO(backspace_two_press_combo, KC_BSPC),
  [REDO_TWO_PRESS] = COMBO(redo_two_press_combo, LGUI(LSFT(KC_Z))),
};

static bool right_encoder_scroll_enabled = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_BRIU:
            if (record->event.pressed) {
                // Toggle the right encoder mode whenever KC_BRIU is pressed.
                right_encoder_scroll_enabled = !right_encoder_scroll_enabled;
                // Optionally add a debug print:
                // xprintf("Right encoder mode: %s\n", right_encoder_scroll_enabled ? "Scroll" : "Cursor");
            }
            // Prevent default brightness adjustment.
            return true;
        default:
            return true; // Process all other keycodes normally.
    }
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    // Check for the right encoder (assuming index 2 is the right encoder)
    if (index == 1) {
        if (right_encoder_scroll_enabled) {
            // Scroll Mode: scroll up or down
            if (clockwise) {
                tap_code(MS_WHLD);  // Scroll down
            } else {
                tap_code(MS_WHLU);  // Scroll up
            }
        } else {
            // Cursor Mode: move cursor left/right
            if (clockwise) {
                tap_code(KC_RGHT);
            } else {
                tap_code(KC_LEFT);
            }
        }
        return false; // Skip default encoder_map processing for the right encoder.
    }
    return true; // For other encoders, fall back to the encoder_map.
}

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_QWERTY] = {
        ENCODER_CCW_CW(KC_VOLD, KC_VOLU),
        ENCODER_CCW_CW(KC_VOLD, KC_VOLU),
        ENCODER_CCW_CW(KC_LEFT, KC_RIGHT)
    },
    [_COLEMAK] = {
        ENCODER_CCW_CW(KC_VOLD, KC_VOLU),
        ENCODER_CCW_CW(KC_VOLD, KC_VOLU),
        ENCODER_CCW_CW(KC_LEFT, KC_RIGHT)
    },
    [_LOWER] = {
        ENCODER_CCW_CW(KC_VOLD, KC_VOLU),
        ENCODER_CCW_CW(KC_BRID, KC_BRIU),
        ENCODER_CCW_CW(KC_UP, KC_DOWN)
    },
    [_RAISE] = {
        ENCODER_CCW_CW(KC_VOLD, KC_VOLU),
        ENCODER_CCW_CW(KC_VOLD, KC_VOLU),
        ENCODER_CCW_CW(KC_LEFT, KC_RIGHT)
    },
    [_ADJUST] = {
        ENCODER_CCW_CW(KC_VOLD, KC_VOLU),
        ENCODER_CCW_CW(KC_VOLD, KC_VOLU),
        ENCODER_CCW_CW(KC_LEFT, KC_RIGHT)
    },
    [_NUMPAD] = {
        ENCODER_CCW_CW(KC_VOLD, KC_VOLU),
        ENCODER_CCW_CW(KC_VOLD, KC_VOLU),
        ENCODER_CCW_CW(KC_LEFT, KC_RIGHT)
    },
};
#endif
