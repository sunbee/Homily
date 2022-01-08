#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "SECRETS.h"
#include "Pins.h"

String name="MCU106";

#include "Pot.h"
#define TIMER_INTERVAL  6000     // 6 seconds
#define TIMER_MIN       2000     // 2 seconds MIN
#define TIMER_MAX       180000   // 3 minutes MAX
Pot _pot = Pot();
int potval;
int interval;

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
    if (MQTTClient.connect("nodemcu", MQTT_USERID, MQTT_PASSWD)) {
      Serial.println("Uh-Kay!");
      MQTTClient.subscribe("Test"); // SUBSCRIBE TO TOPIC
    } else {
      Serial.print("Retrying ");
      Serial.println(MQTT_IP);
      delay(699);
    }
  }
}

String make_message() {
  // Pot:
  potval = _pot.get_pot();
  char pot2display[7];
  dtostrf(potval, 4, 0, pot2display);

  // Pack up!
  char readout[32];
  snprintf(readout, 32, "{\"Name\":\"%6s\",\"Pot\":%6s}", name.c_str(), pot2display);
  return readout;
}

void publish_message() {
  String msg_payload =  make_message(); "Namaste from ESP";
  Serial.println(msg_payload);
  char char_buffer[128];
  msg_payload.toCharArray(char_buffer, 128);
  MQTTClient.publish("Test", char_buffer);
}

int LED = 16;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial) {
    // Stabilize the serial bus
  }

  // Connect tounsigned long tic = millis(); WiFi:
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
  interval = map(_pot.get_pot(), 0, 1023, TIMER_MIN, TIMER_MAX);
  if ((toc - tic) > 30000) {
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