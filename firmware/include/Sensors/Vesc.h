#ifndef Vesc_h
#define Vesc_h

#include <Arduino.h>
#include <VescUart.h>
#include "../Motors/Motor.h"
#include "../Wheels/Wheel.h"
#include "../Timer.h"

class Vesc
{
    public:
    Vesc(Stream* serialForVesc, Motor motor, Wheel wheel);
    void update();
    float getSpeed();
    float getDistanceFromOrigin();
    void resetOrigin();

    private:
    float convertErpmToMrpm(float erpm);

    VescUart vescUart;
    float originTachometer;
    float polePairs;
    float tachometerCountsPerRovolution;
    float distanceTraveledPerRevolution;
    float gearingRatio; 
};

#endif