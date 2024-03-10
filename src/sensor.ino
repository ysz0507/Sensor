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

String *measurements[] = {new String(), new String(), new String(), new String(), new String(), new String(), new String(), new String(), new String(), new String()};

String getSensorData()
{
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);
    // fillRandomData(&humidity, &temp);

    String jsonObjectString = "{\n";
    jsonObjectString += "\"temperature\": " + String(temp.temperature) + ",\n";
    jsonObjectString += "\"relHumidity\": " + String(humidity.relative_humidity) + ",\n";
    jsonObjectString += "\"time\": " + String(getTime()) + "\n";
    jsonObjectString += "}";

    return jsonObjectString;
}

String getAllMeasurements()
{
    String jsonListString = "[";
    for (int i = 0; i < sizeof(measurements) / 4; i++)
    {
        if (*(measurements[i]) == "")
        {
            continue;
        }
        jsonListString += *(measurements[i]);
        if (i != (sizeof(measurements) / 4) - 1)
        {
            jsonListString += ",\n";
        }
    }
    jsonListString += "]";
    return jsonListString;
}

void addMeasurement()
{
    Serial.println("Adding automated measurement");
    free(measurements[0]);
    for (int i = 0; i < (sizeof(measurements) / 4) - 1; i++)
    {
        measurements[i] = measurements[i + 1];
    }
    String *newMeasurement = new String(getSensorData());
    measurements[(sizeof(measurements) / 4) - 1] = newMeasurement;
}

void fillRandomData(sensors_event_t *humidity, sensors_event_t *temp)
{
    humidity->relative_humidity = random(30, 60);
    temp->temperature = random(20, 30);
}
