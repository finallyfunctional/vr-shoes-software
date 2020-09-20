#include "../../include/Sensors/Vesc.h"

Vesc::Vesc(Stream* serialForVesc, Motor motor, Wheel wheel) : safetyTimer(Timer(1000))
{
    vescUart.setSerialPort(serialForVesc);

    gearingRatio = motor.GEARING_RATIO;
    tachometerCountsPerRovolution = 3 * motor.POLES;
    polePairs = motor.POLES / 2;
    distanceTraveledPerRevolution = wheel.DIAMETER * PI;
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
    else if(safetyTimer.timeIsUp() || desiredRpm == 0)
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
    Serial.print("setting duty to ");
    Serial.println(newDuty);
    vescUart.setDuty(newDuty);
}

void Vesc::resetOrigin()
{
    originTachometer = vescUart.data.tachometer;
}

void Vesc::setRpm(float rpm)
{
    desiredRpm = convertMrpmToErpm(rpm) * gearingRatio;
    safetyTimer.start();
}

float Vesc::getSpeed()
{
    float rps = convertErpmToMrpm(vescUart.data.rpm) / 60;
    return distanceTraveledPerRevolution * rps / gearingRatio;
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
    return (distanceTraveledPerRevolution * revolutions) / gearingRatio;
}