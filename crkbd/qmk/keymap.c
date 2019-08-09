#include QMK_KEYBOARD_H
#include "bootloader.h"
#ifdef PROTOCOL_LUFA
  #include "lufa.h"
  #include "split_util.h"
#endif
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif

extern keymap_config_t keymap_config;

#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

extern uint8_t is_master;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _QWERTY 0
#define _LOWER 1
#define _RAISE 2
#define _ADJUST 3

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  ADJUST,
  RGBRST
};

enum macro_keycodes {
  COMMA_SCOLON,
  DOT_COLON,
  QUES_EXCL,
};

#define KC______ KC_TRNS
#define KC_XXXXX KC_NO
#define KC_LOWER LOWER
#define KC_RAISE RAISE
#define KC_RST   RESET
#define KC_LRST  RGBRST
#define KC_LTOG  RGB_TOG  //toggle rgb
#define KC_LHUI  RGB_HUI  //hue
#define KC_LHUD  RGB_HUD
#define KC_LSAI  RGB_SAI  //saturation
#define KC_LSAD  RGB_SAD
#define KC_LVAI  RGB_VAI  //value(brightness)
#define KC_LVAD  RGB_VAD
#define KC_LMOD  RGB_MOD  //mode
#define KC_LSPI  RGB_SPI  //speed
#define KC_LSPD  RGB_SPD
#define KC_CAD LCA(KC_DEL) //control-alt-delete
#define KC_CSE LCTL(LSFT(KC_ESC)) //control-shift-esc

// LT(layer, kc) hold for layer, tap for kc
// MT(mod, kc) hold for mod, tap for kc

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
        ESC,     Q,     W,     E,     R,     T,                      Y,     U,     I,     O,     P,  BSPC,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
        TAB,     A,     S,     D,     F,     G,                      H,     J,     K,     L,  QUOT,  ENT,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LSFT,     Z,     X,     C,     V,     B,                      N,     M,  COMMA_SCOLON,   DOT_COLON,  QUES_EXCL,  RSFT,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
            MT(LGUI,ENT), LT(LOWER,ESC), MT(LSFT,SPC),   MT(LCTL,BSPC), LT(RAISE,TAB), MT(LALT, ENT) \
                              //`--------------------'  `--------------------'
  ),

  [_RAISE] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
      XXXXX, XXXXX, XXXXX,  LBRC,  RBRC, GRAVE,                     AT,  HASH,DOLLAR, XXXXX, XXXXX,   DEL,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      XXXXX,  LABK,  RABK,  LPRN,  RPRN, TILDE,                   LEFT,  DOWN,    UP,  RGHT, XXXXX, XXXXX,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      XXXXX,  BSLS,  PSLS,  LCBR,  RCBR,  UNDS,                  EQUAL,  LABK,  RABK,  AMPR,  PIPE, XXXXX,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                             XXXXX, MO(ADJUST), XXXXX,   XXXXX, XXXXX, XXXXX \
                              //`--------------------'  `--------------------'
  ),

  [_LOWER] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
      XXXXX, XXXXX, XXXXX,  HOME,  PGUP, XXXXX,                     P7,    P8,    P9,   DEL, XXXXX, XXXXX,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      XXXXX,  PERC,  CIRC,   END,  PGDN, XXXXX,                     P4,    P5,    P6,   ENT, XXXXX, XXXXX,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      XXXXX,  PSLS,  PAST,  PMNS,  PPLS, XXXXX,                     P1,    P2,    P3,   TAB, XXXXX, XXXXX,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                  XXXXX, XXXXX, XXXXX,   XXXXX,     P0,  PDOT \
                              //`--------------------'  `--------------------'
  ),

  [_ADJUST] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
      XXXXX,    F1,    F2,    F3,    F4,    F5,                     F6,    F7,    F8,    F9,   F10, XXXXX,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      XXXXX,  LRST,  LHUI,  LSAI,  LVAI,  LSPI,                    CAD,   CSE, XXXXX,   F11,   F12, XXXXX,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      XXXXX,  LMOD,  LHUD,  LSAD,  LVAD,  LSPD,                  XXXXX, XXXXX,RGB_TOG,CAPSLOCK,NUMLOCK, XXXXX,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                  XXXXX, XXXXX, XXXXX,   XXXXX, XXXXX, XXXXX \
                              //`--------------------'  `--------------------'
  )
};

int RGB_current_mode;

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

// Setting ADJUST layer RGB back to default
void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
  if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
    layer_on(layer3);
  } else {
    layer_off(layer3);
  }
}

void matrix_init_user(void) {
    #ifdef RGBLIGHT_ENABLE
      RGB_current_mode = rgblight_config.mode;
    #endif
    //SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
    #ifdef SSD1306OLED
        iota_gfx_init(!has_usb());   // turns on the display
    #endif
}

//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED

// When add source files to SRC in rules.mk, you can use functions.
const char *read_layer_state(void);
const char *read_logo(void);
void set_keylog(uint16_t keycode, keyrecord_t *record);
const char *read_keylog(void);
const char *read_keylogs(void);

// const char *read_mode_icon(bool swap);
// const char *read_host_led_state(void);
// void set_timelog(void);
// const char *read_timelog(void);

void matrix_scan_user(void) {
   iota_gfx_task();
}

void matrix_render_user(struct CharacterMatrix *matrix) {
  if (is_master) {
    // If you want to change the display of OLED, you need to change here
    matrix_write_ln(matrix, read_layer_state());
    matrix_write_ln(matrix, read_keylog());
    matrix_write_ln(matrix, read_keylogs());
    //matrix_write_ln(matrix, read_mode_icon(keymap_config.swap_lalt_lgui));
    //matrix_write_ln(matrix, read_host_led_state());
    //matrix_write_ln(matrix, read_timelog());
  } else {
    matrix_write(matrix, read_logo());
  }
}

void matrix_update(struct CharacterMatrix *dest, const struct CharacterMatrix *source) {
  if (memcmp(dest->display, source->display, sizeof(dest->display))) {
    memcpy(dest->display, source->display, sizeof(dest->display));
    dest->dirty = true;
  }
}

void iota_gfx_task_user(void) {
  struct CharacterMatrix matrix;
  matrix_clear(&matrix);
  matrix_render_user(&matrix);
  matrix_update(&display, &matrix);
}
#endif//SSD1306OLED

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
#ifdef SSD1306OLED
    set_keylog(keycode, record);
#endif
    // set_timelog();
  }

  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        persistent_default_layer_set(1UL<<_QWERTY);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case ADJUST:
        if (record->event.pressed) {
          layer_on(_ADJUST);
        } else {
          layer_off(_ADJUST);
        }
        return false;
        break;
    case RGB_MOD:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          rgblight_mode(RGB_current_mode);
          rgblight_step();
          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      return false;
      break;
    case RGBRST:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          eeconfig_update_rgblight_default();
          rgblight_enable();
          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      break;

//custom keycodes
//tbd current the shifted key can only be entered once and then shift needs to be released
//future version should allow for multiple shifted keypresses
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
//end custom keycodes
  }
  return true;
}
