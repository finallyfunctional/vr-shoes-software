#ifndef BluetoothCommunicator_h
#define BluetoothCommunicator_h

#include <Arduino.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include "../Communicator.h"

class BluetoothCommunicator : public Communicator
{
    public:
    void initializeCommunication();
    void processMessages();

    private:
    BluetoothSerial serialBt;

    protected:
    void sendMessage(String message);
};

#endif