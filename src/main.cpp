#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "SECRETS.h"

const char* SSID = WIFI_SSID;
const char* PASS = WIFI_PASS;
const char* MQTTIP = MQTT_IP;

WiFiClient HTTPClient ;
PubSubClient MQTTClient(HTTPClient);

int LED = 2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial) {
    // Stabilize the serial bus
  }

  // Connect to WiFi:
  WiFi.mode(WIFI_OFF);
  delay(1500);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);
  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(600);
    Serial.print(".");
  }
  Serial.println("Connected: ");
  Serial.println(SSID);

  pinMode(LED, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED, HIGH);
  delay(666);
  digitalWrite(LED, LOW);
  delay(666);
}