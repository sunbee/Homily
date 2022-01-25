#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "SECRETS.h"
#include "Pins.h"

String name="FLLTRA";

int LED = 16;

#include "Float.h"
Float _floodLo = Float(PIN_FLOATA);
Float _floodMe = Float(PIN_FLOATB);
Float _floodHi = Float(PIN_FlOATC);
bool isLevelLo = false;
bool isLevelMe = false;
bool isLevelHi = false;

unsigned long tic = millis();

WiFiClient HTTPClient ;
PubSubClient MQTTClient(HTTPClient);

void onmessage(char* topic, byte* payload, unsigned int length) {
  /*
  Handle a new message published to the subscribed topic on the 
  MQTT broker and show in the OLED display.
  This is the heart of the subscriber, making it so the nodemcu
  can act upon information, say, to operate a solenoid valve when
  the mositure sensor indicates dry soil.
  */
  Serial.print("Got a message in topic ");
  Serial.println(topic);
  Serial.print("Received data: ");
  char message2display[length];
  for (unsigned int i = 0; i < length; i++) {
    Serial.print(char(payload[i]));
    message2display[i] = payload[i];
  }
  Serial.println();
}

String make_message() {
 // Levels
 isLevelLo = _floodLo.get_isflood();
 isLevelMe = _floodMe.get_isflood();
 isLevelHi = _floodHi.get_isflood();

 char levelLo2display[2];
 char levelMe2display[2];
 char levelHi2display[2];

 dtostrf((isLevelLo * 1.0), 1, 0, levelLo2display);
 dtostrf((isLevelMe * 1.0), 1, 0, levelMe2display);
 dtostrf((isLevelHi * 1.0), 1, 0, levelHi2display);

 // Pack up!
 char readout[25]; // 22
 snprintf(readout, 25, "{\"Lo\":%s,\"Me\":%s,\"Hi\":%s}", levelLo2display, levelMe2display, levelHi2display);
 return readout;
}

void publish_message() {
  String msg_payload =  make_message(); // "Namaste from ESP";
  Serial.print("Payload: ");
  Serial.println(msg_payload);
  char char_buffer[128];
  msg_payload.toCharArray(char_buffer, 128);
  MQTTClient.publish("FLL", char_buffer);
}

void reconnect() {
  /*
  Connect to the MQTT broker in order to publish a message
  or listen in on a topic of interest. 
  The 'connect()' method requires client credentials. 
  When the MQTT broker is not setup for authentication, 
  we have successfully connected to the MQTT broker 
  passing string literals for args 'id' and 'user' and NULL for 'pass'.
  Having connected successully, proceed to publish or listen.
  Example: "BHRIGU", "fire_up_your_neurons", NULL
  */
  while (!MQTTClient.connected()) {
    if (MQTTClient.connect("FLLTRA", MQTT_USERID, MQTT_PASSWD)) {
      Serial.println("Uh-Kay!");
      MQTTClient.subscribe("FLL"); // SUBSCRIBE TO TOPIC
    } else {
      Serial.print("Retrying ");
      Serial.println(MQTT_IP);
      delay(600);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial) {
    // Stabilize the serial bus
  }
  delay(600);

  // Connect to WiFi:
  WiFi.mode(WIFI_OFF);
  delay(1500);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(600);
    Serial.print(".");
  }
  Serial.println(".");
  Serial.print("Connected: ");
  Serial.println(WIFI_SSID);

  // MQTT:
  MQTTClient.setServer(MQTT_IP, 1883);
  MQTTClient.setCallback(onmessage);

  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long toc = millis();
  /*
    Lights! Camera! Action!
    Here is where the action is for publisher and subscriber.
    Note the use of millis to scheduling the publication of
    sensor readings to the MQTT broker in a non-blocking way. 
    The use of 'delay()' would block the listener, 
    causing events to be missed.  
  */
  digitalWrite(LED, LOW);  
  if ((toc - tic) > 3000) {
     tic = toc;
    if (!MQTTClient.connected()) {
      Serial.println("Made no MQTT connection.");
      reconnect();
    } else {
      digitalWrite(LED, HIGH);
      publish_message(); // Publisher action
    }
  }
  MQTTClient.loop(); // Callbacks handled in event loop
}