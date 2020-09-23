#ifndef Vesc_h
#define Vesc_h

#include <Arduino.h>
#include <VescUart.h>
#include "../Motors/Motor.h"
#include "../Wheels/Wheel.h"
#include "../Timer.h"
#include "../Pid.h"

class Vesc
{
    public:
    Vesc(Stream* serialForVesc, Motor motor, Wheel wheel);
    void update();
    float getSpeed();
    float getDistanceFromOrigin();
    void resetOrigin();
    void setRpm(float rpm);
    void inverseDirection();
    void resetDirection();

    private:
    float convertErpmToMrpm(float erpm);
    float convertMrpmToErpm(float mrpm);
    float convertErpmToDutyCycle(float erpm);

    void updateSpeedUsingPidLoop();
    void updateSpeedUsingSimpleStepping();

    Pid pidLoop;
    VescUart vescUart;
    float originTachometer;
    float polePairs;
    float tachometerCountsPerRovolution;
    float distanceTraveledPerRevolution;
    float gearingRatio; 
    float desiredRpm;
    float maxErpm;
    Timer safetyTimer;
    int directionInverter;

    const double simpleLargeMoveStep = 0.02;
    const double simpleMediumMoveStep = 0.01;
    const double simpleSmallMoveStep = 0.001;
};

#endif