#include "../../include/Sensors/Vesc.h"

Vesc::Vesc(Stream* serialForVesc, Motor motor, Wheel wheel) : safetyTimer(Timer(1000))
{
    vescUart.setSerialPort(serialForVesc);

    directionInverter = 1;
    gearingRatio = motor.GEARING_RATIO;
    tachometerCountsPerRovolution = 3 * motor.POLES;
    polePairs = motor.POLES / 2;
    distanceTraveledPerRevolution = wheel.DIAMETER * PI;
    triggerBrakeAtErpm = convertMrpmToErpm(TRIGGER_BRAKE_AT_MRPM) * gearingRatio;
    maxErpm = convertMrpmToErpm(motor.MAX_RPM);
    pidLoop = Pid();
    pidLoop.tune(0, 0, 0);
}

void Vesc::update()
{
    if(!vescUart.getVescValues())
    {
        Serial.println("Could not connect to VESC!");
    }
    else if(safetyTimer.timeIsUp() || mode == BRAKING_MODE)
    {
        handleBraking();
    }
    else if(desiredRpm == 0 && mode == MOVING_MODE)
    {
        vescUart.setCurrent(0);
    }
    else
    {
        updateSpeedUsingSimpleStepping();
    }
}

void Vesc::updateSpeedUsingPidLoop()
{
    float nextRpm = pidLoop.compute(vescUart.data.rpm, desiredRpm);
    float newDuty = convertErpmToDutyCycle(nextRpm);
    if(newDuty > 1)
    {
        newDuty = 1;
    }
    else if(newDuty < -1)
    {
       newDuty = -1;
    }
    Serial.print("setting duty to ");
    Serial.println(newDuty);
    vescUart.setDuty(newDuty);
}

void Vesc::updateSpeedUsingSimpleStepping()
{
    float newDuty = 0;
    int difference = fabs((vescUart.data.rpm - desiredRpm) / desiredRpm) * 100;
    double moveStep;
    if(difference > 50)
    {
        moveStep = SIMPLE_LARGE_MOVE_STEP;
    }
    else if(difference > 5)
    {
        moveStep = SIMPLE_MEDIUM_MOVE_STEP;
    }
    else 
    {
        moveStep = SIMPLE_SMALL_MOVE_STEP;
    }
    
    if(vescUart.data.rpm < desiredRpm)
    {
        newDuty = vescUart.data.dutyCycleNow + moveStep;
        if(newDuty > 1)
        {
            newDuty = 1;
        }
    }
    else
    {
        newDuty = vescUart.data.dutyCycleNow - moveStep;
        if(newDuty < -1)
        {
            newDuty = -1;
        }
    }
    vescUart.setDuty(newDuty);
}

void Vesc::handleBraking()
{
    double brakeCurrent = previousBrakeCurrent;
    if(fabs(vescUart.data.rpm) < triggerBrakeAtErpm)
    {
        if(previousBrakeCurrent != 0)
        {
            vescUart.setCurrent(0);
            return;
        }
        else if(previousBrakeCurrent > 0)
        {
            brakeCurrent -= DECREMENT_BRAKE_STEP;
        }
    }
    else 
    {
        brakeCurrent += INCREMENT_BRAKE_STEP;
        if(brakeCurrent > MAX_BRAKE_CURRENT)
        {
            brakeCurrent = MAX_BRAKE_CURRENT;
        }
    }
    vescUart.setHandBrakeCurrent(brakeCurrent);
    previousBrakeCurrent = brakeCurrent;
}

void Vesc::resetOrigin()
{
    originTachometer = vescUart.data.tachometer;
}

void Vesc::setRpm(float rpm)
{
    desiredRpm = convertMrpmToErpm(rpm) * gearingRatio * directionInverter;
    mode = MOVING_MODE;
    previousBrakeCurrent = 0;
    safetyTimer.start();
}

void Vesc::setSpeed(float speed)
{
    desiredRpm = convertMrpmToErpm((speed / distanceTraveledPerRevolution) * 60 * gearingRatio) * directionInverter;
    mode = MOVING_MODE;
    previousBrakeCurrent = 0;
    safetyTimer.start();
}

float Vesc::getSpeed()
{
    float rps = convertErpmToMrpm(vescUart.data.rpm) / 60;
    return distanceTraveledPerRevolution * rps * directionInverter / gearingRatio;
}

float Vesc::convertErpmToMrpm(float erpm)
{
    return erpm / polePairs;
}

float Vesc::convertMrpmToErpm(float mrpm)
{
    return mrpm * polePairs;
}

float Vesc::convertErpmToDutyCycle(float erpm)
{
    return erpm / maxErpm;
}

float Vesc::getDistanceFromOrigin()
{
    long tachometerCounts = labs(vescUart.data.tachometer - originTachometer);
    float revolutions = (float) (tachometerCounts) / tachometerCountsPerRovolution;
    return (distanceTraveledPerRevolution * revolutions * directionInverter) / gearingRatio;
}

void Vesc::inverseDirection()
{
    directionInverter = -1;
}

void Vesc::resetDirection()
{
    directionInverter = 1;
}

void Vesc::tunePidLoop(float kp, float ki, float kd)
{
    pidLoop.tune(kp, ki, kd);
}

void Vesc::brake()
{
    mode = BRAKING_MODE;
}