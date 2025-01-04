#include "config.h"
#include "rc5.h"
#include "screen.h"
#include "web.h"
#include <Arduino.h>

void setup() {
  config_setup();
  screen_update_timmings();
}

void loop() {
  rc5_update();
  stopwatch_check();
}
