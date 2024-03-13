#pragma once

#include <Arduino.h>

long long getTime();
void refreshTime();

String getSite();
void refreshSite(String *cache);
void initializeSites();