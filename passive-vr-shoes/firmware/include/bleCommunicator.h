#ifndef BleCommunicator_h
#define BleCommunicator_h

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include "vrShoeSensorData.h"
#include "floatUtils.h"

class BleCommunicator {
    public:
      bool initialize();
      void update(VrShoeSensorData data);

    private:
      String getShoeId();
      void constructMessage(VrShoeSensorData data);

      BLEServer* bleServer;
      BLEService* bleService;
      BLEAdvertising* bleAdvertising;
      BLEAdvertisementData bleAdvertisementData;
      BLECharacteristic* bleCharacteristic;
      Preferences blPreferences;
      StaticJsonDocument<500> json;

      static const char* SERVICE_UUID;
      static const char* CHARACTERISTIC_UUID;
      static const char* DEVICE_ID_PREFIX;
      static const char* SHOE_ID_KEY;
};

#endif