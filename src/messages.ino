
/*
I created a header file for the api keys of callmebot.
The content is looks similar to:

#pragma once
#include <Arduino.h>
String simon_callmebot = "https://signal.callmebot.com/signal/send.php?phone=nvkdhgk3-729g-2746-24j5-286nfjs73j25&apikey=276947&text=";
String joy_callmebot = "https://signal.callmebot.com/signal/send.php?phone=khnsktzh-gks4-9697-kjl9-ljjsb3j7mssf4&apikey=96746&text=";
*/
#include "keys.h"

#include "messages.h"
#include "server.h"
#include "sensor.h"

/*
    Input:
    {
    "temperature": 20.73,
    "relHumidity": 54.73,
    "time": 1710362790183
    }
*/

String lastMessage = "";

void messageLoop(sensors_event_t lastTemperature, sensors_event_t lastHumitity, String name)
{
    float humidity = lastHumitity.relative_humidity, temperature = lastTemperature.temperature;

    String message = name + String("%3A+");
    if (humidity < 30)
    {
        if (temperature > 28)
        {
            // Mach mal die Heizung aus
            message += String("Vergiss+nicht+die+Heiung+auszuschalten%21");
        }
        if (temperature < 20)
        {
            // Fenster zu!
            message += String("Fenster+schließen+nicht+vergessen");
        }
    }
    else if (humidity > 70)
    {
        if (temperature < 20)
        {
            // Achtung Schimmelgefahr!
            message += String("Achtung+Schimmelgefahr!+Am+besten+stoßl&uuml;ften+und+danach+die+Heizung+anschalten.");
        }
        else if (temperature < 28)
        {
            // Du solltest mal lüften
            message += String("L&uuml;ften+nicht+vergessen");
        }
        else
        {
            // Nach dem duschen lüften nicht vergessen
            message += String("Nach+dem+Duschen+unbedingt+l&uuml;ften%21");
        }
    }
    if (message == lastMessage || message == name + String("%3A+"))
    {
        return;
    }
    lastMessage = message;
    message += String("%0ATemperatur%3A+") + String(temperature) + String("°C");
    message += String("%0ARelative Luftfeuchtigkeit%3A+") + String(humidity) + String("r.H.");
    Serial.println(message);
    requestUrl(simon_callmebot + message);
    // requestUrl(joy_callmebot + message);
}