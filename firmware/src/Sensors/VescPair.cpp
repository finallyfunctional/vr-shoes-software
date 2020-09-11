#include "../../include/Sensors/VescPair.h"

VescPair::VescPair(Stream* serialForForwardVesc, Stream* serialForSidewaysVesc, Motor motor, Wheel wheel)
{
    forwardVesc.setSerialPort(serialForForwardVesc);
    sidewaysVesc.setSerialPort(serialForSidewaysVesc);
    
    gearingRatio = motor.GEARING_RATIO;
    tachometerCountsPerRovolution = 3 * motor.POLES;
    polePairs = motor.POLES / 2;
    distanceTraveledPerRevolution = wheel.DIAMETER * PI;
}

void VescPair::update()
{
    forwardVesc.getVescValues();
    sidewaysVesc.getVescValues();
} 

void VescPair::resetOrigin()
{
    orientation = 0;
    originTachometerX = forwardVesc.data.tachometer;
    originTachometerY = sidewaysVesc.data.tachometer;
}

Vector2D VescPair::getSpeed()
{
    return Vector2D(getSpeed(forwardVesc.data.rpm), getSpeed(sidewaysVesc.data.rpm));
}

float VescPair::getSpeed(float erpm)
{
    float rps = convertErpmToMrpm(erpm) / 60;
    return distanceTraveledPerRevolution * rps / gearingRatio;
}

float VescPair::convertErpmToMrpm(float erpm)
{
    return erpm / polePairs;
}

Vector2D VescPair::getDistanceFromOrigin()
{
    return Vector2D(
        getLinearDistanceFromOrigin(forwardVesc.data.tachometer, originTachometerX), 
        getLinearDistanceFromOrigin(sidewaysVesc.data.tachometer, originTachometerY)
    );
}

float VescPair::getLinearDistanceFromOrigin(long currentTachometerCounts, long tachometerCountOrigin)
{
    long tachometerCounts = labs(currentTachometerCounts - tachometerCountOrigin);
    float revolutions = (float) (tachometerCounts) / tachometerCountsPerRovolution;
    return (distanceTraveledPerRevolution * revolutions) / gearingRatio;
}

float VescPair::getOrientation()
{
    return orientation;
}