#include <Arduino.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include "vrShoeSensorData.h"
#include "floatUtils.h"

class SerialBluetooth {
    public:
      bool initialize();
      void update(VrShoeSensorData data);

    private:
      String getShoeId();
      void constructMessage(VrShoeSensorData data);

      BluetoothSerial serialBt;
      Preferences blPreferences;
      StaticJsonDocument<500> json;

      static const String DEVICE_ID_PREFIX;
      static const char* SHOE_ID_KEY;
};