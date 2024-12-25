#include "config.h"

void config_setup() {
  Serial.begin(115200);

  pinMode(SENSOR_PIN, INPUT_PULLUP);

  pinMode(NEOPIXEL_PIN, OUTPUT);

  set_led(RGB_CONNECTION, 10, 10, 0);

  set_led(RGB_POWER, 0, 10, 0);
  client_setup();
}