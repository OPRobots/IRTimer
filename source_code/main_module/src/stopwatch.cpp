#include "stopwatch.h"

static bool is_running = false;
static long last_time_ms = 0;
static long last_timer_update_ms = 0;
static bool update_last_laps = false;
static bool last_timer_update_laps_updated = false;

static long start_ms = 0;
static int lap_number = 0;
static long last_lap_ms = 0;
static long best_lap_ms = 0;
static long last_laps[5] = {0, 0, 0, 0, 0};
static long last_laps_delta[5] = {0, 0, 0, 0, 0};
static int last_laps_number[5] = {0, 0, 0, 0, 0};

void stopwatch_start() {
  // stopwatch_reset();
  is_running = true;
  start_ms = millis();
  web_start();
}

void stopwatch_stop() {
  is_running = false;
  start_ms = 0;
  web_stop();
}

void stopwatch_lap() {
  if (!is_running) {
    stopwatch_start();
  } else {
    long lap_ms = millis() - start_ms;
    if (lap_ms < 1000) {
      return;
    }
    lap_number++;
    update_last_laps = true;
    start_ms = millis();
    if (lap_ms < best_lap_ms || best_lap_ms == 0) {
      best_lap_ms = lap_ms;
    }
    for (int i = 4; i > 0; i--) {
      last_laps[i] = last_laps[i - 1];
      last_laps_delta[i] = last_laps_delta[i - 1];
      last_laps_number[i] = last_laps_number[i - 1];
    }
    last_laps[0] = lap_ms;
    last_laps_delta[0] = last_lap_ms > 0 ? (lap_ms - last_lap_ms) : (-lap_ms);
    last_laps_number[0] = lap_number;
    last_lap_ms = lap_ms;
  }
  web_lap();
}

void stopwatch_reset() {
  is_running = false;
  start_ms = 0;
  lap_number = 0;
  last_lap_ms = 0;
  best_lap_ms = 0;
  for (int i = 0; i < 5; i++) {
    last_laps[i] = 0;
    last_laps_delta[i] = 0;
    last_laps_number[i] = 0;
  }
  web_reset();
}

void stopwatch_check() {
  if (!digitalRead(SENSOR_PIN)) {
    stopwatch_lap();
  }
  if (millis() - last_timer_update_ms >= 10) {
    last_time_ms = screen_update_timmings(is_running ? (millis() - start_ms) : 0, last_time_ms, lap_number, best_lap_ms, last_lap_ms, last_laps, last_laps_delta, last_laps_number, best_lap_ms == last_lap_ms);

    last_timer_update_ms = millis();
  }
}
