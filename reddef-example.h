/*******************************************************/
/* MARKISOR - Control Blinds with ESP8266 over Redis   */
/*                                                     */
/* Copyright: Stefan Koch                              */
/* Version  : 1.0 / 2019-07-28                         */
/* License  : GPL v3                                   */
/* Tested on: ESP8266-ESP01                            */
/*                                                     */
/*******************************************************/
#ifndef REDDEF_H
#define REDDEF_H

#define WIFI_SSID       "magic-wifi"
#define WIFI_STATIC_IP  192,168,178,100
#define WIFI_IPMASK     255,255,255,0
#define WIFI_GW         192,168,178,1

#define REDIS_ADDR      "192.168.178.2"
#define REDIS_PORT      6379
#define REDIS_PASSWORD  ""

// Note: if needed the read and write keys can differ
#define REDIS_READ_KEY  "your-redis-rkey"
#define REDIS_WRITE_KEY "your-redis-wkey"
#define REDIS_KEY_TGL_A "0"
#define REDIS_KEY_TGL_B "1"

#endif
