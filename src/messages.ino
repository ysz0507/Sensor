
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

/*
    Input:
    {
    "temperature": 20.73,
    "relHumidity": 54.73,
    "time": 1710362790183
    }
*/
void messageLoop(const char *lastMeasurement)
{

    Serial.println(lastMeasurement);
    // requestUrl(simon_callmebot + String("Hi"));
    // requestUrl(joy_callmebot + String("Hi"));
}