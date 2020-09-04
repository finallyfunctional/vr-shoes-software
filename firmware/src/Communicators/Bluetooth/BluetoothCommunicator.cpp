#include "../../../include/Communicators/Bluetooth/BluetoothCommunicator.h"

void BluetoothCommunicator::initializeCommunication()
{
    serialBt.begin("VR-Shoe-" + deviceId);
}

void BluetoothCommunicator::processMessages()
{
    if(!serialBt.available())
    {
        return;
    }
    String command = serialBt.readStringUntil('\n');
    DeserializationError error = deserializeJson(json, command);
    if (error) 
    {
        serialBt.println("Could not parse Command: ");
        serialBt.println(command);
        return;
    }

    String commandId = json["command"].as<String>();
    if(commandId.equals(Commands::PING))
    {
        serialBt.print("ping received from VR-Shoe-");
        serialBt.println(deviceId);
    }
    json.clear();
}