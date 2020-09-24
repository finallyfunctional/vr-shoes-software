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
    sidewaysVesc->update();
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
    sidewaysVesc->brake();
}