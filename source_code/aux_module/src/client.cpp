#include "client.h"

#define board_ID 1
uint8_t broadcastAddress[] = {0x64, 0xE8, 0x33, 0x80, 0x92, 0x00};

const char *ssid = ENV_WIFI_SSID;
const char *password = ENV_WIFI_PASS;

void data_sent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Readings Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void client_setup() {
  WiFi.mode(WIFI_STA);

  int32_t channel = 0;
  if (int32_t n = WiFi.scanNetworks()) {
    for (uint8_t i = 0; i < n; i++) {
      if (!strcmp(ssid, WiFi.SSID(i).c_str())) {
        channel = WiFi.channel(i);
      }
    }
  }

  WiFi.printDiag(Serial);
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
  WiFi.printDiag(Serial);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    set_led(RGB_CONNECTION, 10, 0, 0);
    return;
  }

  esp_now_register_send_cb(data_sent);

  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    set_led(RGB_CONNECTION, 10, 0, 0);
    return;
  }
  set_led(RGB_CONNECTION, 0, 10, 0);
}

void client_lap() {
  uint8_t data = 27;
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&data, sizeof(data));
}