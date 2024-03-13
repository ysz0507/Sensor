#pragma once

#include <Arduino.h>
#include <Adafruit_AHTX0.h>

struct Measurement
{
    String json;
    sensors_event_t temperature;
    sensors_event_t humidity;
    long long time;
};

void setupSensor();
Measurement getSensorData();
String getAllMeasurements();
Measurement addMeasurement();
