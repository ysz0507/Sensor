#pragma once
#include "sensor.h"

void messageLoop(sensors_event_t lastTemperature, sensors_event_t lastHumitity, String name);