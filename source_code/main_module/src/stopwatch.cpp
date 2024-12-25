#include "stopwatch.h"

void stopwatch_check() {
  if (!digitalRead(SENSOR_PIN)) {
    web_lap();
  }
}
