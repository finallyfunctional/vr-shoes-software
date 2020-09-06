#include "../../include/Communicators/Communicator.h"

const String Communicator::DEVICE_ID_PREFIX = "VR-Shoe-";
const char* Communicator::DEVICE_ID_KEY = "device-id";
const char Communicator::MESSAGE_TERMINATOR = '\n';

void Communicator::initialize(Preferences preferences)
{
    deviceId = preferences.getString(DEVICE_ID_KEY);
    if(deviceId == NULL || deviceId.equals(""))
    {
        deviceId = DEVICE_ID_PREFIX + String(random(100000, 1000000));
        preferences.putString(DEVICE_ID_KEY, deviceId);
    }
    initializeCommunication();
}

void Communicator::handleMessage(String message)
{
    DeserializationError error = deserializeJson(json, message);
    if (error) 
    {
        json.clear();
        json["error"] = 400;
        json["message"] = "Could not parse message - " + message;
        String reply;
        serializeJson(json, reply);
        sendMessage(reply);
        return;
    }
    String commandId = json["command"].as<String>();
    if(commandId.equals(Messages::PING))
    {
        ping();
    }
    else 
    {
        json.clear();
        json["error"] = 400;
        json["message"] = "Unknown command - " + commandId;
        String reply;
        serializeJson(json, reply);
        sendMessage(reply);
    }
    json.clear();
}

void Communicator::ping()
{
    json.clear();
    json["command"] = Messages::PING;
    json["reply"] = true;
    json["message"] = "Ping recieved on " + deviceId;
    String reply;
    serializeJson(json, reply);
    sendMessage(reply);
}