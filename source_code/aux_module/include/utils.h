#ifndef UTILS_H
#define UTILS_H

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include "config.h"

enum LEDS {
  RGB_POWER = 0,
  RGB_CONNECTION = 1,
};

void set_led(enum LEDS led, int r, int g, int b);
void clear_led(enum LEDS led);

void blink_led(enum LEDS led, int r, int g, int b, int ms);
void rainbow_led(enum LEDS led);


#endif // UTILS_H