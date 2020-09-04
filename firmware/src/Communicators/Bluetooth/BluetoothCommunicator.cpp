#include "../../../include/Communicators/Bluetooth/BluetoothCommunicator.h"

void BluetoothCommunicator::initialize()
{
    serialBt.begin("VR-Shoe");
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
        serialBt.println("ping received");
    }
    json.clear();
}