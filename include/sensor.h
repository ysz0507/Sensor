#pragma once

#include <Arduino.h>
#include <Adafruit_AHTX0.h>

void setupSensor();
String getSensorData();
String getAllMeasurements();
const char *addMeasurement();
