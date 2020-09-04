#include "../../../include/Communicators/Bluetooth/BluetoothCommunicator.h"

void BluetoothCommunicator::initializeCommunication()
{
    serialBt.begin(deviceId);
}

void BluetoothCommunicator::processMessages()
{
    while(serialBt.available())
    {
        String message = serialBt.readStringUntil('\n');
        handleMessage(message);
    }
}

void BluetoothCommunicator::sendMessage(String message)
{
    serialBt.println(message);
}