#ifndef UTILS_H
#define UTILS_H

#include "config.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>


String formatDeltaMs(long delta, bool trim);
String formatTimeMs(unsigned long millis);

enum LEDS {
  RGB_POWER = 0,
  RGB_CONNECTION = 1,
};

void set_led(enum LEDS led, int r, int g, int b);
void clear_led(enum LEDS led);

void blink_led(enum LEDS led, int r, int g, int b, int ms);
void rainbow_led(enum LEDS led);

float get_battery_voltage();

#endif