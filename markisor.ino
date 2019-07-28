/*******************************************************/
/* MARKISOR - Control Blinds with ESP8266 over Redis   */
/*                                                     */
/* Copyright: Stefan Koch                              */
/* Version  : 1.0 / 2019-07-28                         */
/* License  : GPL v3                                   */
/* Tested on: ESP8266-ESP01                            */
/*                                                     */
/*******************************************************/
/* Description                                         */
/*                                                     */
/* This application is intended to get powered-on      */
/* then do some work and then get powered off again.   */
/*                                                     */
/* This application implements a toggle switch.        */
/* When started it connects to WIFI with pre-set       */
/* IP address to accelerate the connection.            */
/* If the connection to Wifi fails then it asks for    */
/* the password on the serial console which is then    */
/* stored in EEPROM subsequently                       */
/*                                                     */
/* Then it toogles a given redis-key (see reddef.h)    */
/* for an example. The success is marked by turning    */
/* on the blue LED on the board.                       */
/*                                                     */
/* In my case this redis key triggers a command on     */
/* my somfy system to pull or push the blinds.         */
/*                                                     */
/*******************************************************/

//define the following key to enable more logging on 
//serial console
//#define IONODE_DEBUG 1
#include <EEPROM.h>
#include <Redis.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

//here are our application specific defines
#include "reddef.h"

const char* ssid = WIFI_SSID;
char password[20];
String readLineData;

IPAddress ip(WIFI_STATIC_IP);
IPAddress gateway(WIFI_GW); 
IPAddress subnet(WIFI_IPMASK);

void debugPrint(String what) {
#ifdef IONODE_DEBUG
  Serial.println(what);
#endif
}

/****************************************************************
   This application only lives in the setup section of a arduino
   sketch because the only goal is to toggle a redis-key. After
   the toggle the controller will be powered-off
 ****************************************************************/
void setup() {
  /* ------------------------------------
   * Initialize some hw
   */
  EEPROM.begin(512);
  delay(10);
  Serial.begin(115200);
  delay(10);
  debugPrint("");

  /* ------------------------------------
   * read wifi password and start wifi
   */
  readEEPROM(0, password, sizeof(password));
  debugPrint(password);

  // Connect to WiFi network
  debugPrint("");
  debugPrint("");
  debugPrint("Connecting to ");
  debugPrint(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);

  int maxWait = 20;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    debugPrint(".");
    
    maxWait--;
    if (maxWait <= 0) {
      getSavePassword();
      Serial.print("please restart");
      while (1);
    }
  }
  debugPrint("");
  debugPrint("WiFi connected");

  /* ------------------------------------
   * initialize Redis and connect
   */
  WiFiClient redisConn;
  if (!redisConn.connect(REDIS_ADDR, REDIS_PORT)) {
        Serial.println("Failed to connect to the Redis server!");
        return;
  }

  Redis redis(redisConn);
  auto connRet = redis.authenticate(REDIS_PASSWORD);
  
  if (connRet == RedisSuccess){
    debugPrint("Connected to the Redis server!");
  } else {
    Serial.printf("Failed to authenticate to the Redis server! Errno: %d\n", (int)connRet);
    return;
  }

  /* ------------------------------------
   * here is the logic
   */
  String val  = redis.get(REDIS_READ_KEY);
  debugPrint("read-key = " + val);
  String keyIn = REDIS_KEY_TGL_A;

  if (val == keyIn) {
    redis.set(  REDIS_WRITE_KEY, REDIS_KEY_TGL_B);
    debugPrint( "toggle to B");
  } else {
    redis.set(  REDIS_WRITE_KEY, REDIS_KEY_TGL_A);
    debugPrint( "toggle to A");
  }
 
  /* ------------------------------------
   * Initialize some hw
   *
   */
  redisConn.stop();
  debugPrint("Connection closed!");

  pinMode(1, OUTPUT);
  digitalWrite(1, LOW);
}

/* No use */
void loop() {
}

/*******************************************************************/
/* some utils */
/*******************************************************************/
void readLine() {
  while (1) {
    while (Serial.available() <= 0);
    char recieved = Serial.read();
    debugPrint(".");
    readLineData += recieved; 

    // Process message when new line character is recieved
    if (recieved == '\n') {
      return;
    }
  }
}

void getSavePassword()
{
  Serial.print("Enter Password");  
  readLine();
  Serial.print("OK, save to EEPROM");
  
  writeEEPROM(0, readLineData);
  Serial.print("OK, saved");
}

void writeEEPROM(char addr, String data)
{
  int len = data.length();
  int i;
  for(i = 0; i < len; i++) {
    EEPROM.write(addr + i, data[i]);
  }
  
  EEPROM.write(addr + len,'\0');
  EEPROM.commit();
}

void readEEPROM(char addr, char buf[], int len)
{
  int i = 0;
  buf[0] = 0;
  
  for (i = 0; i < len; i++) {
    buf[i] = EEPROM.read(addr + i);
    if (buf[i] == 0) {
      break;
    }
    if (!isPrintable(buf[i])) {
      buf[i] = 0;
      break;
    }
  }
  
  if (i >= len) {
    buf[len - 1] = 0;
  }
  
  return;
}
