#include <Adafruit_AHTX0.h>

#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

#include "sensor.h"
#include "cache.h"

Adafruit_AHTX0 aht;

void setupSensor()
{
    if (!aht.begin())
    {
        Serial.println("Could not find AHT? Check wiring");
    }
    else
    {
        Serial.println("AHT10 or AHT20 found");
    }
}

const unsigned int NUMBER_OF_SAVED_ENTRIES = 10;
char **measurements = (char **)calloc(NUMBER_OF_SAVED_ENTRIES, sizeof(char *));

String getSensorData()
{
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);
    // fillRandomData(&humidity, &temp);

    String jsonObjectString = String("{\n");
    jsonObjectString += String("\"temperature\": ") + String(temp.temperature) + String(",\n");
    jsonObjectString += String("\"relHumidity\": ") + String(humidity.relative_humidity) + String(",\n");
    jsonObjectString += String("\"time\": ") + String(getTime()) + String("\n");
    jsonObjectString += String("}");

    return jsonObjectString;
}

String getAllMeasurements()
{
    String jsonListString = "[";
    for (unsigned int i = 0; i < NUMBER_OF_SAVED_ENTRIES; i++)
    {
        Serial.println(measurements[i]);
        if (measurements[i] == 0x0)
        {
            continue;
        }
        jsonListString += String(measurements[i]);
        if (i != NUMBER_OF_SAVED_ENTRIES - 1)
        {
            jsonListString += String(",\n");
        }
    }
    jsonListString += String("]");
    return jsonListString;
}

const char *addMeasurement()
{
    Serial.println("Adding automated measurement");
    if (measurements[0] != 0x0)
    {
        free(measurements[0]);
    }
    for (unsigned int i = 0; i < NUMBER_OF_SAVED_ENTRIES - 1; i++)
    {
        measurements[i] = measurements[i + 1];
    }
    String sensorDataString = getSensorData();
    const char *newMeasurement = sensorDataString.c_str();
    char *ptr = (char *)malloc(200);
    measurements[NUMBER_OF_SAVED_ENTRIES - 1] = ptr;
    strcpy(measurements[NUMBER_OF_SAVED_ENTRIES - 1], newMeasurement);
    return measurements[NUMBER_OF_SAVED_ENTRIES - 1];
}

void fillRandomData(sensors_event_t *humidity, sensors_event_t *temp)
{
    humidity->relative_humidity = random(30, 60);
    temp->temperature = random(20, 30);
}
