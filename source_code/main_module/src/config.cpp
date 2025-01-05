#include "config.h"

void config_setup() {
  Serial.begin(115200);

  pinMode(SENSOR_PIN, INPUT_PULLUP);

  rc5_init();

  pinMode(IR_PIN, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(IR_PIN), rc5_isr, CHANGE);

  pinMode(BTN_MODE_PIN, INPUT_PULLDOWN);
  pinMode(BTN_RESET_PIN, INPUT_PULLDOWN);

  screen_setup();
  web_setup();
}