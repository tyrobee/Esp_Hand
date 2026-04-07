#include <esp_now.h>
#include <WiFi.h>

const int emgPin = 34;
int threshold = 2000;

int prevValue = 0;

typedef struct struct_message {
  int command;
} struct_message;

struct_message myData;

uint8_t receiverAddress[] = {0x24, 0x6F, 0x28, 0xAA, 0xBB, 0xCC};

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    return;
  }
}

void loop() {
  int emgValue = analogRead(emgPin);

  emgValue = (emgValue + prevValue) / 2;
  prevValue = emgValue;

  if (emgValue > threshold) {
    myData.command = 1;
  } else {
    myData.command = 0;
  }

  esp_now_send(receiverAddress, (uint8_t *) &myData, sizeof(myData));

  delay(50);
}