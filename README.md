# DoorBellSwitch

Double push-type remote switch.

## Introduction

Recently, I'm interested in home automation and more specifically OpenHab.

As I find that some products are shamefully expensive, and that the world of electronics allows us to create them at a reduced price.

As in the past, I was interested in the world of arduino.
I discovered ESP8266 modules.
They are not expensive, they are not bulky, they do not consume much and can work with arduino code that I already knew.
And Sherry On The Cake, they can speak the "MOSQUITTO" thanks to the MQTT protocol that OpenHab also knows.

## Destination of this project

Home Automation and more specifically for OpenHab, but other domotics project can probably also work.

## Material and knowledge needed

### 1. Material

Wifi Shield : ESP-01S ESP8266<BR>
Power Supply : Hi-Link HLK-PM03 AC-DC 220V to 3.3V Mini Power Supply Module<BR>
Resistor :  1 KOhm<BR>
2 Simple Push Button Normaly Open (Contact Free), One for Door Bell, One Contact when the door is open.<BR>

### 2. Knowledge

Arduino language [Here](https://www.arduino.cc/)<BR>
ESP-01 Flashing [Here](http://cordobo.com/2300-flash-esp8266-01-with-arduino-uno/)<BR>
MQTT "Mosquitto" [Here](http://www.steves-internet-guide.com/mosquitto-broker/)<BR> 
OpenHab items and Rules in text files. [Here](https://www.openhab.org/docs/)<BR>

## Sub Folder Content

Arduino-Code : Code Arduino and config file for your own wifi and mosquitto parameter<BR>
Fritzing : Download Fritzing [Here](http://fritzing.org/download/) to view connecting schema.<BR>
OpenHab : Items, Rules, Transform Files<BR>
Gerber : If you want to make a beautifull PCB. I'm not sponsorized but [this one](https://www.seeedstudio.io/fusion_pcb.html) is cheap. 
