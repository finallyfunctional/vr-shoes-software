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
    else if(commandId.equals(Messages::EXTRA_SENSOR_DATA))
    {
        return sendExtraSensorData();
    }
    else if(commandId.equals(Messages::DUTY_CYCLE_BOOST))
    {
        return setDutyCycleBoost();
    }
    else if(commandId.equals(Messages::TUNE_PID_LOOP))
    {
        return tunePidLoop();
    }
    else if(commandId.equals(Messages::SPEED_MULTIPLIER))
    {
        if(!json[MessageKeys::GET].isNull() && json[MessageKeys::GET].as<bool>())
        {
            return getSpeedMultipler();
        }
        else 
        {
            return setSpeedMultipler();
        }
    }
    else if(commandId.equals(Messages::POWER_STATISTICS))
    {
        return getPowerStatistics();
    }
    else if(commandId.equals(Messages::CONFIGURE_BUTTONS))
    {
        return configureButtons();
    }
    else if(commandId.equals(Messages::BUTTON_VALUES))
    {
        return sendButtonValues();
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
    remoteShoe->forwardDistanceFromOrigin = json[MessageKeys::FORWARD_DISTANCE];
    remoteShoe->sidewayDistanceFromOrigin = json[MessageKeys::SIDEWAY_DISTANCE];
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

    Vector2D distance = movementTracker->getDistanceFromOrigin();
    json[MessageKeys::FORWARD_DISTANCE] = roundFloatToTwoDecimalPlaces(distance.getX());
    json[MessageKeys::SIDEWAY_DISTANCE] = roundFloatToTwoDecimalPlaces(distance.getY());
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

int Communicator::sendExtraSensorData()
{
    json.clear();
    json[MessageKeys::COMMAND] = Messages::EXTRA_SENSOR_DATA;
    json[MessageKeys::SHOE_ID] = shoeId;
    json[MessageKeys::REPLY] = true;

    Vector2D desiredSpeed = sensors->getMovementTracker()->getDesiredSpeed();
    json[MessageKeys::FORWARD_DESIRED_SPEED] = desiredSpeed.getX();
    json[MessageKeys::SIDEWAY_DESIRED_SPEED] = desiredSpeed.getY();

    Vector2D currentDutyCycle = sensors->getSpeedController()->getCurrentDutyCycle();
    json[MessageKeys::FORWARD_DUTY_CYCLE] = currentDutyCycle.getX();
    json[MessageKeys::SIDEWAY_DUTY_CYCLE] = currentDutyCycle.getY();

    return ResponseCodes::GOOD_REQUEST_SEND_REPLY;
}

int Communicator::setDutyCycleBoost()
{
    float boost = json[MessageKeys::DUTY_CYCLE_BOOST];
    sensors->getSpeedController()->setDutyCycleBoost(boost);
    return ResponseCodes::GOOD_REQUEST_NO_REPLY;
}

int Communicator::tunePidLoop()
{
    float kp = json[MessageKeys::KP];
    float ki = json[MessageKeys::KI];
    float kd = json[MessageKeys::KD];
    sensors->getSpeedController()->tunePidLoop(kp, ki, kd);
    return ResponseCodes::GOOD_REQUEST_NO_REPLY;
}

int Communicator::getSpeedMultipler()
{
    json[MessageKeys::COMMAND] = Messages::SPEED_MULTIPLIER;
    json[MessageKeys::SHOE_ID] = shoeId;
    json[MessageKeys::REPLY] = true;
    json[MessageKeys::SPEED_MULTIPLER] = shoeController->getSpeedMultiplier();
    return ResponseCodes::GOOD_REQUEST_SEND_REPLY;
}

int Communicator::setSpeedMultipler()
{
    float multiplier = json[MessageKeys::SPEED_MULTIPLER];
    shoeController->setSpeedMultiplier(multiplier);
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

int Communicator::configureButtons()
{
    int maxDiff = json[MessageKeys::BUTTON_MAX_DIFFERENCE];
    Button* frontBtn = sensors->getFrontButton();
    Button* rearBtn = sensors->getRearButton();

    frontBtn->setPressedValue();
    frontBtn->setMaxDifferencePercentage(maxDiff);
    rearBtn->setPressedValue();
    rearBtn->setMaxDifferencePercentage(maxDiff);

    VrShoePreferences.putInt(Button::FRONT_BUTTON_PRESSED_VALUE_KEY, frontBtn->getPressedValue());
    VrShoePreferences.putInt(Button::REAR_BUTTON_PRESSED_VALUE_KEY, rearBtn->getPressedValue());
    VrShoePreferences.putInt(Button::MAX_DIFF_VALUE_KEY, maxDiff);

    return ResponseCodes::GOOD_REQUEST_NO_REPLY;
}

int Communicator::sendButtonValues()
{
    Button* frontBtn = sensors->getFrontButton();
    Button* rearBtn = sensors->getRearButton();

    json[MessageKeys::COMMAND] = Messages::BUTTON_VALUES;
    json[MessageKeys::SHOE_ID] = shoeId;
    json[MessageKeys::REPLY] = true;
    json[MessageKeys::FRONT_BUTTON_PRESSED_VALUE] = frontBtn->getCurrentValue();
    json[MessageKeys::REAR_BUTTON_PRESSED_VALUE] = rearBtn->getCurrentValue();
    json[MessageKeys::BUTTON_MAX_DIFFERENCE] = frontBtn->getMaxDifferencePercentage();

    return ResponseCodes::GOOD_REQUEST_SEND_REPLY;
}