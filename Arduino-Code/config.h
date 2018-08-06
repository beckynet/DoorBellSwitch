/*   
 * DoorBellSwitch
 * ESP8266-based MQTT Door Bell Switch
 * Licensed under the MIT License, Copyright (c) 2018 Beeckmans.O
*/

// Wifi Parameters

#define WIFI_SSID "YOUR_WIFI_SID"
#define WIFI_PASSWORD "YOUR_WIFI_PWD"

// Static IP Parameters

#define STATIC_IP false
#define IP 10,0,1,200
#define GATEWAY 10,0,1,1
#define SUBNET 255,255,255,0

// MQTT Parameters

#define MQTT_BROKER "MOSQUITTO_IP"
#define MQTT_CLIENTID "doorbellswitch"
#define MQTT_USERNAME "MOSQUITTO_USERNAME"
#define MQTT_PASSWORD "MOSQUITTO_PWD"

// Wifi

#define MQTT_WIFI_RSSI_STATUS_TOPIC "doorbellswitch/wifi/rssi/status"

// Door Switch 1 Parameters

#define DOOR1_ALIAS "Door Switch 1"
#define MQTT_DOOR1_ACTION_TOPIC "doorbellswitch/door/1/action"
#define MQTT_DOOR1_STATUS_TOPIC "doorbellswitch/door/1/status"
#define DOOR1_STATUS_PIN 0

// Door Switch 2 Parameters

#define DOOR2_ALIAS "Door Switch 2"
#define MQTT_DOOR2_ACTION_TOPIC "doorbellswitch/door/2/action"
#define MQTT_DOOR2_STATUS_TOPIC "doorbellswitch/door/2/status"
#define DOOR2_STATUS_PIN 2