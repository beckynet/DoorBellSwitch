/*   
 * DoorBellSwitch
 * ESP8266-based MQTT Door Bell Switch
 * Licensed under the MIT License, Copyright (c) 2018 Beeckmans.O
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

const boolean static_ip = STATIC_IP;
IPAddress ip(IP);
IPAddress gateway(GATEWAY);
IPAddress subnet(SUBNET);

const char* mqtt_broker = MQTT_BROKER;
const char* mqtt_clientId = MQTT_CLIENTID;
const char* mqtt_username = MQTT_USERNAME;
const char* mqtt_password = MQTT_PASSWORD;

const char* door1_alias = DOOR1_ALIAS;
const char* mqtt_door1_action_topic = MQTT_DOOR1_ACTION_TOPIC;
const char* mqtt_door1_status_topic = MQTT_DOOR1_STATUS_TOPIC;
const int door1_statusPin = DOOR1_STATUS_PIN;


const char* door2_alias = DOOR2_ALIAS;
const char* mqtt_door2_action_topic = MQTT_DOOR2_ACTION_TOPIC;
const char* mqtt_door2_status_topic = MQTT_DOOR2_STATUS_TOPIC;
const int door2_statusPin = DOOR2_STATUS_PIN;

const char* mqtt_wifi_rssi_status_topic = MQTT_WIFI_RSSI_STATUS_TOPIC;

int door1_lastStatusValue = 2;
int door2_lastStatusValue = 2;
unsigned long door1_lastSwitchTime = 0;
unsigned long door2_lastSwitchTime = 0;
int debounceTime = 2000;

long wifi_rssi_lastValue = 0;
long  TXinterval = 20;            // periodic transmission interval in seconds
long  LastPeriod = -1;

String availabilityBase = MQTT_CLIENTID;
String availabilitySuffix = "/reachable";
String availabilityTopicStr = availabilityBase + availabilitySuffix;
const char* availabilityTopic = availabilityTopicStr.c_str();
const char* birthMessage = "Online";
const char* lwtMessage = "Offline";

WiFiClient espClient;
PubSubClient client(espClient);

// Wifi setup function

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.hostname("DoorBellSwitch");
  WiFi.begin(ssid, password);

  if (static_ip) {
    WiFi.config(ip, gateway, subnet);
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print(" WiFi connected - IP address: ");
  Serial.println(WiFi.localIP());
}

// Functions that check door status and publish an update when called

void publish_door1_status() {
  if (digitalRead(door1_statusPin) == LOW) {
      Serial.print(door1_alias);
      Serial.print(" Pushed! Publishing to ");
      Serial.print(mqtt_door1_status_topic);
      Serial.println("...");
      client.publish(mqtt_door1_status_topic, "ON", true);
  }
  else {
      Serial.print(door1_alias);
      Serial.print(" Released! Publishing to ");
      Serial.print(mqtt_door1_status_topic);
      Serial.println("...");
      client.publish(mqtt_door1_status_topic, "OFF", true);
  }
}

void publish_door2_status() {
  if (digitalRead(door2_statusPin) == LOW) {
      Serial.print(door2_alias);
      Serial.print(" Pushed! Publishing to ");
      Serial.print(mqtt_door2_status_topic);
      Serial.println("...");
      client.publish(mqtt_door2_status_topic, "ON", true);
  }
  else {
      Serial.print(door2_alias);
      Serial.print(" Released! Publishing to ");
      Serial.print(mqtt_door2_status_topic);
      Serial.println("...");
      client.publish(mqtt_door2_status_topic, "OFF", true);
  }
}

void check_door1_status() {
  int currentStatusValue = digitalRead(door1_statusPin);
  if (currentStatusValue != door1_lastStatusValue) {
    unsigned int currentTime = millis();
    if (currentTime - door1_lastSwitchTime >= debounceTime) {
      publish_door1_status();
      door1_lastStatusValue = currentStatusValue;
      door1_lastSwitchTime = currentTime;
    }
  }
}

void check_door2_status() {
  int currentStatusValue = digitalRead(door2_statusPin);
  if (currentStatusValue != door2_lastStatusValue) {
    unsigned int currentTime = millis();
    if (currentTime - door2_lastSwitchTime >= debounceTime) {
      publish_door2_status();
      door2_lastStatusValue = currentStatusValue;
      door2_lastSwitchTime = currentTime;
    }
  }
}

void publish_rssi_status(char* value) {
  Serial.print("WiFi RSSI Status");
  Serial.print(" Pushed! Publishing to ");
  Serial.print(mqtt_wifi_rssi_status_topic);
  Serial.println("...");
  Serial.println(value);
  client.publish(mqtt_wifi_rssi_status_topic, value, true);
}

void check_rssi_status() {
  int CurrentPeriod = millis()/(TXinterval*1000);
    if (CurrentPeriod != LastPeriod) {             // interval elapsed ?
      LastPeriod = CurrentPeriod;
      long rssi = WiFi.RSSI();
      char* buff_msg = "";
      sprintf(buff_msg, "%d", abs(rssi));
      if (rssi != wifi_rssi_lastValue) {
        publish_rssi_status(buff_msg);
        wifi_rssi_lastValue = rssi;
      }        
  }
}

void publish_birth_message() {
  // Publish the birthMessage
  Serial.print("Publishing birth message \"");
  Serial.print(birthMessage);
  Serial.print("\" to ");
  Serial.print(availabilityTopic);
  Serial.println("...");
  client.publish(availabilityTopic, birthMessage, true);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(mqtt_clientId, mqtt_username, mqtt_password, availabilityTopic, 0, true, lwtMessage)) {
      Serial.println("Connected!");

      // Publish the birth message on connect/reconnect
      publish_birth_message();

      // Subscribe to the action topics to listen for action messages
      Serial.print("Subscribing to ");
      Serial.print(mqtt_door1_action_topic);
      Serial.println("...");
      client.subscribe(mqtt_door1_action_topic);

      Serial.print("Subscribing to ");
      Serial.print(mqtt_door2_action_topic);
      Serial.println("...");
      client.subscribe(mqtt_door2_action_topic);

      // Publish the current door status on connect/reconnect to ensure status is synced with whatever happened while disconnected
      publish_door1_status();
      publish_door2_status();
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(door1_statusPin, INPUT_PULLUP);
  // Update variable with current door state
  door1_lastStatusValue = digitalRead(door1_statusPin);
  pinMode(door2_statusPin, INPUT_PULLUP);
  // Update variable with current door state
  door2_lastStatusValue = digitalRead(door2_statusPin);
  // Setup serial output, connect to wifi, connect to MQTT broker, set MQTT message callback
  Serial.begin(115200);
  Serial.println("Starting DoorBellSwitch...");
  setup_wifi();
  client.setServer(mqtt_broker, 1883);
}

void loop() {
  // Connect/reconnect to the MQTT broker and listen for messages
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  // Check door open/closed status each loop and publish changes
  check_door1_status();
  check_door2_status();
  check_rssi_status(); 
}
