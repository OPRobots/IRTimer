#ifndef STOPWATCH_H
#define STOPWATCH_H

#include "config.h"
#include "web.h"
#include <Arduino.h>

void stopwatch_check();
void stopwatch_start();
void stopwatch_stop();
void stopwatch_lap();
void stopwatch_reset();

#endif