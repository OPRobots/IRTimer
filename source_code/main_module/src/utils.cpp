#include "utils.h"

String formatDeltaMs(long delta) {
  char buffer[10];
  float delta_s = abs(delta) / 1000.0;
  sprintf(buffer, "%0.3f", delta_s);
  String delta_str = String(buffer);

  if (delta_s < 1.0) {
    delta_str = delta_str.substring(1, 5);
  }else{
    delta_str = delta_str.substring(0, 4);
  }

  String delta_final;
  if (delta < 0) {
    delta_final = "-" + delta_str;
  } else {
    delta_final = "+" + delta_str;
  }
  return delta_final;
}

String formatTimeMs(unsigned long millis) {
  unsigned long seconds = millis / 1000;
  unsigned long minutes = seconds / 60;
  seconds = seconds % 60;
  unsigned long milliseconds = millis % 1000;
  char buffer[10];
  if (minutes > 99) {
    sprintf(buffer, "%03lu:%02lu:%02lu", minutes, seconds, milliseconds);
  } else {
    sprintf(buffer, "%02lu:%02lu:%03lu", minutes, seconds, milliseconds);
  }
  return String(buffer);
}