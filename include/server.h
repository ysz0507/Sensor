#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer *setupServer();
void serverLoop();
String getWebsite(const char *path);
long long getCurrentTime();