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

  // set_led(RGB_POWER, 255, 0, 0);
  // set_led(RGB_CONNECTION, 0, 255, 0);
  // rainbow_led(RGB_POWER);
  // rainbow_led(RGB_CONNECTION);

  // Serial.print("BTN_MODE: ");
  // Serial.print(digitalRead(BTN_MODE_PIN));
  // Serial.print(" BTN_RESET: ");
  // Serial.println(digitalRead(BTN_RESET_PIN));

  if(!digitalRead(BATT_CHARGING_PIN)){
    set_led(RGB_POWER, 50, 0, 0);
  }else{
    set_led(RGB_POWER, 0, 0, 0);
  }

  if(!digitalRead(BATT_FULL_PIN)){
    set_led(RGB_CONNECTION, 0, 50, 0);
  }else{
    set_led(RGB_CONNECTION, 0, 0, 0);
  }

  Serial.print("BATT_CHARGING_PIN: ");
  Serial.print(!digitalRead(BATT_CHARGING_PIN));
  Serial.print(" BATT_FULL_PIN: ");
  Serial.print(!digitalRead(BATT_FULL_PIN));
  Serial.print(" BATT_VOLTAGE_PIN: ");
  Serial.println(analogRead(BATT_VOLTAGE_PIN)*3.3/4095*3.13);
  delay(100);
}
