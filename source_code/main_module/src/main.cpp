#include "config.h"
#include "rc5.h"
#include "web.h"
#include <Arduino.h>


void setup() {
  config_setup();
}

void loop() {
  rc5_update();
  stopwatch_check();
}
