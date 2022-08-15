#ifndef SensorDataMessenger_h
#define SensorDataMessenger_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include "vrShoeSensorData.h"

class SensorDataMessenger {
    public:
      static void constructMessage(VrShoeSensorData data, String& message);
      static void serialPrintSensorData(VrShoeSensorData data);

    private:
      static StaticJsonDocument<500> json;

};

#endif