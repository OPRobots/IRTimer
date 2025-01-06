#include "config.h"
#include "rc5.h"
#include "screen.h"
#include "web.h"
#include "control.h"
#include <Arduino.h>

void setup() {
  config_setup();
  control_init();
}

void loop() {
  // rc5_update();
  // stopwatch_check();
control_manage_screen();

  // Serial.print("BTN_MODE: ");
  // Serial.print(digitalRead(BTN_MODE_PIN));
  // Serial.print(" BTN_RESET: ");
  // Serial.println(digitalRead(BTN_RESET_PIN));
  // delay(100);
}
