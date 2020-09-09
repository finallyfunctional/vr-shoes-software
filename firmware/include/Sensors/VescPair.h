#ifndef Vesc_h
#define Vesc_h

#include <Arduino.h>
#include <VescUart.h>
#include "../Vector2D.h"
#include "./Sensors/MovementTracker.h"
#include "../Motors/Motor.h"
#include "../Wheels/Wheel.h"

class VescPair : public MovementTracker
{
    public:
    VescPair(Stream* serialForForwardVesc, Stream* serialForSidewaysVesc, Motor motor, Wheel wheel);
    void update();
    Vector2D getSpeed();
    Vector2D getDistanceFromOrigin();
    void resetOrigin();
    float getOrientation();

    private:
    float getSpeed(float erpm);
    float convertErpmToMrpm(float erpm);
    float getLinearDistanceFromOrigin(float currentTachometerCounts, float tachometerCountOrigin);

    VescUart forwardVesc;
    VescUart sidewaysVesc;
    float orientation;
    float originTachometerX;
    float originTachometerY;
    float polePairs;
    float tachometerCountsPerRovolution;
    float distanceTraveledPerRevolution;
};

#endif