#include "config.h"

void config_setup() {
  Serial.begin(115200);

  pinMode(NEOPIXEL_PIN, OUTPUT);
  pinMode(SENSOR_PIN, INPUT_PULLUP);

  set_led(RGB_CONNECTION, 10, 10, 0);
  set_led(RGB_POWER, 0, 10, 0);

  pinMode(BATT_CHARGING_PIN, INPUT);
  pinMode(BATT_FULL_PIN, INPUT);
  pinMode(BATT_VOLTAGE_PIN, INPUT);

  client_setup();
}

enum WALLS config_get_wall() {
  if(client_get_mac() == "64:E8:33:87:CB:AC") {
    return WALL_START;
  }else if(client_get_mac() == "34:B7:DA:F8:C9:9C") {
    return WALL_FINISH;
  }
  return WALL_START;
}