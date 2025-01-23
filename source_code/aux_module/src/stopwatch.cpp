#include "stopwatch.h"

static uint32_t last_event_ms = 0;
static bool can_start = true;

void stopwatch_check() {
  if (!digitalRead(SENSOR_PIN) && millis() - last_event_ms > 500) {
    switch (config_get_wall()) {
      case WALL_START:
        if (can_start) {
          client_start();
          can_start = false;
          set_led(RGB_CONNECTION, 10, 10, 0);
        }
        break;
      case WALL_FINISH:
        client_finish();
        break;
    }

    uint32_t sensor_ms = millis();
    while (!digitalRead(SENSOR_PIN)) {
      if (!can_start && millis() - sensor_ms > 1000) {
        can_start = true;
        set_led(RGB_CONNECTION, 0, 10, 0);
      }
    }

    last_event_ms = millis();
  }
}
