#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "SECRETS.h"
#include "Pins.h"

String name="MCU106";

int LED = 16;

#include "Pot.h"
unsigned long TIMER_MIN = 2000;     // 2 seconds MIN
unsigned long TIMER_MAX = 180000;   // 3 minutes MAX
Pot _pot = Pot();
int potval;

#include "Dallas.h"
Dallas _dallas = Dallas();
float cels;

#include "Gas.h"
Gas _gas = Gas();
bool isgas;

#include "Flame.h"
Flame _flame = Flame();
bool isfire;

#include "Buzzer.h"
Buzzer _buzzer = Buzzer();

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
  // Pot:
  potval = _pot.get_pot();
  char pot2display[7];
  dtostrf(potval, 4, 0, pot2display);

  // Dallas:
  cels = _dallas.get_Temp(CELSIUS, true);
  char temp2display[7];
  dtostrf((((cels < -99.99) || (cels > 999.99)) ? -99.99 :  cels), 6, 2, temp2display);

  // Gas
  isgas = !(_gas.get_isgas()); // MQ-135 LO when air-quality bad
  char gas2display[2];
  dtostrf((isgas*1.0), 1, 0, gas2display);

  // Fire
  isfire = !(_flame.get_isflame());
  char fire2display[2];
  dtostrf((isfire*1.0), 1, 0, fire2display);

  if (isgas || isfire) {
    _buzzer.sound_alarm();
  }

  // Pack up!
  char readout[66];
  snprintf(readout, 66, "{\"Name\":\"%6s\",\"Pot\":%6s,\"TempC\":%6s,\"AirQ\":%1s,\"Fire\":%1s}", name.c_str(), pot2display, temp2display, gas2display, fire2display);
  return readout;
}

void publish_message() {
  String msg_payload =  make_message(); // "Namaste from ESP";
  Serial.print("Payload: ");
  Serial.println(msg_payload);
  char char_buffer[128];
  msg_payload.toCharArray(char_buffer, 128);
  MQTTClient.publish("Test", char_buffer);
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
    if (MQTTClient.connect("MCU106", MQTT_USERID, MQTT_PASSWD)) {
      Serial.println("Uh-Kay!");
      MQTTClient.subscribe("Test"); // SUBSCRIBE TO TOPIC
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

  // Dallas:
  _dallas.start();

  // Buzzer
  _buzzer.sound_alarm();

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