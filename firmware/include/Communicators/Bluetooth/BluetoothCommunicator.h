#ifndef BluetoothCommunicator_h
#define BluetoothCommunicator_h

#include <Arduino.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include "../ICommunicator.h"

class BluetoothCommunicator : ICommunicator
{
    public:
    void initialize();
    void processMessages();

    private:
    String parseBluetoothSerialCommand();

    BluetoothSerial serialBt;
    StaticJsonDocument<200> json;

    const String PING = "ping"; 

};

#endif