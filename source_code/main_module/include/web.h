#ifndef WEB_H
#define WEB_H

#include "screen.h"
#include "stopwatch.h"
#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <esp_now.h>

void web_setup();

void web_start();
void web_stop();
void web_lap();
void web_reset();

#endif