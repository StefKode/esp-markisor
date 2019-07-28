# Markisor
MARKISOR - Control Blinds with ESP8266 over Redis

* Copyright: Stefan Koch
* Version  : 1.0 / 2019-07-28
* License  : GPL v3
* Tested on: ESP8266-ESP01

# Description

This application is intended to get powered-on
then do some work and then get powered off again.

This application implements a toggle switch.
When started it connects to WIFI with pre-set
IP address to accelerate the connection.
If the connection to Wifi fails then it asks for
the password on the serial console which is then
stored in EEPROM subsequently

Then it toogles a given redis-key (see reddef.h)
for an example. The success is marked by turning
on the blue LED on the board.

In my case this redis key triggers a command on
my somfy system to pull or push the blinds.

# Video
Here you can see a cardboard prototype box. The ESP sticks out so you can see the LEDs. It takes only a few seconds before the ESP connects to the Wifi and toggles the redis key.

[![Markisor Video](https://i.ytimg.com/vi/8Q8e2SZQTwA/hqdefault.jpg)](https://youtu.be/8Q8e2SZQTwA "Markisor Video")
