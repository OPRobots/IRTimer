#include "stopwatch.h"

static uint32_t last_event_ms = 0;

void stopwatch_check() {
  if (!digitalRead(SENSOR_PIN) && millis() - last_event_ms > 500) {
    client_lap();
    last_event_ms = millis();
  }
}
