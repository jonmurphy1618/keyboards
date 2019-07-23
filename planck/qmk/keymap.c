/* Copyright 2015-2017 Jack Humbert
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

extern keymap_config_t keymap_config;

enum planck_layers {
  _QWERTY,
  _LOWER,
  _RAISE,
  _ADJUST,
};

enum planck_keycodes {
  QWERTY = SAFE_RANGE,
  BACKLIT,
  COMMA_SCOLON,
  DOT_COLON,
  QUES_EXCL,
};
/*
#define UNI_DLARROW UC(00AB) // «
#define UNI_DRARROW UC(00BB) // »
*/
#define KC_CAD LCA(KC_DEL) // control-alt-delete
#define KC_CSE LCTL(LSFT(KC_ESC)) // control-shift-esc

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_QWERTY] = LAYOUT_planck_mit(
    KC_ESC, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSPC,
    KC_TAB, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_QUOT, KC_ENT,
    KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, COMMA_SCOLON, DOT_COLON, QUES_EXCL, XXXXXXX,
    KC_LCTL, XXXXXXX, KC_LALT, KC_LGUI, MO(_LOWER), KC_SPC, MO(_RAISE), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
),

[_LOWER] = LAYOUT_planck_mit(
    XXXXXXX, XXXXXXX, XXXXXXX, KC_HOME, KC_PGUP, XXXXXXX, XXXXXXX, XXXXXXX, KC_P7, KC_P8, KC_P9, KC_DEL,
    XXXXXXX, KC_PERC, KC_CIRC, KC_END, KC_PGDN, XXXXXXX, XXXXXXX, XXXXXXX, KC_P4, KC_P5, KC_P6, KC_ENT,
    XXXXXXX, KC_PSLS, KC_PAST, KC_PMNS, KC_PPLS, XXXXXXX, XXXXXXX, XXXXXXX, KC_P1, KC_P2, KC_P3, KC_TAB,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MO(_LOWER), KC_SPC, MO(_RAISE), KC_P0, KC_PDOT, XXXXXXX, XXXXXXX
),

[_RAISE] = LAYOUT_planck_mit(
    XXXXXXX, XXXXXXX, XXXXXXX, KC_LPRN, KC_RPRN, KC_PIPE, KC_AT, KC_HASH, KC_DOLLAR, XXXXXXX, XXXXXXX, KC_DEL,
    XXXXXXX, KC_LABK, KC_RABK, KC_LBRC, KC_RBRC, KC_PMNS, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, XXXXXXX, XXXXXXX,
    XXXXXXX, KC_BSLS, KC_PSLS, KC_LCBR, KC_RCBR, KC_UNDS, KC_EQUAL, KC_LABK, KC_RABK, KC_AMPR, KC_PIPE, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MO(_LOWER), KC_SPC, MO(_RAISE), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
),

[_ADJUST] = LAYOUT_planck_mit(
    BACKLIT, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, XXXXXXX ,
    XXXXXXX, KC_F11, KC_F12, KC_GRV, KC_ESC, KC_TILD, KC_CAD, KC_CSE, KC_LWIN, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, KC_MPLY, KC_MPRV, KC_MNXT, KC_MSTP, KC_VOLD, KC_VOLU, KC_MUTE, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MO(_LOWER), XXXXXXX, MO(_RAISE), XXXXXXX, XXXXXXX, KC_CAPSLOCK, KC_NUMLOCK
)
};

uint32_t layer_state_set_user(uint32_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    /* macro template 
	** send string codes: https://github.com/qmk/qmk_firmware/blob/master/quantum/send_string_keycodes.h
	** macro documentation: https://docs.qmk.fm/#/feature_macros
	case FOO:
      if (record->event.pressed) {
        // do something when pressed
      } else {
		// do something else when released
	  }
      return false;
      break;
    */
	case BACKLIT:
      if (record->event.pressed) {
        register_code(KC_RSFT);
        #ifdef BACKLIGHT_ENABLE
          backlight_step();
        #endif
        #ifdef KEYBOARD_planck_rev5
          PORTE &= ~(1<<6);
        #endif
      } else {
        unregister_code(KC_RSFT);
        #ifdef KEYBOARD_planck_rev5
          PORTE |= (1<<6);
        #endif
      }
      return false;
      break;
	  
	
	case COMMA_SCOLON: // ;(w/ shift) or ,
      if (record->event.pressed) {
		  if (keyboard_report->mods & MOD_BIT (KC_LSFT)) {
			clear_mods();
			tap_code(KC_SCOLON);
		  } else {
			tap_code(KC_COMMA);
		  }
	  } else {
		  clear_keyboard();
	  }
      return false;
      break;
	  
	case DOT_COLON: // :(w/ shift) or .
      if (record->event.pressed) {
		  if (keyboard_report->mods & MOD_BIT (KC_LSFT)) {
			tap_code(KC_SCOLON);
		  } else {
			tap_code(KC_DOT);
		  }
	  } else {
		  clear_keyboard();
	  }
      return false;
      break;
	  
	case QUES_EXCL: // !(w/ shift) or ?
      if (record->event.pressed) {
		  if (keyboard_report->mods & MOD_BIT (KC_LSFT)) {
			clear_mods();
			register_code(KC_LSFT);
			register_code(KC_1);
			unregister_code(KC_1);
			unregister_code(KC_LSFT);
		  } else {
			clear_mods();
			register_code(KC_LSFT);
			register_code(KC_SLASH);
			unregister_code(KC_SLASH);
			unregister_code(KC_LSFT);
		  }
	  } else {
		  clear_keyboard();
	  }
      return false;
      break;
	
  }
  return true;
}