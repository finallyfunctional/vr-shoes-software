#include <Arduino.h>
#include "../include/Communicators/ICommunicator.h"
#include "../include/Communicators/Bluetooth/BluetoothCommunicator.h"

BluetoothCommunicator bluetoothCommunicator;

void setup() 
{
  Serial.begin(115200);
  bluetoothCommunicator.initialize();
}

void loop() 
{
  bluetoothCommunicator.processMessages();
}
