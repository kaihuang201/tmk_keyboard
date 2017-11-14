/*
Copyright 2012,2013 Jun Wako <wakojun@gmail.com>

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
#include <stdint.h>
#include <stdbool.h>
#include "keycode.h"
#include "action.h"
#include "action_macro.h"
#include "host.h"
#include "debug.h"
#include "print.h"
#include "keymap.h"


/* translates key to keycode */
const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* QWERTY base layer
     * ,-----------------------------------------------------------.
     * |  `|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|  \|Del|
     * |-----------------------------------------------------------|
     * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]| BS  |
     * |-----------------------------------------------------------|
     * |Esc   |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|   Enter|
     * |-----------------------------------------------------------|
     * |Shift|Num|  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|Shift|Fn2|
     * |-----------------------------------------------------------|
     * |Ctl | Sys| Alt|         Space          | Alt| Fn0| App| Ctl|
     * `-----------------------------------------------------------'
     */
    [0] =
    {
        {   KC_GRV,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,  KC_EQL, KC_BSLS },
        {   KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_LBRC, KC_RBRC, KC_BSPC },
        {   KC_FN3,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,   KC_NO,  KC_ENT },
        {   KC_FN5,  KC_FN1,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_FN2,  KC_FN6 },
        {  KC_LCTL, KC_LGUI, KC_LALT,   KC_NO,   KC_NO,  KC_SPC,   KC_NO,   KC_NO,   KC_NO,  KC_DEL,  KC_FN0, KC_RALT,  KC_APP, KC_RCTL }
    },


    /* Dvorak base layer */
    /*
    [1] =
    {
        {   KC_GRV,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_LBRC, KC_RBRC, KC_BSLS },
        {   KC_TAB, KC_QUOT, KC_COMM,  KC_DOT,    KC_P,    KC_Y,    KC_F,    KC_G,    KC_C,    KC_R,    KC_L, KC_SLSH,  KC_EQL, KC_BSPC },
        {   KC_FN3,    KC_A,    KC_O,    KC_E,    KC_U,    KC_I,    KC_D,    KC_H,    KC_T,    KC_N,    KC_S, KC_MINS,   KC_NO,  KC_ENT },
        {   KC_FN5,  KC_FN1, KC_SCLN,    KC_Q,    KC_J,    KC_K,    KC_X,    KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,  KC_FN2,  KC_FN6 },
        {  KC_LCTL, KC_LGUI, KC_LALT,   KC_NO,   KC_NO,  KC_SPC,   KC_NO,   KC_NO,   KC_NO,  KC_DEL,  KC_FN0, KC_RALT,  KC_APP, KC_RCTL }
    },
    //*/

    /* Carpalx QFMLWY base layer
     * ,-----------------------------------------------------------.
     * |  `|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|  \|Del|
     * |-----------------------------------------------------------|
     * |Tab  |  Q|  F|  M|  L|  W|  Y|  U|  O|  B|  J|  [|  ]| BS  |
     * |-----------------------------------------------------------|
     * |Esc   |  D|  S|  T|  N|  R|  I|  A|  E|  H|  ;|  '|   Enter|
     * |-----------------------------------------------------------|
     * |Shift|Num|  Z|  V|  G|  C|  X|  P|  K|  ,|  .|  /|Shift|Fn2|
     * |-----------------------------------------------------------|
     * |Ctl | Sys| Alt|         Space          | Alt| Fn0| App| Ctl|
     * `-----------------------------------------------------------'
     */
    //*
    [1] =
    {
        {   KC_GRV,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,  KC_EQL, KC_BSLS },
        {   KC_TAB,    KC_Q,    KC_F,    KC_M,    KC_L,    KC_W,    KC_Y,    KC_U,    KC_O,    KC_B,    KC_J, KC_LBRC, KC_RBRC, KC_BSPC },
        {   KC_FN3,    KC_D,    KC_S,    KC_T,    KC_N,    KC_R,    KC_I,    KC_A,    KC_E,    KC_H, KC_SCLN, KC_QUOT,   KC_NO,  KC_ENT },
        {   KC_FN5,  KC_FN1,    KC_Z,    KC_V,    KC_G,    KC_C,    KC_X,    KC_P,    KC_K, KC_COMM,  KC_DOT, KC_SLSH,  KC_FN2,  KC_FN6 },
        {  KC_LCTL, KC_LGUI, KC_LALT,   KC_NO,   KC_NO,  KC_SPC,   KC_NO,   KC_NO,   KC_NO,  KC_DEL,  KC_FN0, KC_RALT,  KC_APP, KC_RCTL }
    },
    //*/



    [2] =
    /* Workman base layer */
    {
        {   KC_GRV,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,  KC_EQL, KC_BSLS },
        {   KC_TAB,    KC_Q,    KC_D,    KC_R,    KC_W,    KC_B,    KC_J,    KC_F,    KC_U,    KC_P, KC_SCLN, KC_LBRC, KC_RBRC, KC_BSPC },
        {   KC_FN3,    KC_A,    KC_S,    KC_H,    KC_T,    KC_G,    KC_Y,    KC_N,    KC_E,    KC_O,    KC_I, KC_QUOT,   KC_NO,  KC_ENT },
        {   KC_FN5,  KC_FN1,    KC_Z,    KC_X,    KC_M,    KC_C,    KC_V,    KC_K,    KC_L, KC_COMM,  KC_DOT, KC_SLSH,  KC_FN2,  KC_FN6 },
        {  KC_LCTL, KC_LGUI, KC_LALT,   KC_NO,   KC_NO,  KC_SPC,   KC_NO,   KC_NO,   KC_NO,  KC_DEL,  KC_FN0, KC_RALT,  KC_APP, KC_RCTL }
    },

    /* Carpalx QGMLWY base layer
     * ,-----------------------------------------------------------.
     * |  `|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|  \|Del|
     * |-----------------------------------------------------------|
     * |Tab  |  Q|  G|  M|  L|  W|  Y|  F|  U|  B|  ;|  [|  ]| BS  |
     * |-----------------------------------------------------------|
     * |Esc   |  D|  S|  T|  N|  R|  I|  A|  E|  O|  H|  '|   Enter|
     * |-----------------------------------------------------------|
     * |Shift|Num|  Z|  X|  C|  V|  J|  K|  P|  ,|  .|  /|Shift|Fn2|
     * |-----------------------------------------------------------|
     * |Ctl | Sys| Alt|         Space          | Alt| Fn0| App| Ctl|
     * `-----------------------------------------------------------'
     */
    [3] =
    {
        {   KC_GRV,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,  KC_EQL, KC_BSLS },
        {   KC_TAB,    KC_Q,    KC_G,    KC_M,    KC_L,    KC_W,    KC_Y,    KC_F,    KC_U,    KC_B, KC_SCLN, KC_LBRC, KC_RBRC, KC_BSPC },
        {   KC_FN3,    KC_D,    KC_S,    KC_T,    KC_N,    KC_R,    KC_I,    KC_A,    KC_E,    KC_O,    KC_H, KC_QUOT,   KC_NO,  KC_ENT },
        {   KC_FN5,  KC_FN1,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_J,    KC_K,    KC_P, KC_COMM,  KC_DOT, KC_SLSH,  KC_FN2,  KC_FN6 },
        {  KC_LCTL, KC_LGUI, KC_LALT,   KC_NO,   KC_NO,  KC_SPC,   KC_NO,   KC_NO,   KC_NO,  KC_DEL,  KC_FN0, KC_RALT,  KC_APP, KC_RCTL }
    },


    /* Function layer
     * ,-----------------------------------------------------------.
     * |   | F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12|Slp|INS|
     * |-----------------------------------------------------------|
     * |     |LMB|MUp|RMB|   |   |   |Hme|PgD|PgU|End| L0| L1| DEL |
     * |-----------------------------------------------------------|
     * |      |MLt|MDn|MRt|   |MUT|Lft|Dwn| Up|Rgt|SLk| L2|        |
     * |-----------------------------------------------------------|
     * |AC0 |AC2|MB4|MMB|MB5|VO+|VO-|WhL|WhD|WhU|WhR|PSc|    L3|   |
     * |-----------------------------------------------------------|
     * |    |    |    |                        |    |    |    |    |
     * `-----------------------------------------------------------'
     */
    [5] =
    {
        {  KC_FN20,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12, KC_SLEP },
        {  KC_TRNS, KC_BTN1, KC_MS_U, KC_BTN2, KC_TRNS, KC_TRNS, KC_TRNS, KC_HOME, KC_PGDN, KC_PGUP,  KC_END, KC_FN10, KC_FN11,  KC_DEL },
        {  KC_TRNS, KC_MS_L, KC_MS_D, KC_MS_R, KC_TRNS, KC_MUTE, KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, KC_SLCK, KC_FN12, KC_TRNS, KC_TRNS },
        {  KC_ACL0, KC_ACL2, KC_BTN4, KC_BTN3, KC_BTN5, KC_VOLD, KC_VOLU, KC_WH_L, KC_WH_D, KC_WH_U, KC_WH_R, KC_PSCR, KC_TRNS, KC_FN13 },
        {  KC_TRNS, KC_RGUI, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_INS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS }
    },


    /* Number pad layer
     * ,-----------------------------------------------------------.
     * |BKL| F1| F2| F3| F4| F5| F6|  7|  8|  9|  /|   |   |   |   |
     * |-----------------------------------------------------------|
     * |     |   |   |   |   |   |   |  4|  5|  6|  -|   |   |     |
     * |-----------------------------------------------------------|
     * |      |   |   |   |   |   |   |  1|  2|  3|  +|   |        |
     * |-----------------------------------------------------------|
     * |     |  |   |   |   |   |   |   |  0|  *|  .|ENT|      |   |
     * |-----------------------------------------------------------|
     * |    |    |    |                        |    |    |    |    |
     * `-----------------------------------------------------------'
     * BKL: cycle backlight modes: off, constant, breathing
     */

    [6] =
    {
        {  KC_FN21,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_P7,   KC_P8,   KC_P9, KC_PSLS, KC_PMNS, KC_PPLS, KC_PSLS },
        {  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_P4,   KC_P5,   KC_P6, KC_PMNS, KC_TRNS, KC_TRNS, KC_TRNS },
        {  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_P1,   KC_P2,   KC_P3, KC_PPLS, KC_TRNS, KC_TRNS, KC_TRNS },
        {  KC_CAPS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_P0, KC_PAST, KC_PDOT, KC_PENT, KC_TRNS, KC_NLCK },
        {  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS }
    },


    /* Special layer */
    [7] =
    {
        {  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        {  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        {  KC_LSFT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        {  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_ESC, KC_TRNS },
        {  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
    },
};


/* actions */
const action_t PROGMEM fn_actions[] = {
    [0] = ACTION_LAYER_MOMENTARY(5),
    [2] = ACTION_LAYER_TAP_TOGGLE(5),
    [3] = ACTION_LAYER_TAP_KEY(6, KC_ESC), // Dual role, hold activates number layer, tap sends ESC.

    [5] = ACTION_MODS_ONESHOT(MOD_LSFT),
    [6] = ACTION_MODS_ONESHOT(MOD_RSFT),

    [10] = ACTION_DEFAULT_LAYER_SET(0),  // set qwerty layout
    [11] = ACTION_DEFAULT_LAYER_SET(1),  // set dvorak layout
    [12] = ACTION_DEFAULT_LAYER_SET(2),  // set workman layout
    [13] = ACTION_DEFAULT_LAYER_SET(3),  // set carpalx qgmlwy layout

    [20] = ACTION_LAYER_TOGGLE(7),
    [21] = ACTION_BACKLIGHT_STEP(),
};
