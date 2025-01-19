#include "screen.h"

SPIClass tftSPI = SPIClass(SPI);
Adafruit_ST7789 tft = Adafruit_ST7789(&tftSPI, TFT_CS, TFT_DC, TFT_RST);
static long last_lap_updated_ms = 0;

static void screen_fast_draw_bitmap(const uint16_t *bitmap, uint16_t bg_color) {
  int total = tft.width() * tft.height();
  uint8_t x = 0;
  uint8_t y = 0;
  uint16_t draw_color = 0;
  int count_color = 0;
  for (int i = 0; i < total; i++) {
    if (bitmap[i] != bg_color) {
      x = i % tft.width();
      y = i / tft.width();
      count_color = 1;
      draw_color = bitmap[i];
      while (i + 1 < total && bitmap[i + 1] == draw_color) {
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
  tftSPI.begin(TFT_SCLK, 0, TFT_MOSI, TFT_RST);

  tft.init(135, 240);
  tft.setSPISpeed(80000000);
  tft.setRotation(3);
}

void screen_show_unknown() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(3);
  tft.println("Unknown\nscreen\n:'(");
}

void screen_show_splash() {
  tft.fillScreen(ST77XX_WHITE);
  screen_fast_draw_bitmap(splash_screen, ST77XX_WHITE);
  // tft.drawRGBBitmap(0, 0, splash_screen, tft.width(), tft.height());
}

void screen_show_menu() {
  tft.fillScreen(ST77XX_BLACK);
  screen_fast_draw_bitmap(menu_screen, ST77XX_BLACK);
}

void screen_show_timer() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
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
}

void screen_set_button_text(enum BUTTONS button, String label) {
  int x = 0;
  int y = 0;
  uint8_t y_rect = 0;
  uint8_t h_rect = 0;
  switch (button) {
    case BUTTON_TOP_RIGHT:
      x = 228;
      y = 2;
      y_rect = 0;
      h_rect = 4;
      break;
    case BUTTON_BOTTOM_RIGHT:
      x = 228;
      y = 71;
      if (label.length() < 4) {
        y += 16 * (4 - label.length());
      }
      y_rect = y - 2;
      h_rect = 2;
      break;
  }
  tft.setCursor(x, y);
  tft.setTextSize(2);
  tft.fillRect(x - 6, y, 30, 67, ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  for (int i = 0; i < label.length(); i++) {
    tft.println(label[i]);
    tft.setCursor(x, tft.getCursorY());
  }
  tft.drawRoundRect(222, y_rect, 238, tft.getCursorY() - y + h_rect, 8, ST77XX_BLUE);
}

void screen_set_button_icon(enum BUTTONS button, enum ICONS icon) {
  int x = 0;
  int y = 0;
  uint8_t y_rect = 0;
  uint8_t h_rect = 0;
  switch (button) {
    case BUTTON_TOP_RIGHT:
      x = 228;
      y = 2;
      break;
    case BUTTON_BOTTOM_RIGHT:
      x = 228;
      y = 71;
      break;
  }
  tft.setCursor(x, y);
  tft.setTextSize(2);
  tft.fillRect(x - 6, y, 30, 67, ST77XX_BLACK);

  switch (icon) {
    case ICON_UP_DOWN:

      switch (button) {
        case BUTTON_TOP_RIGHT:
          x = 228;
          y = 4;
          y_rect = 0;
          h_rect = 4;
          break;
        case BUTTON_BOTTOM_RIGHT:
          x = 228;
          y = 105;
          y_rect = y - 3;
          h_rect = 2;
          break;
      }
      tft.fillTriangle(x, y + 10, x + 5, y, x + 10, y + 10, ST77XX_WHITE);
      tft.fillTriangle(x, y + 15, x + 5, y + 25, x + 10, y + 15, ST77XX_WHITE);
      tft.setCursor(x, y + 30);
      break;
    case ICON_CHECK:
      switch (button) {
        case BUTTON_TOP_RIGHT:
          x = 228;
          y = 2;
          y_rect = 0;
          h_rect = 2;
          break;
        case BUTTON_BOTTOM_RIGHT:
          x = 228;
          y = 115;
          y_rect = y - 2;
          h_rect = 2;
          break;
      }
      tft.drawLine(x, y + 10, x + 5, y + 15, ST77XX_WHITE);
      tft.drawLine(x + 5, y + 15, x + 10, y + 2, ST77XX_WHITE);
      tft.setCursor(x, y + 20);
      break;
  }

  tft.drawRoundRect(222, y_rect, 238, tft.getCursorY() - y + h_rect, 8, ST77XX_BLUE);
}

void screen_set_side_selector(uint8_t index, uint8_t count) {
  tft.fillRect(0, 0, 20, 240, ST77XX_BLACK);

  uint8_t selector_height = 0;
  switch (count) {
    case 2:
      selector_height = 40;
      break;
    default:
      return;
      break;
  }
  uint8_t margin = (135 - (selector_height * count)) / (count + 1);
  uint8_t y = margin + (selector_height * index) + (margin * index) + selector_height / 2;

  // tft.fillRect(0, y-20, 10, 40, ST77XX_WHITE);
  tft.fillTriangle(0, y - 10, 10, y, 0, y + 10, ST77XX_WHITE);
}

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
}