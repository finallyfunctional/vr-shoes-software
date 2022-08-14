#ifndef MessageFormatter_h
#define MessageFormatter_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include "vrShoeSensorData.h"

class MessageFormatter {
    public:
      static void constructMessage(VrShoeSensorData data, String& message) {
          json.clear();
          json["type"] = "data";
          json["qw"] = FloatUtils::roundFloatToTwoDecimalPlaces(data.qw);
          json["qx"] = FloatUtils::roundFloatToTwoDecimalPlaces(data.qx);
          json["qy"] = FloatUtils::roundFloatToTwoDecimalPlaces(data.qy);
          json["qz"] = FloatUtils::roundFloatToTwoDecimalPlaces(data.qz);
          json["ticks"] = data.encoderTicks;

          serializeJson(json, message);
      }

    private:
      static StaticJsonDocument<500> json;
};

#endif