#ifndef CLIENT_H
#define CLIENT_H

#include "utils.h"
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>


void client_setup();
void client_start();
void client_finish();

String client_get_mac();

#endif