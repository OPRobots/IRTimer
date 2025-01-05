#ifndef SCREEN_H
#define SCREEN_H

#include "config.h"
#include "splash_screen.h"
#include "utils.h"
#include <Adafruit_GFX.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_ST7789.h>
#include <Arduino.h>
#include <SPI.h>

void screen_setup();
void screen_show_splash_screen();
void screen_show_button_labels();
void screen_show_server_info(String ip, String mac, String channel, String status);
long screen_update_timmings(long time_ms, long last_time_ms, int lap_number, long best_lap_ms, long last_lap_ms, long *last_laps, long *last_laps_delta, int *last_laps_number, bool update_best_lap);

#endif