#include "../../include/Sensors/Vesc.h"

Vesc::Vesc(Stream* serialForVesc, Motor motor, Wheel wheel)
{
    vescUart.setSerialPort(serialForVesc);

    gearingRatio = motor.GEARING_RATIO;
    tachometerCountsPerRovolution = 3 * motor.POLES;
    polePairs = motor.POLES / 2;
    distanceTraveledPerRevolution = wheel.DIAMETER * PI;
}

void Vesc::update()
{
    vescUart.getVescValues();
}

void Vesc::resetOrigin()
{
    originTachometer = vescUart.data.tachometer;
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

float Vesc::getDistanceFromOrigin()
{
    long tachometerCounts = labs(vescUart.data.tachometer - originTachometer);
    float revolutions = (float) (tachometerCounts) / tachometerCountsPerRovolution;
    return (distanceTraveledPerRevolution * revolutions) / gearingRatio;
}