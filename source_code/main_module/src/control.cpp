#include "control.h"


void control_init() {
  screen_show_splash();
  screen_show_menu();
}

void control_manage_screen() {
  if (digitalRead(BTN_MODE_PIN)) {
    screen_set_top_button("PATO");
    while (digitalRead(BTN_MODE_PIN)) {
    }
    screen_set_top_button("MODE");
  }

  if (digitalRead(BTN_RESET_PIN)) {
    screen_set_bottom_button("AZUL");
    while (digitalRead(BTN_RESET_PIN)) {
    }
    screen_set_bottom_button("STOP");
  }
}