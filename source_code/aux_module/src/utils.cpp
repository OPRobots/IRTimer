#include "utils.h"

static Adafruit_NeoPixel pixels(2, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

static bool toggle_led_state[2] = {false, false};
static int blink_led_ms[2] = {0, 0};

static int rainbow_led_ms[2] = {0, 0};
int rainbow_led_RGB[2][3] = {{255, 0, 0}, {255, 0, 0}};
int rainbow_led_color_desc[2] = {0, 0};
int rainbow_led_color_asc[2] = {1, 1};

static uint32_t battey_blink_ms = 0;
static bool battery_blink_state = false;

/**
 * @brief Activa un LED con un color determinado
 *
 * @param led LED a modificar
 * @param r Red
 * @param g Green
 * @param b Blue
 */
void set_led(enum LEDS led, int r, int g, int b) {
  pixels.setPixelColor(led, pixels.Color(r, g, b));
  pixels.show();
  toggle_led_state[led] = true;
  blink_led_ms[led] = millis();
}

/**
 * @brief Apaga un LED
 *
 * @param led LED a apagar
 */
void clear_led(enum LEDS led) {
  pixels.setPixelColor(led, pixels.Color(0, 0, 0));
  pixels.show();
  toggle_led_state[led] = false;
  blink_led_ms[led] = millis();
}

/**
 * @brief Parpadea un LED a una frecuencia determinada
 *
 * @param led LED a parpadear
 * @param ms Frecuencia de parpadeo
 */
void blink_led(enum LEDS led, int r, int g, int b, int ms) {
  if (millis() - blink_led_ms[led] >= ms) {
    if (toggle_led_state[led]) {
      clear_led(led);
    } else {
      set_led(led, r, g, b);
    }
  }
}

/**
 * @brief Aplica un efecto arcoiris a un LED RGB
 *
 * @param led
 * @param ms
 */
void rainbow_led(enum LEDS led) {
  if (millis() > rainbow_led_ms[led] + 30) {
    rainbow_led_ms[led] = millis();
    rainbow_led_RGB[led][rainbow_led_color_desc[led]] -= 20;
    rainbow_led_RGB[led][rainbow_led_color_asc[led]] += 20;
    pixels.setPixelColor(led, pixels.Color(rainbow_led_RGB[led][0], rainbow_led_RGB[led][1], rainbow_led_RGB[led][2]));
    pixels.show();
    if (rainbow_led_RGB[led][rainbow_led_color_desc[led]] <= 0 || rainbow_led_RGB[led][rainbow_led_color_asc[led]] >= 255) {
      rainbow_led_RGB[led][rainbow_led_color_desc[led]] = 0;
      rainbow_led_RGB[led][rainbow_led_color_asc[led]] = 255;
      pixels.setPixelColor(led, pixels.Color(rainbow_led_RGB[led][0], rainbow_led_RGB[led][1], rainbow_led_RGB[led][2]));
      pixels.show();
      rainbow_led_color_desc[led]++;
      if (rainbow_led_color_desc[led] > 2) {
        rainbow_led_color_desc[led] = 0;
      }
      rainbow_led_color_asc[led] = rainbow_led_color_desc[led] == 2 ? 0 : rainbow_led_color_desc[led] + 1;
    }
  }
}

void manage_battery() {
  if (!digitalRead(BATT_CHARGING_PIN)) {
    set_led(RGB_POWER, 50, 0, 0);
  } else if (!digitalRead(BATT_FULL_PIN)) {
    set_led(RGB_POWER, 0, 50, 0);
  } else if (get_battery_voltage() < 3.5) {
    if (millis() - battey_blink_ms > 500) {
      battery_blink_state = !battery_blink_state;
      set_led(RGB_POWER, battery_blink_state ? 50 : 0, 0, 0);
      battey_blink_ms = millis();
    }
  } else {
    set_led(RGB_POWER, 0, 0, 0);
  }
}

float get_battery_voltage() {
  return analogRead(BATT_VOLTAGE_PIN) * 3.3 / 4095 * 3.13;
}