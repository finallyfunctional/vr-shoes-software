#include "../../include/Communicators/Communicator.h"

const String Communicator::DEVICE_ID_PREFIX = "VR-Shoe-";
const char* Communicator::SHOE_ID_KEY = "device-id";
const char Communicator::MESSAGE_TERMINATOR = '\n';

void Communicator::initialize(Sensors* sensors)
{
    this->sensors = sensors;
    sendSensorDataTimer = new Timer(((unsigned long)100));
    sendSensorDataTimer->start();
    shoeId = VrShoePreferences.getString(SHOE_ID_KEY);
    if(shoeId == NULL || shoeId.equals(""))
    {
        shoeId = DEVICE_ID_PREFIX + String(random(100000, 1000000));
        VrShoePreferences.putString(SHOE_ID_KEY, shoeId);
    }
    initializeCommunication();
    lastSensorDataMessageSent = "";
}

void Communicator::update()
{
    if(isInitialized())
    {
        processMessages();
        String otherShoeId = sensors->getRemoteVrShoe()->shoeId;
        if(otherShoeId != NULL && !otherShoeId.equals("") && sendSensorDataTimer->timeIsUp())
        {
            sendSensorDataIfStale(otherShoeId);
        }    
    }
    else 
    {
        initializeCommunication();
    }
}

int Communicator::handleRecievedMessage(String message)
{
    json.clear();
    DeserializationError error = deserializeJson(json, message);
    if (error) 
    {
        json["error"] = 400;
        json["message"] = "Could not parse message - " + message;
        String reply;
        serializeJson(json, reply);
        return ResponseCodes::BAD_REQUEST;
    }
    String commandId = json["command"].as<String>();
    if(shouldForwardMessage())
    {
        return ResponseCodes::FORWARD_MESSAGE;
    }
    else if(commandId.equals(Messages::PING))
    {
        return ping();
    }
    else if(commandId.equals(Messages::SENSOR_DATA))
    {
        if(!json["get"].isNull() && json["get"].as<bool>())
        {
            return replyWithSensorData();
        }
        else
        {
            return recieveSensorData();
        }
        
    }
    else if(commandId.equals(Messages::RESET_ORIGIN))
    {
        return resetOrigin();
    }
    else if(commandId.equals(Messages::DISTANCE_FROM_ORIGIN))
    {
        return sendDistanceFromOrigin();
    }
    else if(commandId.equals(Messages::SET_RPM))
    {
        return setRpm();
    }
    else if(commandId.equals(Messages::SET_COMMUNICATION_MODE))
    {
        return setCommunicationMode();
    }
    else if(commandId.equals(Messages::OTHER_SHOE_ID))
    {
        setOtherShoeId();
    }
    else
    {
        int implResponse = handleImplementationSpecificMessage(commandId);
        if(implResponse != ResponseCodes::BAD_REQUEST)
        {
            return implResponse;
        }
        json.clear();
        json["error"] = 400;
        json["message"] = "Unknown command - " + commandId;
        String reply;
        serializeJson(json, reply);
        return ResponseCodes::BAD_REQUEST;
    }
}

int Communicator::ping()
{
    json.clear();
    json["command"] = Messages::PING;
    json["reply"] = true;
    json[MessageKeys::SHOE_ID] = shoeId;
    json["message"] = "Ping recieved on " + shoeId;
    return ResponseCodes::GOOD_REQUEST_SEND_REPLY;
}

int Communicator::replyWithSensorData()
{
    getSensorData();
    json["reply"] = true;
    String message;
    serializeJson(json, message);
    return ResponseCodes::GOOD_REQUEST_SEND_REPLY;
}

void Communicator::sendSensorDataIfStale(String sendingToshoeId)
{
    getSensorData();
    String message;
    serializeJson(json, message);
    if(!message.equals(lastSensorDataMessageSent))
    {
        json["to"] = sendingToshoeId;
        sendMessageTo(sendingToshoeId);
        lastSensorDataMessageSent = message;
        sendSensorDataTimer->start();
    }
}

int Communicator::recieveSensorData()
{
    sensors->getRemoteVrShoe()->frontButtonPressed = json["frontButtonPressed"];
    sensors->getRemoteVrShoe()->rearButtonPressed = json["rearButtonPressed"];
    sensors->getRemoteVrShoe()->forwardSpeed = json["forwardSpeed"];
    sensors->getRemoteVrShoe()->sidewaySpeed = json["sidewaySpeed"];
    return ResponseCodes::GOOD_REQUEST_NO_REPLY;
}

void Communicator::getSensorData()
{
    json.clear();
    json["command"] = Messages::SENSOR_DATA;
    json["frontButtonPressed"] = sensors->isFrontButtonPressed();
    json["rearButtonPressed"] = sensors->isRearButtonPressed();
    json[MessageKeys::SHOE_ID] = shoeId;

    Vector2D speed = sensors->getMovementTracker()->getSpeed();
    json["forwardSpeed"] = speed.getX();
    json["sidewaySpeed"] = speed.getY();
}

int Communicator::resetOrigin()
{
    sensors->getMovementTracker()->resetOrigin();

    json.clear();
    json["command"] = Messages::RESET_ORIGIN;
    json["reply"] = true;
    json[MessageKeys::SHOE_ID] = shoeId;
    json["message"] = "Origin reset on " + shoeId;

    return ResponseCodes::GOOD_REQUEST_SEND_REPLY;
}

int Communicator::sendDistanceFromOrigin()
{
    Vector2D distance = sensors->getMovementTracker()->getDistanceFromOrigin();

    json.clear();
    json["command"] = Messages::DISTANCE_FROM_ORIGIN;
    json["reply"] = true;
    json[MessageKeys::SHOE_ID] = shoeId;
    json["forwardDistance"] = distance.getX();
    json["sidewaysDistance"] = distance.getY();

    return ResponseCodes::GOOD_REQUEST_SEND_REPLY;
}

int Communicator::setRpm()
{
    float forwardRpm = json["forwardRpm"];
    float sidewayRpm = json["sidewayRpm"];

    sensors->getSpeedController()->setRpm(forwardRpm, sidewayRpm);

    json.clear();
    json["command"] = Messages::SET_RPM;
    json["reply"] = true;
    json[MessageKeys::SHOE_ID] = shoeId;
    json["message"] = "rpm set";

    return ResponseCodes::GOOD_REQUEST_SEND_REPLY;
}

int Communicator::setOtherShoeId()
{
    String otherShoeId = json["otherShoeId"];
    sensors->getRemoteVrShoe()->shoeId = otherShoeId;
    return ResponseCodes::GOOD_REQUEST_NO_REPLY;
}