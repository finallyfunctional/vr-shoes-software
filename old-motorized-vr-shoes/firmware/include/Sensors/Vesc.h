#ifndef Vesc_h
#define Vesc_h

#include <Arduino.h>
#include <VescUart.h>
#include "../Motors/Motor.h"
#include "../Wheels/Wheel.h"
#include "../Timer.h"
#include "../Pid.h"
#include "./PowerStatistics.h"

class Vesc
{
    public:
    Vesc(Stream* serialForVesc, Motor motor, Wheel wheel);
    void update();
    float getSpeed();
    float getDistanceFromOrigin();
    float getCurrentDutyCycle();
    float getDesiredSpeed();
    void resetDistance();
    void setRpm(float rpm);
    void setSpeed(float speed);
    void inverseDirection();
    void resetDirection();
    void tunePidLoop(float kp, float ki, float kd);
    void brake();
    void setDutyCycleBoost(float boost);
    PowerStatistics getCurrentStatistics();

    private:
    float convertErpmToMrpm(float erpm);
    float convertMrpmToErpm(float mrpm);
    float convertErpmToDutyCycle(float erpm);
    float getSpeedFromErpm(float erpm);
    void handleBraking();
    void updateStatistics();

    void updateSpeedUsingPidLoop();
    void updateSpeedUsingSimpleStepping();
    void updateSpeedUsingBoost();

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
    float dutyCycleBoost;
    PowerStatistics powerStats;

    float peakCurrent;
    float currentSum;
    float currentReadCount;

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