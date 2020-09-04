#include <Arduino.h>
#include <Preferences.h>
#include "../include/Communicators/Bluetooth/BluetoothCommunicator.h"

BluetoothCommunicator bluetoothCommunicator;
Preferences preferences;

void setup() 
{
  Serial.begin(9600);
  preferences.begin("VR-Shoe");
  EepromStorage::initialize();
  bluetoothCommunicator.initialize(preferences);
}

void loop() 
{
  bluetoothCommunicator.processMessages();
}
