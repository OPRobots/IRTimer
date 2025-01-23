#include <Arduino.h>

#include "client.h"
#include "config.h"
#include "stopwatch.h"

void setup() {
  config_setup();
  delay(500);
  set_led(RGB_CONNECTION, 0, 0, 0);
  delay(500);
  switch (config_get_wall()) {
    case WALL_START:
      set_led(RGB_CONNECTION, 0, 10, 0);
      break;
    case WALL_FINISH:
      set_led(RGB_CONNECTION, 10, 0, 0);
      break;
  }
}

void loop() {
  stopwatch_check();
  manage_battery();
}
