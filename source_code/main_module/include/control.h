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

enum RC5_EVENTS{
  EVENT_NONE,
  EVENT_RESET,
  EVENT_STOP,
};

void control_init();
void control_manage_screen();
void control_manage_battery();

void control_manage_pending_event();
void control_set_pending_event(enum RC5_EVENTS event);

#endif