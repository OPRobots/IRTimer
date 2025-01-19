#ifndef SCREEN_H
#define SCREEN_H

#include "config.h"
#include "control.h"
#include "splash_screen.h"
#include "menu_screen.h"
#include "icon_wifi.h"
#include "utils.h"
#include <Adafruit_GFX.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_ST7789.h>
#include <Arduino.h>
#include <SPI.h>

enum BUTTONS {
  BUTTON_TOP_RIGHT,
  BUTTON_BOTTOM_RIGHT,
};

enum ICONS {
    ICON_UP_DOWN,
    ICON_CHECK,
};

void screen_setup();

void screen_show_unknown();
void screen_show_splash();
void screen_show_menu();
void screen_show_timer();
void screen_show_wifi_init();
void screen_show_wifi_info();
void screen_show_wifi_fail();

void screen_set_button_text(enum BUTTONS button, String label);
void screen_set_button_icon(enum BUTTONS button, enum ICONS icon);
void screen_set_side_selector(uint8_t index, uint8_t count);

long screen_update_timmings(long time_ms, long last_time_ms, int lap_number, long best_lap_ms, long last_lap_ms, long *last_laps, long *last_laps_delta, int *last_laps_number, bool update_best_lap);

#endif