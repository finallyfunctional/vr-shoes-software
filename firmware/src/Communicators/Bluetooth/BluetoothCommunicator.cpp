#include "BluetoothCommunicator.h"

void BluetoothCommunicator::initialize()
{
    serialBt.begin("VR-Shoe");
}

void BluetoothCommunicator::processMessages()
{
    String command = parseCommand();
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
    if(commandId.equals(INIT_BLUETOOTH_MESH))
    {
        initializeBluetoothMesh(json);
    }
    json.clear();
}

String BluetoothCommunicator::parseCommand()
{
    if (serialBt.available()) 
    {
        return parseBluetoothSerialCommand();
    }
    return ""; //More to be added later
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

/*
Expected information is in json format. This info is used to start up a bluetooth mesh.
Example:
{
  "command":"initialize-bluetooth-mesh",
  "your-id": "vr-shoe-1",
  "other-shoe-id":"vr-shoe-2",
  "my-id": "android-master",
  "ssid": "vr-shoes-mesh-102354",
  "password":"longpassword123456789",
  "port":"801"
}
*/
void BluetoothCommunicator::initializeBluetoothMesh(StaticJsonDocument<200> meshInformation)
{
    String ssid = meshInformation["ssid"];
    String password = meshInformation["password"];
    int port = meshInformation["port"];
    String name = meshInformation["your-id"];

    mesh.setDebugMsgTypes(ERROR | DEBUG | CONNECTION);
    mesh.init(ssid, password, &userScheduler, port);
    mesh.setName(name);

    pcOrMobileId = meshInformation["my-id"].as<String>();
    otherShoeId = meshInformation["other-shoe-id"].as<String>();
}