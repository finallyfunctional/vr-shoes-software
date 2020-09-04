#ifndef BluetoothCommunicator_h
#define BluetoothCommunicator_h

#include <Arduino.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include "../AbstractCommunicator.h"
#include "../Commands.h"

class BluetoothCommunicator : public AbstractCommunicator
{
    public:
    void initializeCommunication();
    void processMessages();

    private:

    BluetoothSerial serialBt;
    StaticJsonDocument<200> json;
};

#endif