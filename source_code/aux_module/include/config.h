#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "client.h"
#include "utils.h"

#define SENSOR_PIN 1
#define NEOPIXEL_PIN 10

#define BATT_CHARGING_PIN 3
#define BATT_FULL_PIN 2
#define BATT_VOLTAGE_PIN 0

enum WALLS{
    WALL_START,
    WALL_FINISH
};


void config_setup();
enum WALLS config_get_wall();

#endif