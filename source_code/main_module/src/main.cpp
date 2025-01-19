#include "config.h"
#include "control.h"
#include "rc5.h"
#include "screen.h"
#include "utils.h"
#include "web.h"
#include <Arduino.h>


void setup() {
  config_setup();
  control_init();
}

void loop() {
  // rc5_update();
  // stopwatch_check();
  control_manage_screen();
  control_manage_battery();
}
