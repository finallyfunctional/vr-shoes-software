#include "../../include/Sensors/VescPair.h"

VescPair::VescPair(Stream* serialForForwardVesc, Stream* serialForSidewaysVesc, Motor motor, Wheel wheel)
{
    forwardVesc.setSerialPort(serialForForwardVesc);
    sidewaysVesc.setSerialPort(serialForSidewaysVesc);
    
    tachometerCountsPerRovolution = 3 * motor.POLES;
    polePairs = motor.POLES / 2;
    distanceTraveledPerRevolution = wheel.DIAMETER * PI;
}

void VescPair::update()
{
    if(!forwardVesc.getVescValues())
    {
        Serial.println("Forward VESC values not read!");
    }
    else
    {
        Serial.print("Forward Speed: ");
        Serial.println(forwardVesc.data.rpm);
        Serial.print("Forward Tachometer: ");
        Serial.println(forwardVesc.data.tachometer);
    }
    if(!sidewaysVesc.getVescValues())
    {
        Serial.println("Sideway VESC values not read!");
    }
    else
    {
        Serial.print("Sideway Speed: ");
        Serial.println(sidewaysVesc.data.rpm);
        Serial.print("Sideway Tachometer: ");
        Serial.println(sidewaysVesc.data.tachometer);
    }
    
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
    return distanceTraveledPerRevolution * rps;
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

float VescPair::getLinearDistanceFromOrigin(float currentTachometerCounts, float tachometerCountOrigin)
{
    float tachometerCounts = fabs(currentTachometerCounts - tachometerCountOrigin);
    float revolutions = tachometerCounts / tachometerCountsPerRovolution;
    return distanceTraveledPerRevolution * revolutions;
}

float VescPair::getOrientation()
{
    return orientation;
}