#ifndef BluetoothCommunicator_h
#define BluetoothCommunicator_h

#include <Arduino.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include "../ICommunicator.h"
#include "../Commands.h"

class BluetoothCommunicator : ICommunicator
{
    public:
    void initialize();
    void processMessages();

    private:

    BluetoothSerial serialBt;
    StaticJsonDocument<200> json;
};

#endif