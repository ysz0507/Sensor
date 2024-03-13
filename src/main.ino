#include "sensor.h"
#include "server.h"
#include "cache.h"
#include "messages.h"

long long lastMeasurement;
const int led = 2;

void setup()
{
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
    lastMeasurement = millis();
    Serial.begin(115200);

    // Sensor Setup
    setupSensor();

    // Server Setup
    ESP8266WebServer *server = setupServer();

    server->on("/", [server]()
               {String website = getSite(); 
                server->send(200, "text/html", website); });
    server->on("/authorize.html", [server]()
               {String website = getAuthSite(); 
                server->send(200, "text/html", website); });
    server->on("/status.json", [server]()
               {server->sendHeader("Access-Control-Allow-Origin", "*");
                server->send(200, "text/html", "I am online"); });
    server->on("/current.json", [server]()
               {server->sendHeader("Access-Control-Allow-Origin", "*");
                server->send(200, "application/json", getSensorData()); });
    server->on("/all.json", [server]()
               {server->sendHeader("Access-Control-Allow-Origin", "*");
                server->send(200, "application/json", getAllMeasurements()); });

    server->onNotFound([server]()
                       { server->send(404, "text/plain", "Not found"); });
    server->begin();

    // Cache
    initializeSites();
    refreshTime();

    // Time
    Serial.printf("setup time: %.4fs\n\n", (millis() - lastMeasurement) / 1000.0);
    lastMeasurement = getTime();
    digitalWrite(led, HIGH);

    addMeasurement();
    addMeasurement();
    addMeasurement();
    addMeasurement();
    addMeasurement();
    addMeasurement();
    addMeasurement();
    addMeasurement();
    addMeasurement();
    addMeasurement();
}

void loop()
{
    digitalWrite(led, LOW);
    refreshTime();
    serverLoop();
    if (getTime() - lastMeasurement > 1000 * 10)
    {
        const char *measurement = addMeasurement();
        messageLoop(measurement);
        lastMeasurement = getTime();
    }
    digitalWrite(led, HIGH);
    delay(500);
}
