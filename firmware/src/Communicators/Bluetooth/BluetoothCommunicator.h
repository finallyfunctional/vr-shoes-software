#ifndef BluetoothCommunicator_h
#define BluetoothCommunicator_h

#include <Arduino.h>
#include <BluetoothSerial.h>
#include <namedMesh.h>
#include <ArduinoJson.h>
#include "../ICommunicator.h"

class BluetoothCommunicator : ICommunicator
{
    public:
    void initialize();
    void processMessages();

    private:
    String parseCommand();
    String parseBluetoothSerialCommand();
    void initializeBluetoothMesh(StaticJsonDocument<200> meshInformation);

    namedMesh mesh;
    Scheduler userScheduler;
    BluetoothSerial serialBt;
    StaticJsonDocument<200> json;
    String otherShoeId;
    String pcOrMobileId;

    const String INIT_BLUETOOTH_MESH = "initialize-bluetooth-mesh"; 

};

#endif