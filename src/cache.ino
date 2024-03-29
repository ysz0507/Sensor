#include <Arduino.h>
#include "server.h"
#include "cache.h"

long long lastTime;
long lastTimeSync = -1000L * 60L * 60L * 24L * 7L;

String lastSite;

long long getTime()
{
    return lastTime + (millis() - lastTimeSync);
}

void refreshTime()
{
    if (millis() - lastTimeSync > 1000 * 60 * 2)
    {
        Serial.println("Refreshing time");
        long long newTime = getCurrentTime();
        if (newTime == -1)
        {
            Serial.println("Time refresh aborted\n");
            return;
        }
        lastTime = newTime;
        lastTimeSync = millis();
        Serial.println("Time refreshed: " + String(lastTime) + "\n");
    }
}

void initializeSites()
{
    Serial.println("Initializing sites");
    refreshSite(&lastSite, "index.html");
    Serial.println("Sites initialized\n");
}

String getSite()
{
    if (lastSite == "")
    {
        refreshSite(&lastSite, "index.html");
    }
    return lastSite == "" ? "<h1>Could not load main site into cache!</h1><a href=\"/all.json\">View all data</a> <br> <a href=\"/current.json\">View current data</a>" : lastSite;
}

void refreshSite(String *cache, const char *path)
{
    Serial.println("Refreshing site");
    for (int i = 0; i < 3; i++)
    {
        Serial.println(String("attempt ") + String(i + 1));
        *cache = getWebsite(path);
        if (*cache != "")
        {
            break;
        }
    }
    if (*cache == "")
    {
        Serial.println("Site refresh aborted\n");
    }
    else
    {
        Serial.println("Site refreshed: " + String((*cache).length()) + "chars\n");
    }
}
