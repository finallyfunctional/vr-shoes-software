#include "../../include/Sensors/VescPair.h"

VescPair::VescPair(Vesc* forwardVesc, Vesc* sidewaysVesc)
{
    this->forwardVesc = forwardVesc;
    this->sidewaysVesc = sidewaysVesc;
    this->updateTimer = new Timer(50);
    updateTimer->start();
}

void VescPair::update()
{
    if(!updateTimer->timeIsUp())
    {
        return;
    }
    updateTimer->start();
    forwardVesc->update();
    //sidewaysVesc->update();
}

void VescPair::resetOrigin()
{
    forwardVesc->resetOrigin();
    sidewaysVesc->resetOrigin();
}

Vector2D VescPair::getSpeed()
{
    return Vector2D(forwardVesc->getSpeed(), sidewaysVesc->getSpeed());
}

Vector2D VescPair::getDistanceFromOrigin()
{
    return Vector2D(
        forwardVesc->getDistanceFromOrigin(),
        sidewaysVesc->getDistanceFromOrigin()
    );
}

void VescPair::setRpm(float forwardRpm, float sidewayRpm)
{
    forwardVesc->setRpm(forwardRpm);
    sidewaysVesc->setRpm(sidewayRpm);
}

void VescPair::setForwardRpm(float rpm)
{
    forwardVesc->setRpm(rpm);
}

void VescPair::setSidewayRpm(float rpm)
{
    sidewaysVesc->setRpm(rpm);
}

void VescPair::setForwardSpeed(float speed)
{
    forwardVesc->setSpeed(speed);
}

void VescPair::setSidewaySpeed(float speed)
{
    sidewaysVesc->setSpeed(speed);
}

void VescPair::inverseForwardDirection()
{
    forwardVesc->inverseDirection();
}

void VescPair::inverseSidewayDirection()
{
    sidewaysVesc->inverseDirection();
}

void VescPair::resetDirections()
{
    forwardVesc->resetDirection();
    sidewaysVesc->resetDirection();
}

void VescPair::brakeForwardsBackwards()
{
    forwardVesc->brake();
}

void VescPair::brakeSideway()
{
    //sidewaysVesc->brake();
}

void VescPair::setDutyCycleBoost(float boost)
{
    forwardVesc->setDutyCycleBoost(boost);
    sidewaysVesc->setDutyCycleBoost(boost);
}

Vector2D VescPair::getDesiredSpeed()
{
    return Vector2D(forwardVesc->getDesiredSpeed(), sidewaysVesc->getDesiredSpeed());
}

Vector2D VescPair::getCurrentDutyCycle()
{
    return Vector2D(forwardVesc->getCurrentDutyCycle(), sidewaysVesc->getCurrentDutyCycle());
}

void VescPair::tunePidLoop(float kp, float ki, float kd)
{
    forwardVesc->tunePidLoop(kp, ki, kd);
    sidewaysVesc->tunePidLoop(kp, ki, kd);
}

PowerStatistics VescPair::getForwardCurrentStatistics()
{
    return forwardVesc->getCurrentStatistics();
}
    
PowerStatistics VescPair::getSidewayCurrentStatistics()
{
    return sidewaysVesc->getCurrentStatistics();
}