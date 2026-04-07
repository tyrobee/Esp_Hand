#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

typedef struct struct_message {
  int command;
} struct_message;

struct_message myData;

Servo s1, s2, s3, s4, s5;

int posOpen[5]  = {10, 10, 10, 10, 10};
int posClose[5] = {170, 170, 170, 170, 170};

int targetPos[5] = {10, 10, 10, 10, 10};

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

  if (myData.command == 1) {
    for (int i = 0; i < 5; i++) targetPos[i] = posClose[i];
  } else {
    for (int i = 0; i < 5; i++) targetPos[i] = posOpen[i];
  }
}

void setup() {
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  s1.attach(13);
  s2.attach(12);
  s3.attach(14);
  s4.attach(27);
  s5.attach(26);
}

void loop() {
  s1.write(targetPos[0]);
  s2.write(targetPos[1]);
  s3.write(targetPos[2]);
  s4.write(targetPos[3]);
  s5.write(targetPos[4]);
  delay(20);
}