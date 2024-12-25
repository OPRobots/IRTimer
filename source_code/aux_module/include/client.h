#ifndef CLIENT_H
#define CLIENT_H

#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include "utils.h"

void client_setup();

void client_lap();

#endif