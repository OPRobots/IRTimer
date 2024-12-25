#include "config.h"

void config_setup() {
  Serial.begin(115200);

  pinMode(SENSOR_PIN, INPUT_PULLUP);

  rc5_init();

  pinMode(IR_PIN, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(IR_PIN), rc5_isr, CHANGE);

  screen_setup();
  web_setup();
}