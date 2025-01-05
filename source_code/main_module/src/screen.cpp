#include "screen.h"

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

static void screen_fast_draw_bitmap(const uint16_t *bitmap) {
  int total = tft.width() * tft.height();
  uint8_t x = 0;
  uint8_t y = 0;
  uint16_t draw_color = 0;
  int count_color = 0;
  for (int i = 0; i < total; i++) {
    if (splash_screen[i] != 0xFFFF) {
      x = i % tft.width();
      y = i / tft.width();
      count_color = 1;
      draw_color = splash_screen[i];
      while (i + 1 < total && splash_screen[i + 1] == draw_color) {
        i++;
        count_color++;
      }
      if (count_color == 1) {
        tft.drawPixel(x, y, draw_color);
      } else if (count_color > 1) {
        tft.drawFastHLine(x, y, count_color, draw_color);
      }
    }
  }
}

void screen_setup() {
  tft.init(135, 240, SPI_MODE2);
  tft.setRotation(3);
  screen_show_splash_screen();
}

void screen_show_splash_screen() {
  tft.fillScreen(ST77XX_WHITE);
  screen_fast_draw_bitmap(splash_screen);
  delay(2000);
  tft.fillScreen(ST77XX_BLACK);
  // tft.drawRGBBitmap(0, 0, splash_screen, tft.width(), tft.height());
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

void screen_update_timmings() {
  delay(3000);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(25, 0);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(3);
  tft.println("00:00.000");
  tft.setTextSize(1);
  tft.println("");
  tft.setTextSize(2);
  tft.println("L1 BEST 00:00.000");
  tft.setTextSize(1);
  tft.println("");
  tft.setTextSize(2);
  tft.println("L1 -3.6 00:00.000");
  tft.println("L2 +0.8 00:00.000");
  tft.println("L3 +0.0 00:00.000");
  tft.println("L4 +0.0 00:00.000");
  tft.println("L5 +0.0 00:00.000");

  /* tft.setRotation(4);
  tft.setCursor(0, 0);

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.print("MODE");
  tft.print("   ");
  tft.println("STOP"); */
  int x = 228;
  int y = 2;
  tft.setCursor(x, y);
  tft.println("M");
  tft.setCursor(x, tft.getCursorY());
  tft.println("O");
  tft.setCursor(x, tft.getCursorY());
  tft.println("D");
  tft.setCursor(x, tft.getCursorY());
  tft.println("E");
  tft.setCursor(x, tft.getCursorY());

  tft.drawRoundRect(222, 0, 238, tft.getCursorY() - y + 4, 8, ST77XX_BLUE);
  // tft.fillRoundRect(222, 0, 238, tft.getCursorY()-y+4, 8, ST77XX_BLUE);

  y = tft.getCursorY() + 5;
  tft.setCursor(x, y);
  tft.println("S");
  tft.setCursor(x, tft.getCursorY());
  tft.println("T");
  tft.setCursor(x, tft.getCursorY());
  tft.println("O");
  tft.setCursor(x, tft.getCursorY());
  tft.println("P");
  tft.setCursor(x, tft.getCursorY());

  tft.drawRoundRect(222, y - 2, 238, tft.getCursorY() - y + 2, 8, ST77XX_BLUE);

  delay(3000);
  tft.drawRGBBitmap(0, 0, IRTimer_splash, tft.width(), tft.height());
}