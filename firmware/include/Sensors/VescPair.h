#ifndef VescPair_h
#define VescPair_h

#include <Arduino.h>
#include "../Vector2D.h"
#include "./Sensors/MovementTracker.h"
#include "./Sensors/SpeedController.h"
#include "./Sensors/PowerTracker.h"
#include "./Vesc.h"
#include "../Timer.h"
#include "./PowerStatistics.h"

class VescPair : public MovementTracker, public SpeedController, public PowerTracker
{
    public:
    VescPair(Vesc* forwardVesc, Vesc* sidewaysVesc);
    void update();
    Vector2D getSpeed();
    Vector2D getDistanceFromOrigin();
    Vector2D getDesiredSpeed();
    Vector2D getCurrentDutyCycle();
    void resetOrigin();
    void setRpm(float forwardRpm, float sidewayRpm);
    void setForwardRpm(float rpm);
    void setSidewayRpm(float rpm);
    void setForwardSpeed(float speed);
    void setSidewaySpeed(float speed);
    void inverseForwardDirection();
    void inverseSidewayDirection();
    void resetDirections();
    void brakeForwardsBackwards();
    void brakeSideway();
    void setDutyCycleBoost(float boost);
    void tunePidLoop(float kp, float ki, float kd);
    PowerStatistics getForwardCurrentStatistics();
    PowerStatistics getSidewayCurrentStatistics();

    private:
    Vesc* forwardVesc;
    Vesc* sidewaysVesc;
    Timer* updateTimer;
};

#endif