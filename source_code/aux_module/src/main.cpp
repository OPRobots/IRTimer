#include <Arduino.h>

#include "config.h"
#include "stopwatch.h"

void setup() {
  config_setup();
}

void loop() {
  stopwatch_check();
}
