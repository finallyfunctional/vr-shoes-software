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
    void setSpeed(float speed);
    void inverseDirection();
    void resetDirection();
    void tunePidLoop(float kp, float ki, float kd);
    void brake();

    private:
    float convertErpmToMrpm(float erpm);
    float convertMrpmToErpm(float mrpm);
    float convertErpmToDutyCycle(float erpm);
    void handleBraking();

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
    int mode;
    int triggerBrakeAtErpm;
    double previousBrakeCurrent = 0;

    const int MOVING_MODE = 1;
    const int BRAKING_MODE = 2;

    const double SIMPLE_LARGE_MOVE_STEP = 0.02;
    const double SIMPLE_MEDIUM_MOVE_STEP = 0.01;
    const double SIMPLE_SMALL_MOVE_STEP = 0.001;

    const int TRIGGER_BRAKE_AT_MRPM = 10;
    const int MAX_BRAKE_CURRENT = 12;
    const int INCREMENT_BRAKE_STEP = 5;
    const double DECREMENT_BRAKE_STEP = 0.2;
};

#endif