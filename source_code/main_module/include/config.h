#ifndef CONFIG_H
#define CONFIG_H

#include "rc5.h"
#include "screen.h"
#include "web.h"
#include <Arduino.h>

#define IR_PIN 7
#define SENSOR_PIN 1
#define NEOPIXEL_PIN 10

#define BTN_MODE_PIN 6
#define BTN_RESET_PIN 5

#define BATT_CHARGING_PIN 3
#define BATT_FULL_PIN 2
#define BATT_VOLTAGE_PIN 0

#define TFT_MOSI 8 // SDA Pin on ESP32
#define TFT_SCLK 9 // SCL Pin on ESP32
#define TFT_CS 20  // Chip select control pin
#define TFT_DC 21  // Data Command control pin
#define TFT_RST 4  // Reset pin (could connect to RST pin)

void config_setup();

#endif