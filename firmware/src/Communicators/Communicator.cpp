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

    //Serial.print("Shoe ID - ");
    //Serial.println(shoeId);
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

String Communicator::cleanMessage(String message)
{
    int jsonStart = -1;
    int jsonEnd = -1;
    for(int i = 0; i < message.length(); i++)
    {
        if(message[i] == '{')
        {
            jsonStart = i;
            break;
        }
    }
    for(int i = message.length() - 1; i >= jsonStart; i--)
    {
        if(message[i] == '}')
        {
            jsonEnd = i;
            break;
        }
    }
    if(jsonStart == -1 || jsonEnd == -1)
    {
        return "";
    }
    return message.substring(jsonStart, jsonEnd + 1);
}

int Communicator::handleRecievedMessage(String message)
{
    message = cleanMessage(message);
    json.clear();
    DeserializationError error = deserializeJson(json, message);
    if (error) 
    {
        json.clear();
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
        return isGetRequest() ? replyWithSensorData() : recieveSensorData();        
    }
    else if(commandId.equals(Messages::RESET_DISTANCE_TRACKER))
    {
        return resetDistanceTracker();
    }
    else if(commandId.equals(Messages::SET_RPM))
    {
        return setRpm();
    }
    else if(commandId.equals(Messages::SET_COMMUNICATION_MODE))
    {
        return setCommunicationMode();
    }
    else if(commandId.equals(Messages::SHOE_CONFIGURATIONS))
    {
        return isGetRequest() ? getShoeConfigurations() : configureShoe();
    }
    else if(commandId.equals(Messages::START_NEGATING_MOTION))
    {
        return startNegatingMotion();
    }
    else if(commandId.equals(Messages::STOP_NEGATING_MOTION))
    {
        return stopNegatingMotion();
    }
    else if(commandId.equals(Messages::POWER_STATISTICS))
    {
        return getPowerStatistics();
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
    }
    return ResponseCodes::UNKNOWN_REQUEST;
}

bool Communicator::isGetRequest()
{
    return !json[MessageKeys::GET].isNull() && json[MessageKeys::GET].as<bool>();
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
    sendSensorData();
    json[MessageKeys::REPLY] = true;
    return ResponseCodes::GOOD_REQUEST_SEND_REPLY;
}

void Communicator::sendSensorDataIfStale(String sendingToShoeId)
{
    sendSensorData();
    json[MessageKeys::DESTINATION] = sendingToShoeId;
    String message;
    serializeJson(json, message);
    if(!message.equals(lastSensorDataMessageSent))
    {
        sendMessageTo(sendingToShoeId);
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
    RemoteVrShoe* remoteShoe = sensors->getRemoteVrShoe();
    remoteShoe->frontButtonPressed = json[MessageKeys::FRONT_BUTTON_PRESSED];
    remoteShoe->rearButtonPressed = json[MessageKeys::REAR_BUTTON_PRESSED];
    remoteShoe->forwardSpeed = json[MessageKeys::FORWARD_SPEED];
    remoteShoe->sidewaySpeed = json[MessageKeys::SIDEWAY_SPEED];
    remoteShoe->forwardDistance = json[MessageKeys::FORWARD_DISTANCE];
    remoteShoe->sidewaysDistance = json[MessageKeys::SIDEWAY_DISTANCE];
    return ResponseCodes::GOOD_REQUEST_NO_REPLY;
}

void Communicator::sendSensorData()
{
    json.clear();
    json[MessageKeys::COMMAND] = Messages::SENSOR_DATA;
    json[MessageKeys::FRONT_BUTTON_PRESSED] = sensors->isFrontButtonPressed();
    json[MessageKeys::REAR_BUTTON_PRESSED] = sensors->isRearButtonPressed();
    json[MessageKeys::SHOE_ID] = shoeId;

    MovementTracker* movementTracker = sensors->getMovementTracker();

    Vector2D speed = movementTracker->getSpeed();
    json[MessageKeys::FORWARD_SPEED] = roundFloatToTwoDecimalPlaces(speed.getX());
    json[MessageKeys::SIDEWAY_SPEED] = roundFloatToTwoDecimalPlaces(speed.getY());

    Vector2D distance = movementTracker->getDistanceTracked();
    json[MessageKeys::FORWARD_DISTANCE] = roundFloatToTwoDecimalPlaces(distance.getX());
    json[MessageKeys::SIDEWAY_DISTANCE] = roundFloatToTwoDecimalPlaces(distance.getY());

    json[MessageKeys::MOVEMENT_STATE] = shoeController->getMovementState();
}

float Communicator::roundFloatToTwoDecimalPlaces(float number)
{
    return (float)(roundf(number * 100) / 100);
}

int Communicator::resetDistanceTracker()
{
    sensors->getMovementTracker()->resetDistance();

    json.clear();
    json[MessageKeys::COMMAND] = Messages::RESET_DISTANCE_TRACKER;
    json[MessageKeys::REPLY] = true;
    json[MessageKeys::SHOE_ID] = shoeId;

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

int Communicator::getShoeConfigurations()
{
    json.clear();
    json[MessageKeys::COMMAND] = Messages::SHOE_CONFIGURATIONS;
    json[MessageKeys::REPLY] = true;
    json[MessageKeys::SHOE_ID] = shoeId;
    json[MessageKeys::OTHER_SHOE_ID] = sensors->getRemoteVrShoe()->shoeId;
    json[MessageKeys::SIDE] = VrShoePreferences.getInt(ShoeSides::SHOE_SIDE_KEY, ShoeSides::UNSPECIFIED);
    json[MessageKeys::DUTY_CYCLE_BOOST] = roundFloatToTwoDecimalPlaces(sensors->getSpeedController()->getDutyCycleBoost());
    json[MessageKeys::SPEED_MULTIPLER] = roundFloatToTwoDecimalPlaces(shoeController->getSpeedMultiplier());

    return ResponseCodes::GOOD_REQUEST_SEND_REPLY;
}

int Communicator::configureShoe()
{
    String otherShoeId = json[MessageKeys::OTHER_SHOE_ID];
    float boost = json[MessageKeys::DUTY_CYCLE_BOOST];
    int shoeSide = json[MessageKeys::SIDE];
    float speedMultiplier = json[MessageKeys::SPEED_MULTIPLER];

    if(setShoeSide(shoeSide) == ResponseCodes::BAD_REQUEST)
    {
        return ResponseCodes::BAD_REQUEST;
    }

    sensors->getRemoteVrShoe()->shoeId = otherShoeId;
    sensors->getSpeedController()->setDutyCycleBoost(boost);
    shoeController->setSpeedMultiplier(speedMultiplier);
    
    return ResponseCodes::GOOD_REQUEST_NO_REPLY;
}

int Communicator::setShoeSide(int shoeSide)
{
    if(shoeSide == ShoeSides::LEFT)
    {
        VrShoePreferences.putInt(ShoeSides::SHOE_SIDE_KEY, shoeSide);
        sensors->getSpeedController()->inverseForwardDirection();
        return ResponseCodes::GOOD_REQUEST_NO_REPLY;
    }
    else if(shoeSide == ShoeSides::RIGHT)
    {
        VrShoePreferences.putInt(ShoeSides::SHOE_SIDE_KEY, shoeSide);
        sensors->getSpeedController()->inverseSidewayDirection();
        return ResponseCodes::GOOD_REQUEST_NO_REPLY;
    }
    else 
    {
        return ResponseCodes::BAD_REQUEST;
    }
}

int Communicator::startNegatingMotion()
{
    shoeController->start();
    return ResponseCodes::GOOD_REQUEST_NO_REPLY;
}

int Communicator::stopNegatingMotion()
{
    shoeController->stop();
    return ResponseCodes::GOOD_REQUEST_NO_REPLY;
}

int Communicator::getPowerStatistics()
{
    json[MessageKeys::COMMAND] = Messages::POWER_STATISTICS;
    json[MessageKeys::SHOE_ID] = shoeId;
    json[MessageKeys::REPLY] = true;

    PowerStatistics sidewayStatistics = sensors->getPowerTracker()->getSidewayCurrentStatistics();
    json[MessageKeys::SIDEWAY_PEAK_CURRENT] = roundFloatToTwoDecimalPlaces(sidewayStatistics.peakCurrent);
    json[MessageKeys::SIDEWAY_AVERAGE_CURRENT] = roundFloatToTwoDecimalPlaces(sidewayStatistics.averageCurrent);
    json[MessageKeys::SIDEWAY_CURRENT_NOW] = roundFloatToTwoDecimalPlaces(sidewayStatistics.currentNow);
    json[MessageKeys::SIDEWAY_AMP_HOURS] = roundFloatToTwoDecimalPlaces(sidewayStatistics.ampHours);
    json[MessageKeys::SIDEWAY_AMP_HOURS_CHARGED] = roundFloatToTwoDecimalPlaces(sidewayStatistics.ampHoursCharged);

    PowerStatistics forwardStatistics = sensors->getPowerTracker()->getForwardCurrentStatistics();
    json[MessageKeys::FORWARD_PEAK_CURRENT] = roundFloatToTwoDecimalPlaces(forwardStatistics.peakCurrent);
    json[MessageKeys::FORWARD_AVERAGE_CURRENT] = roundFloatToTwoDecimalPlaces(forwardStatistics.averageCurrent);
    json[MessageKeys::FORWARD_CURRENT_NOW] = roundFloatToTwoDecimalPlaces(forwardStatistics.currentNow);
    json[MessageKeys::FORWARD_AMP_HOURS] = roundFloatToTwoDecimalPlaces(forwardStatistics.ampHours);
    json[MessageKeys::FORWARD_AMP_HOURS_CHARGED] = roundFloatToTwoDecimalPlaces(forwardStatistics.ampHoursCharged);

    return ResponseCodes::GOOD_REQUEST_SEND_REPLY;
}