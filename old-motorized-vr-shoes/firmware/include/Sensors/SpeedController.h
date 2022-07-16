#ifndef SpeedController_h
#define SpeedController_h

#include <Arduino.h>
#include <Preferences.h>
#include "../Vector2D.h"
#include "./VrShoeGlobals.h"

class SpeedController
{
    public:
    SpeedController();
    virtual void setRpm(float forwardRpm, float sidewayRpm) = 0;
    virtual void setForwardRpm(float rpm) = 0;
    virtual void setSidewayRpm(float rpm) = 0;
    virtual void setForwardSpeed(float speed) = 0;
    virtual void setSidewaySpeed(float speed) = 0;
    virtual void inverseForwardDirection() = 0;
    virtual void inverseSidewayDirection() = 0;
    virtual void resetDirections() = 0;
    virtual void brakeForwardsBackwards() = 0;
    virtual void brakeSideway() = 0;
    virtual float getDutyCycleBoost() = 0;
    virtual Vector2D getCurrentDutyCycle() = 0;
    virtual void tunePidLoop(float kp, float ki, float kd) = 0;
    virtual void setDutyCycleBoost(float boost);

    static const char* DUTY_CYCLE_BOOST;

    protected:
    float boost;
};

#endif