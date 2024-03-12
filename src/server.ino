#include "server.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

const char *ssid = "Soziales Netzwerk";
const char *password = "erdghu643d";
ESP8266WebServer server(80);
const char *mdns_name = "esp8266";

ESP8266WiFiMulti WiFiMulti;

ESP8266WebServer *setupServer()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.print("\nConnected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    return &server;
}

void serverLoop()
{
    server.handleClient();
}

String getWebsite(const char *path)
{
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
    client->setInsecure();

    HTTPClient https;
    String payload = "";

    if (https.begin(*client, "https://ysz0507.github.io/Sensor/website/html/" + String(path)))
    {
        int httpCode = https.GET();
        if (httpCode > 0)
        {
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
            {
                payload = https.getString();
            }
        }
        else
        {
            Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }

        https.end();
    }
    else
    {
        Serial.printf("[HTTPS] Unable to connect\n");
    }

    return payload;
}

long long getCurrentTime()
{
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
    client->setInsecure();

    HTTPClient https;
    String payload = "";

    for (int i = 0; i < 3; i++)
    {
        https.begin(*client, "https://worldtimeapi.org/api/timezone/Europe/Berlin");
        int httpCode = https.GET();
        if (httpCode == HTTP_CODE_OK)
        {
            payload = https.getString();
            payload = payload.substring(payload.indexOf("unixtime") + 10);
            payload = payload.substring(0, payload.indexOf(","));
            https.end();
            break;
        }
        else
        {
            Serial.printf("[HTTP] GET... failed, error: %s mit code: %d\n", https.errorToString(httpCode).c_str(), httpCode);
            https.end();
        }
    }
    if (payload == "")
    {
        return -1;
    }
    return strtoll(payload.c_str(), NULL, 10) * 1000LL;
}