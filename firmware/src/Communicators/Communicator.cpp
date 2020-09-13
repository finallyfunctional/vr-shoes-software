#include "../../include/Communicators/Communicator.h"

const String Communicator::DEVICE_ID_PREFIX = "VR-Shoe-";
const char* Communicator::DEVICE_ID_KEY = "device-id";
const char Communicator::MESSAGE_TERMINATOR = '\n';

void Communicator::initialize(Preferences preferences, Sensors* sensors)
{
    this->sensors = sensors;
    deviceId = preferences.getString(DEVICE_ID_KEY);
    if(deviceId == NULL || deviceId.equals(""))
    {
        deviceId = DEVICE_ID_PREFIX + String(random(100000, 1000000));
        preferences.putString(DEVICE_ID_KEY, deviceId);
    }
    initializeCommunication();
    lastSensorDataMessageSent = "";
}

void Communicator::update()
{
    processMessages();
    sendSensorData(false);
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
    else if(commandId.equals(Messages::READ_SENSOR_DATA))
    {
        sendSensorData(true);
    }
    else if(commandId.equals(Messages::RESET_ORIGIN))
    {
        resetOrigin();
    }
    else if(commandId.equals(Messages::READ_DISTANCE_FROM_ORIGIN))
    {
        sendDistanceFromOrigin();
    }
    else if(commandId.equals(Messages::SET_RPM))
    {
        setRpm();
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

void Communicator::sendReply()
{
    String reply;
    serializeJson(json, reply);
    sendMessage(reply);
}

void Communicator::ping()
{
    json.clear();
    json["command"] = Messages::PING;
    json["reply"] = true;
    json[Messages::DEVICE_ID] = deviceId;
    json["message"] = "Ping recieved on " + deviceId;
    sendReply();
}

void Communicator::sendSensorData(bool force)
{
    json.clear();
    json["command"] = Messages::READ_SENSOR_DATA;
    json["frontButtonPressed"] = sensors->isFrontButtonPressed();
    json["rearButtonPressed"] = sensors->isRearButtonPressed();
    json[Messages::DEVICE_ID] = deviceId;

    Vector2D speed = sensors->getMovementTracker()->getSpeed();
    json["forwardSpeed"] = speed.getX();
    json["sidewaySpeed"] = speed.getY();

    String message;
    serializeJson(json, message);
    if(force || !message.equals(lastSensorDataMessageSent))
    {
        sendMessage(message);
    }
    lastSensorDataMessageSent = message;
}

void Communicator::resetOrigin()
{
    sensors->getMovementTracker()->resetOrigin();
    
    json.clear();
    json["command"] = Messages::RESET_ORIGIN;
    json["reply"] = true;
    json[Messages::DEVICE_ID] = deviceId;
    json["message"] = "Origin reset on " + deviceId;
    sendReply();
}

void Communicator::sendDistanceFromOrigin()
{
    Vector2D distance = sensors->getMovementTracker()->getDistanceFromOrigin();

    json.clear();
    json["command"] = Messages::READ_DISTANCE_FROM_ORIGIN;
    json["reply"] = true;
    json[Messages::DEVICE_ID] = deviceId;
    json["forwardDistance"] = distance.getX();
    json["sidewaysDistance"] = distance.getY();
    sendReply();
}

void Communicator::setRpm()
{
    float forwardRpm = json["forwardRpm"];
    float sidewayRpm = json["sidewayRpm"];

    sensors->getSpeedController()->setRpm(forwardRpm, sidewayRpm);

    json.clear();
    json["command"] = Messages::SET_RPM;
    json["reply"] = true;
    json[Messages::DEVICE_ID] = deviceId;
    json["message"] = "rpm set";
    sendReply();
}