#include "config.h"

void config_setup() {
  Serial.begin(115200);

  pinMode(SENSOR_PIN, INPUT_PULLUP);

  rc5_init();

  pinMode(IR_PIN, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(IR_PIN), rc5_isr, CHANGE);

  pinMode(BTN_TOP_PIN, INPUT_PULLDOWN);
  pinMode(BTN_BOTTOM_PIN, INPUT_PULLDOWN);

  pinMode(BATT_CHARGING_PIN, INPUT);
  pinMode(BATT_FULL_PIN, INPUT);
  pinMode(BATT_VOLTAGE_PIN, INPUT);

  screen_setup();
  // web_setup();
}