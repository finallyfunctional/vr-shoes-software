#include "../../include/Communicators/Communicator.h"

const String Communicator::DEVICE_ID_PREFIX = "VR-Shoe-";
const char* Communicator::SHOE_ID_KEY = "device-id";
const char Communicator::MESSAGE_TERMINATOR = '\n';

void Communicator::initialize(Sensors* sensors, AutoShoeController* shoeController)
{
    this->sensors = sensors;
    this->shoeController = shoeController;
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
        json[MessageKeys::ERROR] = ResponseCodes::BAD_REQUEST;
        String reply;
        serializeJson(json, reply);
        return ResponseCodes::BAD_REQUEST;
    }
    String commandId = json[MessageKeys::COMMAND].as<String>();
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
        if(!json[MessageKeys::GET].isNull() && json[MessageKeys::GET].as<bool>())
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
        return setOtherShoeId();
    }
    else if(commandId.equals(Messages::SHOE_SIDE))
    {
        if(!json[MessageKeys::GET].isNull() && json[MessageKeys::GET].as<bool>())
        {
            return getShoeSide();
        }
        else 
        {
            return setShoeSide();
        }
    }
    else if(commandId.equals(Messages::START_ALGORITHM))
    {
        return startAlgorithm();
    }
    else if(commandId.equals(Messages::STOP_ALGORITHM))
    {
        return stopAlgorithm();
    }
    else
    {
        int implResponse = handleImplementationSpecificMessage(commandId);
        if(implResponse != ResponseCodes::BAD_REQUEST)
        {
            return implResponse;
        }
        json.clear();
        json[MessageKeys::ERROR] = ResponseCodes::UNKNOWN_REQUEST;
        String reply;
        serializeJson(json, reply);
    }
    return ResponseCodes::UNKNOWN_REQUEST;
}

int Communicator::ping()
{
    json.clear();
    json[MessageKeys::COMMAND] = Messages::PING;
    json[MessageKeys::REPLY] = true;
    json[MessageKeys::SHOE_ID] = shoeId;
    return ResponseCodes::GOOD_REQUEST_SEND_REPLY;
}

int Communicator::replyWithSensorData()
{
    getSensorData();
    json[MessageKeys::REPLY] = true;
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
        json[MessageKeys::DESTINATION] = sendingToshoeId;
        sendMessageTo(sendingToshoeId);
        lastSensorDataMessageSent = message;
        sendSensorDataTimer->start();
    }
}

int Communicator::recieveSensorData()
{
    String otherShoeId = json[MessageKeys::SHOE_ID];
    String destination = json[MessageKeys::DESTINATION];
    if(!destination.equals(shoeId) || !sensors->getRemoteVrShoe()->shoeId.equals(otherShoeId))
    {
        return ResponseCodes::BAD_REQUEST;
    }
    sensors->getRemoteVrShoe()->frontButtonPressed = json[MessageKeys::FRONT_BUTTON_PRESSED];
    sensors->getRemoteVrShoe()->rearButtonPressed = json[MessageKeys::REAR_BUTTON_PRESSED];
    sensors->getRemoteVrShoe()->forwardSpeed = json[MessageKeys::FORWARD_SPEED];
    sensors->getRemoteVrShoe()->sidewaySpeed = json[MessageKeys::SIDEWAY_SPEED];
    return ResponseCodes::GOOD_REQUEST_NO_REPLY;
}

void Communicator::getSensorData()
{
    json.clear();
    json[MessageKeys::COMMAND] = Messages::SENSOR_DATA;
    json[MessageKeys::FRONT_BUTTON_PRESSED] = sensors->isFrontButtonPressed();
    json[MessageKeys::REAR_BUTTON_PRESSED] = sensors->isRearButtonPressed();
    json[MessageKeys::SHOE_ID] = shoeId;

    Vector2D speed = sensors->getMovementTracker()->getSpeed();
    json[MessageKeys::FORWARD_SPEED] = roundFloatToTwoDecimalPlaces(speed.getX());
    json[MessageKeys::SIDEWAY_SPEED] = roundFloatToTwoDecimalPlaces(speed.getY());
}

float Communicator::roundFloatToTwoDecimalPlaces(float number)
{
    return (float)(roundf(number * 100) / 100);
}

int Communicator::resetOrigin()
{
    sensors->getMovementTracker()->resetOrigin();

    json.clear();
    json[MessageKeys::COMMAND] = Messages::RESET_ORIGIN;
    json[MessageKeys::REPLY] = true;
    json[MessageKeys::SHOE_ID] = shoeId;

    return ResponseCodes::GOOD_REQUEST_SEND_REPLY;
}

int Communicator::sendDistanceFromOrigin()
{
    Vector2D distance = sensors->getMovementTracker()->getDistanceFromOrigin();

    json.clear();
    json[MessageKeys::COMMAND] = Messages::DISTANCE_FROM_ORIGIN;
    json[MessageKeys::REPLY] = true;
    json[MessageKeys::SHOE_ID] = shoeId;
    json[MessageKeys::FORWARD_DISTANCE] = roundFloatToTwoDecimalPlaces(distance.getX());
    json[MessageKeys::SIDEWAY_DISTANCE] = roundFloatToTwoDecimalPlaces(distance.getY());

    return ResponseCodes::GOOD_REQUEST_SEND_REPLY;
}

int Communicator::setRpm()
{
    float forwardRpm = json[MessageKeys::FORWARD_RPM];
    float sidewayRpm = json[MessageKeys::SIDEWAY_RPM];

    sensors->getSpeedController()->setRpm(forwardRpm, sidewayRpm);

    json.clear();
    json[MessageKeys::COMMAND] = Messages::SET_RPM;
    json[MessageKeys::REPLY] = true;
    json[MessageKeys::SHOE_ID] = shoeId;

    return ResponseCodes::GOOD_REQUEST_SEND_REPLY;
}

int Communicator::setOtherShoeId()
{
    String otherShoeId = json[MessageKeys::OTHER_SHOE_ID];
    sensors->getRemoteVrShoe()->shoeId = otherShoeId;
    return ResponseCodes::GOOD_REQUEST_NO_REPLY;
}

int Communicator::setShoeSide()
{
    int shoeSide = json[MessageKeys::SIDE];
    if(shoeSide == ShoeSides::LEFT)
    {
        VrShoePreferences.putInt(ShoeSides::SHOE_SIDE_KEY, shoeSide);
        sensors->getSpeedController()->inverseForwardDirection();
        sensors->getSpeedController()->inverseSidewayDirection();
        return ResponseCodes::GOOD_REQUEST_NO_REPLY;
    }
    else if(shoeSide == ShoeSides::RIGHT)
    {
        VrShoePreferences.putInt(ShoeSides::SHOE_SIDE_KEY, shoeSide);
        sensors->getSpeedController()->resetDirections();
        return ResponseCodes::GOOD_REQUEST_NO_REPLY;
    }
    else 
    {
        return ResponseCodes::BAD_REQUEST;
    }
}

int Communicator::getShoeSide()
{
    int shoeSide = VrShoePreferences.getInt(ShoeSides::SHOE_SIDE_KEY, ShoeSides::UNSPECIFIED);
    json.clear();
    json[MessageKeys::COMMAND] = Messages::SHOE_SIDE;
    json[MessageKeys::REPLY] = true;
    json[MessageKeys::SIDE] = shoeSide;
    json[MessageKeys::SHOE_ID] = shoeId;
    return ResponseCodes::GOOD_REQUEST_SEND_REPLY;
}

int Communicator::startAlgorithm()
{
    shoeController->start();
    return ResponseCodes::GOOD_REQUEST_NO_REPLY;
}

int Communicator::stopAlgorithm()
{
    shoeController->stop();
    return ResponseCodes::GOOD_REQUEST_NO_REPLY;
}