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
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include <stdio.h>

#define KEYLOGGER_LENGTH 63
char keylog_str[KEYLOGGER_LENGTH+1];

void keyboard_post_init_user(void) {
  /* rgblight_setrgb(RGB_RED); */
  debug_enable = true;
  debug_matrix = true;

  for (uint8_t i = 0; i < KEYLOGGER_LENGTH; i++) {
    keylog_str[i] = ' ';
  }
}

// FIXME backspace can't toggle layer because deleting a lot (but backspace can be modified by gui or alt!)
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT_split_3x6_3(KC_GRV, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSLS, LT(2,KC_ESC), LALT_T(KC_A), LCTL_T(KC_S), LGUI_T(KC_D), LSFT_T(KC_F), KC_G, KC_H, RSFT_T(KC_J), RGUI_T(KC_K), RCTL_T(KC_L), RALT_T(KC_SCLN), KC_QUOT, TT(3), KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, LT(3,KC_MINS), KC_BSPC, LT(1,KC_TAB), LT(2,KC_ENT), KC_BSPC, LT(1,KC_SPC), KC_DEL),
	[1] = LAYOUT_split_3x6_3(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_CIRC, KC_AMPR, KC_ASTR, KC_MINS, KC_EQL, KC_TRNS, KC_TILD, KC_EXLM, KC_AT, KC_HASH, LSFT_T(KC_DLR), KC_PERC, KC_EQL, KC_LPRN, KC_SCLN, KC_RPRN, KC_QUES, KC_TRNS, DT_UP, DT_DOWN, DT_PRNT, KC_NO, KC_NO, KC_NO, KC_MINS, KC_LBRC, KC_COLN, KC_RBRC, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
	[2] = LAYOUT_split_3x6_3(SGUI(KC_3), SGUI(KC_4), KC_AT, KC_HASH, KC_DLR, KC_PERC, KC_CIRC, KC_P7, KC_P8, KC_P9, KC_PSLS, KC_NUHS, KC_NO, KC_NO, KC_LPRN, KC_SCLN, KC_RPRN, KC_NO, KC_EQL, KC_P4, KC_P5, KC_P6, KC_PAST, KC_NO, KC_NO, KC_NO, KC_LBRC, KC_COLN, KC_RBRC, KC_NO, KC_MINS, KC_P1, KC_P2, KC_P3, KC_COMM, KC_DOT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_P0, KC_TRNS),
	[3] = LAYOUT_split_3x6_3(KC_F1, KC_F2, KC_UP, KC_F3, KC_F4, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, LSFT_T(KC_ESC), KC_LEFT, KC_DOWN, KC_RGHT, KC_LGUI, KC_F5, RGB_TOG, KC_RSFT, KC_RGUI, KC_RCTL, KC_RALT, KC_NO, KC_TRNS, KC_HOME, KC_PGDN, KC_PGUP, KC_END, KC_NO, RGB_MOD, RGB_RMOD, RGB_HUI, RGB_HUD, RGB_VAI, RGB_VAD, KC_SPC, KC_TAB, KC_ENT, KC_NO, KC_NO, KC_NO),
	[4] = LAYOUT_split_3x6_3(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_ESC, KC_LALT, KC_LCTL, KC_LGUI, KC_LSFT, KC_NO, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_HOME, KC_PGDN, KC_PGUP, KC_END, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS)
};

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master()) {
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  }
  return rotation;
}

void oled_render_layer_state(void) {
    oled_write_P(PSTR("L: "), false);
    /* oled_write_P(layer_state, false); */
    switch (layer_state) {
        case 0:
            oled_write_P(PSTR("Default"), false);
            break;
        case 2:
            oled_write_P(PSTR("Symbols"), false);
            break;
        case 4:
            oled_write_P(PSTR("Num"), false);
            break;
        case 8:
            oled_write_P(PSTR("Left Nav/Fn"), false);
            break;
        case 16:
            oled_write_P(PSTR("Vim Nav"), false);
            break;
    }
    oled_write_P(PSTR(" "), false);
    const uint8_t mods = get_mods();
    if (mods & MOD_MASK_SHIFT) {
      oled_write_P(PSTR("Shft"), true);
    }
    if (mods & MOD_MASK_ALT) {
      oled_write_P(PSTR("Alt"), true);
    }
    if (mods & MOD_MASK_GUI) {
      oled_write_P(PSTR("Cmd"), true);
    }
    if (mods & MOD_MASK_CTRL) {
      oled_write_P(PSTR("Ctrl"), true);
    }
    oled_write_P(PSTR("\n"), false);
}


static const char PROGMEM code_to_name[0xFF] = {
//   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',  // 0x
    'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2',  // 1x
    '3', '4', '5', '6', '7', '8', '9', '0', 'R', 'E', 'B', 'T', '_', '-', '=', '[',  // 2x
    ']','\\', '#', ';','\'', '`', ',', '.', '/', 'C', ' ', ' ', ' ', ' ', ' ', ' ',  // 3x
    ' ', ' ', ' ', ' ', ' ', ' ', 'P', 'S', ' ', ' ', 'H', ' ', ' ', ' ', ' ', 'R',  // 4x
    'L', 'D', 'U', ' ', ' ', ' ', ' ', ' ', ' ', '1', '2', '3', '4', '5', '6', '7',  // 5x
    '8', '9', '0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // 6x
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // 7x
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // 8x
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // 9x
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // Ax
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // Bx
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // Cx
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // Dx
    'C', 'S', 'A', 'C', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // Ex
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '        // Fx
};

void set_keylog(uint16_t keycode, keyrecord_t *record) {
  if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
      (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) { keycode = keycode & 0xFF; }
  if (keycode < 0xFF) {
    for (uint8_t i = 1; i < KEYLOGGER_LENGTH; i++) {
      keylog_str[i-1] = keylog_str[i];
    }
    keylog_str[KEYLOGGER_LENGTH-1] = pgm_read_byte(&code_to_name[keycode]);
  }

  // update keylog
  /* snprintf(keylog_str, sizeof(keylog_str), "%dx%d, k%2d : %c", */
  /*          record->event.key.row, record->event.key.col, */
  /*          keycode, name); */
}

void oled_render_keylog(void) {
    oled_write(keylog_str, false);
}

void render_bootmagic_status(bool status) {
    /* Show Ctrl-Gui Swap options */
    static const char PROGMEM logo[][2][3] = {
        {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
        {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
    };
    if (status) {
        oled_write_ln_P(logo[0][0], false);
        oled_write_ln_P(logo[0][1], false);
    } else {
        oled_write_ln_P(logo[1][0], false);
        oled_write_ln_P(logo[1][1], false);
    }
}

void oled_render_logo(void) {
    static const char PROGMEM crkbd_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
        0};
    oled_write_P(crkbd_logo, false);
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        oled_render_layer_state();
        oled_render_keylog();
    } else {
        oled_render_logo();
    }
    return false;
}

#endif // OLED_ENABLE

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  // TODO if we're in the tab-as-tab layer, apply whichever modifier is toggled at the top
  if (record->event.pressed) {
    #ifdef OLED_ENABLE
    set_keylog(keycode, record);
    #endif
/*     const uint8_t mods = get_mods(); */
/*     // if alt (and not cmd or ctrl): could be a special character */
/*     if ( (mods & MOD_MASK_ALT) && */
/*         !(mods & MOD_MASK_GUI) && */
/*         !(mods & MOD_MASK_CTRL)) { */
/*       uint8_t i = 0; */
/*       while (i < NUM_ALT_SYMBOLS) { */
/*         if (keycode == alt_symbols[i]) { */
/*           break; */
/*         } */
/*         i++; */
/*       } */
/*       if (i == NUM_ALT_SYMBOLS) { */
/*         return true; */
/*       } */
/*       // otherwise just pass normal letter through */
/*          /1* find(begin(alt_symbols), end(alt_symbols), keycode) == end(alt_symbols)) { *1/ */
/*          /1* (keycode == AUML || keycode == OUML || keycode == UUML || keycode == EURO)) { *1/ */
/*       // follow https://getreuer.info/posts/keyboards/macros/index.html */
/*       del_mods(MOD_MASK_ALT); */
/*       switch (keycode) { */
/*         case KC_SCLN: */
/*           SEND_STRING(SS_RALT("u")"o"); */
/*           break; */
/*         case KC_QUOTE: */
/*           SEND_STRING(SS_RALT("u")"a"); */
/*           break; */
/*         case KC_BSLS: */
/*           SEND_STRING(SS_RALT("u")"u"); */
/*           break; */
/*         case KC_E: */
/*           SEND_STRING(SS_RSFT(SS_RALT("@"))); */
/*           break; */
/*         case KC_D: */
/*           // TODO unicode? https://github.com/qmk/qmk_firmware/blob/master/docs/feature_unicode.md */
/*           SEND_STRING("¯\\_(ツ)_/¯"); */
/*           break; */
/*       } */
/*       set_mods(mods); */
/*       return false; */
/*     } */
  }
  return true;
}
