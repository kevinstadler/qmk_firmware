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

/* Select hand configuration */

#define MASTER_LEFT
#define SPLIT_USB_DETECT
// #define MASTER_RIGHT
// #define EE_HANDS

#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD

#define IGNORE_MOD_TAP_INTERRUPT
#define TAPPING_FORCE_HOLD
#define TAPPING_TERM 190 // was 200
#define TAPPING_TOGGLE 1

#define COMBO_COUNT 1

#ifdef RGBLIGHT_ENABLE
    #define SPLIT_LAYER_STATE_ENABLE
    #define RGBLIGHT_SPLIT
    /* #define RGBLIGHT_EFFECT_BREATHING */
    /* #define RGBLIGHT_EFFECT_RAINBOW_MOOD */
    /* #define RGBLIGHT_EFFECT_RAINBOW_SWIRL */
    /* #define RGBLIGHT_EFFECT_KNIGHT */
    #undef RGBLIGHT_DEFAULT_MODE
    /* #define RGBLIGHT_EFFECT_CHRISTMAS */
    /* #define RGBLIGHT_EFFECT_STATIC_GRADIENT */
    /* #define RGBLIGHT_EFFECT_RGB_TEST */
    /* #define RGBLIGHT_EFFECT_ALTERNATING */
    /* #define RGBLIGHT_EFFECT_TWINKLE */
    #define RGBLIGHT_LIMIT_VAL 220
    #define RGBLIGHT_HUE_STEP 10
    #define RGBLIGHT_SAT_STEP 17
    #define RGBLIGHT_VAL_STEP 20
#endif

#define OLED_FONT_H "keyboards/crkbd/lib/glcdfont.c"
