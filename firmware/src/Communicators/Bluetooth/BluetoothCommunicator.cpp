#include "../../../include/Communicators/Bluetooth/BluetoothCommunicator.h"

void BluetoothCommunicator::initialize()
{
    serialBt.begin("VR-Shoe");
}

void BluetoothCommunicator::processMessages()
{
    String command = parseBluetoothSerialCommand();
    if(command == NULL || command == "")
    {
        return;
    }
    DeserializationError error = deserializeJson(json, command);
    if (error) 
    {
        return; //TODO
    }

    String commandId = json["command"].as<String>();
    if(commandId.equals(PING))
    {
        serialBt.println("ping received");
    }
    json.clear();
}

String BluetoothCommunicator::parseBluetoothSerialCommand()
{
    String command = "";
    while(serialBt.available())
    {
        char c = serialBt.read();
        command += c;
    }
    return command;
}