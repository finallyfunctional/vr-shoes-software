#include <Arduino.h>
#include <Preferences.h>
#include "../include/VrShoeConfiguration.h"

VrShoeConfiguration config;
Preferences preferences;

void setup() 
{
  Serial.begin(9600);
  preferences.begin("VR-Shoe");
  config.initialize(preferences);
}

void loop() 
{
  config.getSensors()->updateSensors();
  config.getCommunicator()->update();
}
