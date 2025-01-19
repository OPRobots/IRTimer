#ifndef CONTROL_H
#define CONTROL_H

#include "screen.h"
#include "config.h"
#include <Arduino.h>

enum SCREENS {
  SCREEN_SPLASH,
  SCREEN_MENU,
  SCREEN_WIFI_INIT,
  SCREEN_WIFI_INFO,
  SCREEN_WIFI_FAIL,
  SCREEN_TIMER,
};

void control_init();
void control_manage_screen();
void control_manage_battery();

#endif