#ifndef WEB_H
#define WEB_H

#include "screen.h"
#include "stopwatch.h"
#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <esp_now.h>

bool web_setup();

void web_start();
void web_stop();
void web_lap();
void web_reset();

bool web_connected();
String web_get_ip();
String web_get_mac();
String web_get_channel();
String web_get_status();

#endif