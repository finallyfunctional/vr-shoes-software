#include <Arduino.h>
#include <Preferences.h>
#include "../include/VrShoeConfiguration.h"

VrShoeConfiguration config;
Preferences preferences;

void setup() 
{
  Serial.begin(9600);
  Serial1.begin(115200, SERIAL_8N1, 26, 27);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  preferences.begin("VR-Shoe");
  config.initialize(preferences);
}

void loop() 
{
  config.getSensors()->updateSensors();
  config.getCommunicator()->update();
}
