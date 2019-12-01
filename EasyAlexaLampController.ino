#include <Arduino.h>
#include "fauxmoESP.h"

#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

#define WIFI_SSID ""  //your wifi ssid
#define WIFI_PASS ""  //your wifi password
#define LAMP "" //name of device - the name you will use to call from alexa
#define RELAY_PIN 13  //arduino pin where the relay is connected

fauxmoESP fauxmo;
bool stato = false;

void wifiSetup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
}

void setup() {
  Serial.println();
  wifiSetup();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  fauxmo.createServer(true);
  fauxmo.setPort(80);
  fauxmo.enable(true);
  fauxmo.addDevice(LAMP);
  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
    if ( (strcmp(device_name, LAMP) == 0) ) {
      stato = true;
    }
  });
}

void loop() {
  fauxmo.handle();
  static unsigned long last = millis();
  /*if (millis() - last > 5000) {
    last = millis();
  }*/
  if(stato)
  {
    digitalWrite(RELAY_PIN, HIGH);
    delay(250);
    digitalWrite(RELAY_PIN, LOW);
    stato = false;
  }
}
