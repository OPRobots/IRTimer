#include "screen.h"

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void screen_setup() {
  tft.init(135, 240, SPI_MODE2);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
}


void screen_show_server_info(String ip, String mac, String channel, String status) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.println("IP Address:");
  tft.setTextSize(3);
  tft.println(ip);
  tft.setTextSize(2);
  tft.println("MAC Address:");
  tft.setTextSize(3);
  tft.println(mac);
  tft.setTextSize(2);
  tft.print("Channel: ");
  tft.println(channel);
  tft.print("Status: ");
  tft.println(status);
}