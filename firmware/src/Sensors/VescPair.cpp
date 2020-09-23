#include "../../include/Sensors/VescPair.h"

VescPair::VescPair(Vesc* forwardVesc, Vesc* sidewaysVesc)
{
    this->forwardVesc = forwardVesc;
    this->sidewaysVesc = sidewaysVesc;
    this->updateTimer = new Timer(50);
}

void VescPair::update()
{
    if(!updateTimer->timeIsUp())
    {
        return;
    }
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