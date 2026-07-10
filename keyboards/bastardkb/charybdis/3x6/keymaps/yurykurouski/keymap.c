/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 * Copyright 2026 yurykurouski
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

#define _QWERTY 0
#define _LOWER 1
#define _RAISE 2
#define _NUMPAD 3
#define _POINTER 4



enum tap_dance_indexes {
    TD_ESC_CAPS,
};

enum combo_indexes {
    TAB_ESC,
    ENTER_TWO_PRESS,
    BACKSPACE_TWO_PRESS,
    REDO_TWO_PRESS,
    BROWSER_BACK,
    BROWSER_FORWARD,
};

enum custom_keycodes {
    V_SCRL = SAFE_RANGE,
    X_DRG,
    DOT_DRG,
    M_BTN1,
};

// Automatically enable sniping-mode on the pointer layer.
#define CHARYBDIS_AUTO_SNIPING_ON_LAYER _POINTER

static bool vertical_scroll_only = false;
static uint16_t x_drag_timer = 0;
static uint16_t dot_drag_timer = 0;
static uint16_t m_btn1_timer = 0;
static bool m_btn1_held = false;

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#endif     // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* QWERTY
     * ╭────────────────────────────────────────╮ ╭────────────────────────────────────────╮
     * │ Tab  │   Q  │   W  │   E  │   R  │   T  │ │   Y  │   U  │   I  │   O  │   P  │ Bspc │
     * ├──────┼──────┼──────┼──────┼──────┼──────┤ ├──────┼──────┼──────┼──────┼──────┼──────┤
     * │ Shift│   A  │   S  │   D  │   F  │   G  │ │   H  │   J  │   K  │   L  │   ;  │  '   │
     * ├──────┼──────┼──────┼──────┼──────┼──────┤ ├──────┼──────┼──────┼──────┼──────┼──────┤
     * │Opt/Pt│   Z  │ X/Drg│   C  │   V  │   B  │ │   N  │ M/Btn│   ,  │ ./Drg│ / /Pt│EscTD │
     * ╰──────┴──────┴──────┼──────┼──────┼──────┤ ├──────┼──────┼──────┼──────┴──────┴──────;╯
     *                      │ GUI  │LOWER │ Space│ │RAISE │Ent/Ct│
     *                      ╰──────┴──────┴──────╯ ╰──────┴──────╯
     */
    [_QWERTY] = LAYOUT(
        KC_TAB,  KC_Q,  KC_W,  KC_E,  KC_R,  KC_T,                                             KC_Y,  KC_U,  KC_I,  KC_O,  KC_P,  KC_BSPC,
        KC_LSFT, LGUI_T(KC_A), LALT_T(KC_S), LCTL_T(KC_D), LSFT_T(KC_F), KC_G,                KC_H, RSFT_T(KC_J), RCTL_T(KC_K), LALT_T(KC_L), RGUI_T(KC_SCLN), KC_QUOT,
        KC_LOPT, LT(_POINTER, KC_Z), X_DRG,   KC_C, KC_V, KC_B,                 KC_N,  M_BTN1, KC_COMM, DOT_DRG, LT(_POINTER, KC_SLSH), TD(TD_ESC_CAPS),
                        KC_LGUI, MO(_LOWER), KC_SPC,                                           LT(_RAISE, KC_BSPC), LCTL_T(KC_ENT)
    ),

    /* LOWER
     * ╭────────────────────────────────────────╮ ╭────────────────────────────────────────╮
     * │ Tab  │   1  │   2  │   3  │   4  │   5  │ │   6  │   7  │   8  │   9  │   0  │ Bspc │
     * ├──────┼──────┼──────┼──────┼──────┼──────┤ ├──────┼──────┼──────┼──────┼──────┼──────┤
     * │ Shift│ Alt1 │ Alt2 │ Alt3 │ Alt4 │ Ct+Up│ │Brt Dn│  Up  │Brt Up│      │      │      │
     * ├──────┼──────┼──────┼──────┼──────┼──────┤ ├──────┼──────┼──────┼──────┼──────┼──────┤
     * │ Ctrl │ScrnSh│Vol Dn│ Mute │Vol Up│ Ct+Dn│ │ Left │ Down │ Right│      │      │      │
     * ╰──────┴──────┴──────┼──────┼──────┼──────┤ ├──────┼──────┼──────┼──────┴──────┴──────╯
     *                      │ GUI  │      │ Play │ │      │ZoomMt│
     *                      ╰──────┴──────┴──────╯ ╰──────┴──────╯
     */
    [_LOWER] = LAYOUT(
        KC_TAB,  KC_1,  KC_2,  KC_3,  KC_4,  KC_5,                                             KC_6,  KC_7,  KC_8,  KC_9,  KC_0,  KC_BSPC,
        KC_LSFT, LALT(KC_1), LALT(KC_2), LALT(KC_3), LALT(KC_4), RCTL(KC_UP),                 KC_BRID, KC_UP, KC_BRIU, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_LCTL, LGUI(LSFT(KC_4)), KC_VOLD, KC_MUTE, KC_VOLU, RCTL(KC_DOWN),                  KC_LEFT, KC_DOWN, KC_RIGHT, XXXXXXX, XXXXXXX, XXXXXXX,
                        KC_LGUI, _______, KC_MPLY,                                             _______, LGUI(LSFT(KC_A))
    ),

    /* RAISE
     * ╭────────────────────────────────────────╮ ╭────────────────────────────────────────╮
     * │ Tab  │   !  │   @  │   #  │   $  │   %  │ │   ^  │   &  │   *  │   (  │   )  │ Bspc │
     * ├──────┼──────┼──────┼──────┼──────┼──────┤ ├──────┼──────┼──────┼──────┼──────┼──────┤
     * │ Ctrl │NumPad│      │      │      │      │ │   -  │   =  │   [  │   ]  │   \  │  `   │
     * ├──────┼──────┼──────┼──────┼──────┼──────┤ ├──────┼──────┼──────┼──────┼──────┼──────┤
     * │ Shift│      │      │      │      │      │ │   _  │   +  │   {  │   }  │   |  │  ~   │
     * ╰──────┴──────┴──────┼──────┼──────┼──────┤ ├──────┼──────┼──────┼──────┴──────┴──────╯
     *                      │ GUI  │      │ Space│ │      │      │
     *                      ╰──────┴──────┴──────╯ ╰──────┴──────╯
     */
    [_RAISE] = LAYOUT(
        KC_TAB,  KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC,                                     KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
        KC_LCTL, TG(_NUMPAD), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                             KC_MINS, KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS, KC_GRV,
        KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                 KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, KC_TILD,
                        KC_LGUI, _______, KC_SPC,                                              _______, _______
    ),

    /* NUMPAD
     * ╭────────────────────────────────────────╮ ╭────────────────────────────────────────╮
     * │      │      │      │      │      │      │ │   7  │   8  │   9  │   /  │      │ Bspc │
     * ├──────┼──────┼──────┼──────┼──────┼──────┤ ├──────┼──────┼──────┼──────┼──────┼──────┤
     * │      │NumPad│      │      │      │      │ │   4  │   5  │   6  │   *  │   +  │      │
     * ├──────┼──────┼──────┼──────┼──────┼──────┤ ├──────┼──────┼──────┼──────┼──────┼──────┤
     * │      │      │      │      │      │      │ │   1  │   2  │   3  │   -  │      │      │
     * ╰──────┴──────┴──────┼──────┼──────┼──────┤ ├──────┼──────┼──────┼──────┴──────┴──────╯
     *                      │ GUI  │      │ Space│ │   0  │   .  │
     *                      ╰──────┴──────┴──────╯ ╰──────┴──────╯
     */
    [_NUMPAD] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                 KC_7,  KC_8,  KC_9,  KC_SLSH, XXXXXXX, KC_BSPC,
        XXXXXXX, TG(_NUMPAD), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                             KC_4,  KC_5,  KC_6,  KC_ASTR, KC_PLUS, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                 KC_1,  KC_2,  KC_3,  KC_MINS, XXXXXXX, XXXXXXX,
                        KC_LGUI, _______, KC_SPC,                                              KC_0,  KC_DOT
    ),

    /* POINTER
     * ╭────────────────────────────────────────╮ ╭────────────────────────────────────────╮
     * │ Boot │ EEClr│      │      │DPIMod│ SDMod│ │ SDMod│DPIMod│      │ EEClr│ Boot │      │
     * ├──────┼──────┼──────┼──────┼──────┼──────┤ ├──────┼──────┼──────┼──────┼──────┼──────┤
     * │      │ GUI  │ Alt  │ Ctrl │ Shift│      │ │      │ Shift│ Ctrl │ Alt  │ GUI  │      │
     * ├──────┼──────┼──────┼──────┼──────┼──────┤ ├──────┼──────┼──────┼──────┼──────┼──────┤
     * │      │      │DrgScl│V_Scrl│      │      │ │ Btn3 │ Btn1 │ Btn2 │DrgScl│      │      │
     * ╰──────┴──────┴──────┼──────┼──────┼──────┤ ├──────┼──────┼──────┼──────┴──────┴──────╯
     *                      │ Btn1 │ Btn2 │ Btn3 │ │ Btn3 │ Btn1 │
     *                      ╰──────┴──────┴──────╯ ╰──────┴──────╯
     */
    [_POINTER] = LAYOUT(
        QK_BOOT, EE_CLR,  XXXXXXX, XXXXXXX, DPI_MOD, S_D_MOD,                                 S_D_MOD, DPI_MOD, XXXXXXX, EE_CLR,  QK_BOOT,  XXXXXXX,
        XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                                 XXXXXXX, KC_RSFT, KC_RCTL, KC_LALT, KC_RGUI,  XXXXXXX,
        XXXXXXX, _______, DRGSCRL, V_SCRL,  XXXXXXX, XXXXXXX,                           KC_BTN3, KC_BTN1, KC_BTN2, DRGSCRL, _______,  XXXXXXX,
                        KC_BTN1, KC_BTN2, KC_BTN3,                                             KC_BTN3, KC_BTN1
    ),
};
// clang-format on



// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_ESC_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_ESC, KC_CAPS),
};

const uint16_t PROGMEM tab_esc_combo[]             = {KC_TAB, KC_Q, COMBO_END};
const uint16_t PROGMEM backspace_two_press_combo[] = {KC_W, KC_E, COMBO_END};
const uint16_t PROGMEM enter_two_press_combo[]     = {KC_S, KC_D, COMBO_END};
const uint16_t PROGMEM redo_two_press_combo[]      = {KC_LOPT, LT(_POINTER, KC_Z), COMBO_END};
const uint16_t PROGMEM browser_back_combo[]        = {KC_A, KC_S, COMBO_END};
const uint16_t PROGMEM browser_forward_combo[]     = {KC_D, KC_F, COMBO_END};

combo_t key_combos[] = {
    [TAB_ESC]             = COMBO(tab_esc_combo, KC_ESC),
    [ENTER_TWO_PRESS]     = COMBO(enter_two_press_combo, KC_ENT),
    [BACKSPACE_TWO_PRESS] = COMBO(backspace_two_press_combo, KC_BSPC),
    [REDO_TWO_PRESS]      = COMBO(redo_two_press_combo, LGUI(LSFT(KC_Z))),
    [BROWSER_BACK]        = COMBO(browser_back_combo, LGUI(KC_LBRC)),
    [BROWSER_FORWARD]     = COMBO(browser_forward_combo, LGUI(KC_RBRC)),
};


#ifdef POINTING_DEVICE_ENABLE
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
    if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD || abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(_POINTER);
        }
        auto_pointer_layer_timer = timer_read();
    }
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

    if (vertical_scroll_only) {
        mouse_report.h = 0;
        mouse_report.x = 0;
    }

    return mouse_report;
}

void matrix_scan_user(void) {
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
    if (auto_pointer_layer_timer != 0 && TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(_POINTER);
    }
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

    if (m_btn1_timer != 0 && !m_btn1_held && timer_elapsed(m_btn1_timer) >= TAPPING_TERM) {
        m_btn1_held = true;
        register_code16(KC_BTN1);
    }
}

#    ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, CHARYBDIS_AUTO_SNIPING_ON_LAYER));
    return state;
}
#    endif // CHARYBDIS_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLE

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case V_SCRL:
            charybdis_set_pointer_dragscroll_enabled(record->event.pressed);
            vertical_scroll_only = record->event.pressed;
            return false;
        case X_DRG:
            if (record->event.pressed) {
                x_drag_timer = timer_read();
                charybdis_set_pointer_dragscroll_enabled(true);
            } else {
                charybdis_set_pointer_dragscroll_enabled(false);
                if (x_drag_timer != 0 && timer_elapsed(x_drag_timer) < TAPPING_TERM) {
                    tap_code16(KC_X);
                }
                x_drag_timer = 0;
            }
            return false;
        case DOT_DRG:
            if (record->event.pressed) {
                dot_drag_timer = timer_read();
                charybdis_set_pointer_dragscroll_enabled(true);
            } else {
                charybdis_set_pointer_dragscroll_enabled(false);
                if (dot_drag_timer != 0 && timer_elapsed(dot_drag_timer) < TAPPING_TERM) {
                    tap_code16(KC_DOT);
                }
                dot_drag_timer = 0;
            }
            return false;
        case M_BTN1:
            if (record->event.pressed) {
                m_btn1_timer = timer_read();
                m_btn1_held = false;
            } else {
                if (m_btn1_held) {
                    unregister_code16(KC_BTN1);
                    m_btn1_held = false;
                } else {
                    if (m_btn1_timer != 0 && timer_elapsed(m_btn1_timer) < TAPPING_TERM) {
                        tap_code16(KC_M);
                    }
                }
                m_btn1_timer = 0;
            }
            return false;
    }
    return true;
}
