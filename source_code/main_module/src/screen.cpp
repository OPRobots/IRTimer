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
  delay(2000);
  tft.fillScreen(ST77XX_BLACK);
  screen_show_button_labels();
}

void screen_show_button_labels() {
  int x = 228;
  int y = 2;
  tft.setCursor(x, y);
  tft.fillRect(x - 6, 0, 30, 140, ST77XX_BLACK);
  tft.println("M");
  tft.setCursor(x, tft.getCursorY());
  tft.println("O");
  tft.setCursor(x, tft.getCursorY());
  tft.println("D");
  tft.setCursor(x, tft.getCursorY());
  tft.println("E");
  tft.setCursor(x, tft.getCursorY());

  tft.drawRoundRect(222, 0, 238, tft.getCursorY() - y + 4, 8, ST77XX_BLUE);

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
}

static long last_lap_updated_ms = 0;
long screen_update_timmings(long time_ms, long last_time_ms, int lap_number, long best_lap_ms, long last_lap_ms, long *last_laps, long *last_laps_delta, int *last_laps_number, bool update_best_lap) {
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(3);
  int x1 = 30;
  int char_width = 18;
  int char_height = 30;
  int char_count = 9;
  String lastTime = formatTimeMs(last_time_ms);
  String time = formatTimeMs(time_ms);
  if (time != lastTime || last_time_ms == 0 || time_ms == 0) {
    int first_char = 0;
    int char_count = 0;
    for (int i = 0; i < 10; i++) {
      if (time[i] != lastTime[i] && last_time_ms != 0) {
        if (first_char == 0) {
          first_char = i;
        }
        char_count++;
      }
    }
    tft.fillRect(x1 + (first_char * char_width), 0, char_width * char_count, char_height, ST77XX_BLACK);
    tft.setCursor(x1 + (first_char * char_width), 0);
    tft.println(time.substring(first_char));
  }

  if (last_lap_updated_ms != last_lap_ms) {
    last_lap_updated_ms = last_lap_ms;
    tft.setCursor(0, 30);
    tft.setTextSize(2);
    if (update_best_lap) {
      tft.fillRect(0, 30, 220, 20, ST77XX_BLACK);
      if (best_lap_ms != 0) {
        tft.setTextColor(ST77XX_WHITE);
        tft.printf("L%-2d ", lap_number);
        tft.setTextColor(ST77XX_MAGENTA);
        tft.setCursor(tft.getCursorX() + 2, 30);
        tft.printf("%s", "BEST");
        tft.setTextColor(ST77XX_WHITE);
        tft.setCursor(tft.getCursorX() + 10, tft.getCursorY());
        tft.println(formatTimeMs(best_lap_ms));
      }
    }

    for (int i = 0; i < 5; i++) {
      tft.fillRect(0, 53 + (i * 17), 220, 30, ST77XX_BLACK);
      tft.setCursor(0, 53 + (i * 17));
      if (last_laps[i] == 0) {
        continue;
      }
      tft.setTextColor(ST77XX_WHITE);
      tft.printf("L%-2d", last_laps_number[i]);
      if (last_laps_delta[i] <= 0) {
        tft.setTextColor(ST77XX_GREEN);
      } else {
        tft.setTextColor(ST77XX_ORANGE);
      }
      tft.setCursor(tft.getCursorX() + 6, tft.getCursorY());
      tft.printf("%s", formatDeltaMs(last_laps_delta[i]));
      tft.setTextColor(ST77XX_WHITE);
      tft.setCursor(tft.getCursorX() + 6, tft.getCursorY());
      tft.println(formatTimeMs(last_laps[i]));
    }
  }
  return time_ms;
  // tft.print("L1 ");
  // tft.setTextColor(ST77XX_MAGENTA);
  // tft.print("BEST");
  // tft.setTextColor(ST77XX_WHITE);
  // tft.println(" 00:00.000");
  // tft.setTextSize(1);
  // tft.println("");
  // tft.setTextSize(2);
  // tft.print("L1 ");
  // tft.setTextColor(ST77XX_GREEN);
  // tft.print("-3.6");
  // tft.setTextColor(ST77XX_WHITE);
  // tft.println(" 00:00.000");
  // tft.print("L2 ");
  // tft.setTextColor(ST77XX_ORANGE);
  // tft.print("+0.8");
  // tft.setTextColor(ST77XX_WHITE);
  // tft.println(" 00:00.000");
  // tft.println("L3 +0.0 00:00.000");
  // tft.println("L4 +0.0 00:00.000");
  // tft.println("L5 +0.0 00:00.000");

  // tft.setTextColor(ST77XX_WHITE);
  // tft.setTextSize(3);
  // int x1 = 25;
  // int char_width = 18;
  // int char_height = 30;
  // int char_count = 9;
  // String lastTime = "00:00:000";
  // while (true) {
  //   String time = formatTimeMs(millis());
  //   if (time != lastTime) {
  //     int first_char = 0;
  //     int char_count = 0;
  //     for (int i = 0; i < 10; i++) {
  //       if (time[i] != lastTime[i]) {
  //         if (first_char == 0) {
  //           first_char = i;
  //         }
  //         char_count++;
  //       }
  //     }
  //     tft.fillRect(x1 + (first_char * char_width), 0, char_width * char_count, char_height, ST77XX_BLACK);
  //     tft.setCursor(x1 + (first_char * char_width), 0);
  //     tft.println(time.substring(first_char));
  //   }
  //   lastTime = time;
  //   delay(50);
  // }
}