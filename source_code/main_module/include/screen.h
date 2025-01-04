#ifndef SCREEN_H
#define SCREEN_H

#include "config.h"
#include <Adafruit_GFX.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_ST7789.h>
#include <Arduino.h>
#include <SPI.h>


void screen_setup();
void screen_show_server_info(String ip, String mac, String channel, String status);
void screen_update_timmings();

#endif