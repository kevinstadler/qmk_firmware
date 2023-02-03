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

#define KEYLOGGER_LENGTH 70
char keylog_str[KEYLOGGER_LENGTH+1];

void keyboard_post_init_user(void) {
  /* rgblight_setrgb(RGB_RED); */
  debug_enable = true;
  debug_matrix = true;

  for (uint8_t i = 0; i < KEYLOGGER_LENGTH; i++) {
    keylog_str[i] = ' ';
  }

  rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
}

bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        case KC_MINS:
            add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
            return true;
        case KC_ENTER:
        case KC_ESC:
          return false;
    }
    return true;
}

enum layers {
  _BASE = 0,
  _SYM,
  /* _NUM, */
  _RNAV,
  _LNAV,
  _CMDTAB
};

/* https://github.com/samhocevar-forks/qmk-firmware/blob/master/docs/feature_tap_dance.md#example-5-using-tap-dance-for-advanced-mod-tap-and-layer-tap-keys */
// tapdance keycodes
enum td_keycodes {
  LSHIFT_RIGHTPARENS // Our example key: `LALT` when held, `(` when tapped. Add additional keycodes for each tapdance.
};
// define a type containing as many tapdance states as you need
typedef enum {
  SINGLE_TAP,
  SINGLE_HOLD,
  DOUBLE_SINGLE_TAP
} td_state_t;
// create a global instance of the tapdance state type
static td_state_t td_state;
// declare your tapdance functions:
// function to determine the current tapdance state
int cur_dance (qk_tap_dance_state_t *state);
// `finished` and `reset` functions for each tapdance keycode
void shiftrp_finished (qk_tap_dance_state_t *state, void *user_data);
void shiftrp_reset (qk_tap_dance_state_t *state, void *user_data);

// determine the tapdance state to return
int cur_dance (qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted || !state->pressed) { return SINGLE_TAP; }
    else { return SINGLE_HOLD; }
  }
  if (state->count == 2) { return DOUBLE_SINGLE_TAP; }
  else { return 3; } // any number higher than the maximum state value you return above
}

// handle the possible states for each tapdance keycode you define:

void shiftrp_finished (qk_tap_dance_state_t *state, void *user_data) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP:
      register_code16(KC_RPRN);
      break;
    case SINGLE_HOLD:
      register_mods(MOD_BIT(KC_LSFT)); // for a layer-tap key, use `layer_on(_MY_LAYER)` here
      break;
    case DOUBLE_SINGLE_TAP: // allow nesting of 2 parens `((` within tapping term
      tap_code16(KC_RPRN);
      register_code16(KC_RPRN);
  }
}

void shiftrp_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      unregister_code16(KC_RPRN);
      break;
    case SINGLE_HOLD:
      unregister_mods(MOD_BIT(KC_LSFT)); // for a layer-tap key, use `layer_off(_MY_LAYER)` here
      break;
    case DOUBLE_SINGLE_TAP:
      unregister_code16(KC_RPRN);
  }
}

// define `ACTION_TAP_DANCE_FN_ADVANCED()` for each tapdance keycode, passing in `finished` and `reset` functions
qk_tap_dance_action_t tap_dance_actions[] = {
  [LSHIFT_RIGHTPARENS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, shiftrp_finished, shiftrp_reset)
};


/* const key_override_t coln_key_override = */
/*     ko_make_basic(MOD_MASK_SHIFT, KC_MINS, KC_ASTR); // Shift - is * */

/* const key_override_t** key_overrides = (const key_override_t*[]){ */
/*     &coln_key_override, */
/*     NULL */
/* }; */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT_split_3x6_3(KC_GRV, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_NO, KC_ESC, LCTL_T(KC_A), LALT_T(KC_S), LT(4,KC_D), LSFT_T(KC_F), KC_G, KC_H, RSFT_T(KC_J), LT(4,KC_K), RALT_T(KC_L), KC_SCLN, KC_QUOT, TT(3), KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, TT(3), KC_BSPC, LT(1,KC_TAB), LT(2,KC_ENT), LT(2,KC_ENT), LT(1,KC_SPC), KC_BSPC),
	[1] = LAYOUT_split_3x6_3(KC_TRNS, KC_NO, KC_LBRC, KC_UNDS, KC_RBRC, KC_COMM, KC_CIRC, KC_7, KC_8, KC_9, KC_EQL, KC_NO, DT_UP, LCTL_T(KC_BSLS), KC_LPRN, KC_MINS, SC_RSPC, KC_DOT, KC_PLUS, RSFT_T(KC_4), KC_5, KC_6, KC_TRNS, KC_BSLS, DT_DOWN, DT_PRNT, KC_LCBR, KC_NO, KC_RCBR, KC_NO, KC_NO, KC_1, KC_2, KC_3, KC_TRNS, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_SPC, KC_0, KC_BSPC),
	[2] = LAYOUT_split_3x6_3(KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_ESC, KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, KC_NO, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, SGUI(KC_3), SGUI(KC_4), KC_NO, KC_HOME, KC_PGDN, KC_PGUP, KC_END, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_TRNS, KC_TRNS),
	[3] = LAYOUT_split_3x6_3(KC_F14, KC_F15, KC_NO, KC_UP, KC_DEL, KC_NO, KC_MPRV, KC_MPLY, KC_MNXT, 0x7F, 0x81, 0x80, KC_TRNS, KC_LCTL, KC_LEFT, LT(4,KC_DOWN), LSFT_T(KC_RGHT), KC_TAB, RGB_TOG, KC_RSFT, KC_RGUI, KC_RALT, KC_RCTL, KC_NO, KC_TRNS, KC_HOME, KC_PGDN, KC_PGUP, KC_END, KC_NO, RGB_RMOD, RGB_MOD, RGB_HUD, RGB_HUI, RGB_VAD, KC_TRNS, KC_BSPC, KC_SPC, KC_ENT, KC_TRNS, KC_TRNS, KC_TRNS),
	[4] = LAYOUT_split_3x6_3(KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_SPC, KC_TAB, KC_TRNS, KC_TRNS, KC_0, KC_TRNS)
};

const uint16_t PROGMEM capslock_combo[] = {LT(4,KC_D), LT(4,KC_K), COMBO_END};
combo_t key_combos[COMBO_COUNT] = {
    COMBO(capslock_combo, KC_CAPS)
};

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master()) {
    return OLED_ROTATION_90;  // flips the display 180 degrees if offhand
  }
  return OLED_ROTATION_270;
  /* return rotation; */
}

void oled_render_layer_state(void) {
    // TODO don't get highest layer, but check if any of the layers is on but don't render Cmd layer unless it's the only thing that's active
    switch (get_highest_layer(layer_state)) {
    /* switch (layer_state) { */
        case _BASE:
            oled_write_P(PSTR("Alpha"), true);
            break;
        case _SYM:
            oled_write_ln_P(PSTR("Sym"), true);
            break;
        /* case (1 << _NUM): */
        /*     oled_write_ln_P(PSTR("Num"), true); */
        /*     break; */
        case _RNAV:
            oled_write_ln_P(PSTR("Nav"), true);
            break;
        case _LNAV:
            oled_write_ln_P(PSTR("LNav"), true);
            break;
        case _CMDTAB:
            oled_write_ln_P(PSTR("Cmd"), true);
            break;
    }
    if (is_caps_word_on()) {
      oled_write_P(PSTR("CAPS"), true);
    } else {
      const uint8_t mods = get_mods();
      if (mods & MOD_MASK_SHIFT) {
        oled_write_P(PSTR("Sh"), true);
      }
      if (mods & MOD_MASK_ALT) {
        oled_write_P(PSTR("Al"), true);
      }
      if (mods & MOD_MASK_GUI) {
        oled_write_P(PSTR("Cm"), true);
      }
      if (mods & MOD_MASK_CTRL) {
        oled_write_P(PSTR("Ct"), true);
      }
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

/* static layer_state_t prev_layer_state; */
static bool cmdOn = false;
layer_state_t layer_state_set_user(layer_state_t state) {
	/* if (state == (1 << _CMDTAB)) { */
  if (IS_LAYER_ON_STATE(state, _CMDTAB)) {
		register_mods(MOD_LGUI);
		cmdOn = true;
	} else if (cmdOn) {//prev_layer_state == (1 << _CMDTAB)) {
		unregister_mods(MOD_LGUI);
	  cmdOn = false;
	}
	/* prev_layer_state = state; */
	return state;
}

bool led_update_user(led_t led_state) {
  return false; // don't run keyboard-level led code
}

uint8_t hue;
uint8_t brightness;

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    hue = rgblight_get_hue() + 40 + rand() % (255-80);
    brightness = 190;
    for (uint8_t i = 0; i < RGBLED_NUM; i++) {
      sethsv(hue, 255, brightness, &led[i]);
    }
    rgblight_set();
  }
  /* rgblight_sethsv_master(newHue, 255, 150); */
  /* rgblight_sethsv_slave(newHue, 255, 150); */
}

void matrix_scan_user(void) {
  if (brightness > 0) {
    brightness--;
    for (uint8_t i = 0; i < RGBLED_NUM; i++) {
      sethsv(hue, 255, brightness, &led[i]);
    }
    rgblight_set();
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  // TODO if we're in the tab-as-tab layer, apply whichever modifier is toggled at the top

//  https://github.com/qmk/qmk_firmware/blob/master/docs/feature_advanced_keycodes.md#shift--backspace-for-delete-idshift-backspace-for-delete
  #ifdef OLED_ENABLE
  if (record->event.pressed) {
    set_keylog(keycode, record);
  }
  #endif
  const uint8_t mods = get_mods();
  if (keycode == KC_BSPC) {
    static bool delkey_registered;
    if (record->event.pressed) {
      if (mods & MOD_MASK_SHIFT) {
        del_mods(MOD_MASK_SHIFT);
        register_code(KC_DEL);
        delkey_registered = true;
        // Reapplying modifier state so that the held shift key(s)
        // still work even after having tapped the Backspace/Delete key.
        set_mods(mods);
        return false;
      }
    } else {
      // In case KC_DEL is still being sent even after the release of KC_BSPC
      if (delkey_registered) {
        unregister_code(KC_DEL);
        delkey_registered = false;
        return false;
      }
    }

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
