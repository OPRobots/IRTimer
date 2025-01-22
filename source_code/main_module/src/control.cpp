#include "control.h"

static enum SCREENS current_screen = SCREEN_SPLASH;
static uint32_t current_screen_ms = 0;

static uint8_t menu_index = 0;

static uint32_t battey_blink_ms = 0;
static bool battery_blink_state = false;

static enum RC5_EVENTS pending_event = EVENT_NONE;

static void set_screen(enum SCREENS screen) {
  current_screen = screen;
  current_screen_ms = millis();
  switch (screen) {
    case SCREEN_SPLASH:
      screen_show_splash();
      break;
    case SCREEN_MENU:
      screen_show_menu();
      screen_set_button_icon(BUTTON_TOP_RIGHT, ICON_UP_DOWN);
      screen_set_button_icon(BUTTON_BOTTOM_RIGHT, ICON_CHECK);
      menu_index = 0;
      screen_set_side_selector(menu_index, 2);
      break;
    case SCREEN_WIFI_INIT:
      screen_show_wifi_init();
      if (web_setup()) {
        set_screen(SCREEN_WIFI_INFO);
      } else {
        set_screen(SCREEN_WIFI_FAIL);
      }
      break;
    case SCREEN_WIFI_INFO:
      screen_show_wifi_info();
      screen_set_button_text(BUTTON_BOTTOM_RIGHT, "MENU");
      break;
    case SCREEN_WIFI_FAIL:
      screen_show_wifi_fail();
      screen_set_button_text(BUTTON_BOTTOM_RIGHT, "MENU");
      break;
    case SCREEN_TIMER:
      screen_show_timer();
      screen_set_button_text(BUTTON_TOP_RIGHT, "RST");
      screen_set_button_text(BUTTON_BOTTOM_RIGHT, "MENU");
      break;
    default:
      screen_show_unknown();
      break;
  }
}

void control_init() {
  set_screen(SCREEN_SPLASH);
}

void control_manage_screen() {
  switch (current_screen) {
    case SCREEN_SPLASH:
      if (millis() - current_screen_ms > 2000) {
        set_screen(SCREEN_MENU);
      }
      break;
    case SCREEN_MENU:
      if (digitalRead(BTN_TOP_PIN)) {
        menu_index = (menu_index + 1) % 2;
        screen_set_side_selector(menu_index, 2);
        while (digitalRead(BTN_TOP_PIN)) {
        }
      }
      if (digitalRead(BTN_BOTTOM_PIN)) {
        switch (menu_index) {
          case 0:
            set_screen(SCREEN_WIFI_INIT);
            break;
          case 1:
            set_screen(SCREEN_TIMER);
            break;
        }
        while (digitalRead(BTN_BOTTOM_PIN)) {
        }
      }
      break;
    case SCREEN_WIFI_INFO:
    case SCREEN_WIFI_FAIL:
      if (digitalRead(BTN_BOTTOM_PIN)) {
        stopwatch_reset();
        set_screen(SCREEN_MENU);
        while (digitalRead(BTN_BOTTOM_PIN)) {
        }
      }
      break;
    case SCREEN_TIMER:
      stopwatch_check();
      if (digitalRead(BTN_TOP_PIN)) {
        stopwatch_reset();
        while (digitalRead(BTN_TOP_PIN)) {
        }
      }
      if (digitalRead(BTN_BOTTOM_PIN)) {
        stopwatch_reset();
        set_screen(SCREEN_MENU);
        while (digitalRead(BTN_BOTTOM_PIN)) {
        }
      }
      break;
  }
}

void control_manage_battery() {
  if (!digitalRead(BATT_CHARGING_PIN)) {
    set_led(RGB_POWER, 50, 0, 0);
  } else if (!digitalRead(BATT_FULL_PIN)) {
    set_led(RGB_POWER, 0, 50, 0);
  } else if (get_battery_voltage() < 3.5) {
    if (millis() - battey_blink_ms > 500) {
      battery_blink_state = !battery_blink_state;
      set_led(RGB_POWER, battery_blink_state ? 50 : 0, 0, 0);
      battey_blink_ms = millis();
    }
  } else {
    set_led(RGB_POWER, 0, 0, 0);
  }
}

void control_manage_pending_event() {
  switch (pending_event) {
    case EVENT_RESET:
      stopwatch_reset();
      break;
    case EVENT_STOP:
      stopwatch_stop();
      break;
  }
  pending_event = EVENT_NONE;
}

void control_set_pending_event(enum RC5_EVENTS event) {
  pending_event = event;
}