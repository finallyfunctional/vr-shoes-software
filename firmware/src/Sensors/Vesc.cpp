#include "../../include/Sensors/Vesc.h"

Vesc::Vesc(Stream* serialForVesc, Motor motor, Wheel wheel) : safetyTimer(Timer(1000)), performanceTimer(Timer(0))
{
    vescUart.setSerialPort(serialForVesc);

    gearingRatio = motor.GEARING_RATIO;
    tachometerCountsPerRovolution = 3 * motor.POLES;
    polePairs = motor.POLES / 2;
    distanceTraveledPerRevolution = wheel.DIAMETER * PI;
}

void Vesc::update()
{
    performanceTimer.start();
    if(!vescUart.getVescValues())
    {
        Serial.println("Could not connect to VESC!");
        return;
    }
    Serial.print("Reading values took ");
    Serial.println(performanceTimer.getElapsedTime());
    if(safetyTimer.timeIsUp() || desiredRpm == 0)
    {
        Serial.println("Setting current to 0");
        performanceTimer.start();
        vescUart.setCurrent(0);
        Serial.print("Setting current took ");
        Serial.println(performanceTimer.getElapsedTime());
    }
    else if(vescUart.data.rpm == desiredRpm)
    {
        Serial.println("Current RPM matches desired RPM");
        vescUart.setDuty(vescUart.data.dutyCycleNow);
    }
    else
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
    
    
}

void Vesc::resetOrigin()
{
    originTachometer = vescUart.data.tachometer;
}

void Vesc::setRpm(float rpm)
{
    desiredRpm = rpm;
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

float Vesc::getDistanceFromOrigin()
{
    long tachometerCounts = labs(vescUart.data.tachometer - originTachometer);
    float revolutions = (float) (tachometerCounts) / tachometerCountsPerRovolution;
    return (distanceTraveledPerRevolution * revolutions) / gearingRatio;
}