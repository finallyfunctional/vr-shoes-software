#include "../../../include/AutoShoeControllers/StrideTracking/StrideTracker.h"

StrideTracker::StrideTracker(MovementTracker* movementTracker)
{
    this->movementTracker = movementTracker;
    centerRange = new Range();
    strideLength = DEFAULT_STRIDE_LENGTH;
    centerRadius = DEFAULT_CENTER_RADUIS;
    centerOffset = DEFAULT_CENTER_OFFSET;
    calculateStrideLength = true;
    movementTracker->resetDistance();
    reset();
}

void StrideTracker::recenter()
{
    movementTracker->resetDistance();
    centerRange->setRange(-1 * centerRadius + centerOffset, centerRadius + centerOffset);
    startedAtCenter = true;
}

void StrideTracker::reset()
{
    int position = getPosition();
    if(position == ShoePositionState::MIDDLE)
    {
        return;
    }
    else 
    {
        startedAtCenter = false;
        float center = 0;
        if(position == ShoePositionState::FRONT)
        {
            center = 0 - strideLength / 2 + centerOffset;
        }
        else 
        {
            center = strideLength / 2 + centerOffset;
        }
        centerRange->setRange(center - centerRadius, center + centerRadius);
    }
    movementTracker->resetDistance();
}

int StrideTracker::getPosition()
{
    float currentDistance = movementTracker->getDistanceTracked().getX();
    if(centerRange->inRange(currentDistance))
    {
        return ShoePositionState::MIDDLE;
    }
    else if(centerRange->belowMinimum(currentDistance))
    {
        return ShoePositionState::BEHIND;
    }
    else 
    {
        return ShoePositionState::FRONT;
    }
}

void StrideTracker::updateCurrentStrideLength()
{
    if(!calculateStrideLength)
    {
        return;
    }
    int position = getPosition();
    if(startedAtCenter && position == ShoePositionState::MIDDLE) //no stride
    {
        return;
    }
    strideLength = fabs(movementTracker->getDistanceTracked().getX());
    if((startedAtCenter && (position == ShoePositionState::FRONT || position == ShoePositionState::BEHIND)) ||
       (!startedAtCenter && position == ShoePositionState::MIDDLE)) //half stride
    {
        strideLength *= 2;
    }
}

float StrideTracker::getCenterRadius()
{
    return centerRadius;
}

void StrideTracker::setCenterRadius(float centerRadius)
{
    this->centerRadius = centerRadius;
}

float StrideTracker::getStrideLength()
{
    return strideLength;
}

bool StrideTracker::isStrideLengthCalculated()
{
    return calculateStrideLength;
}

void StrideTracker::shouldCalculateStrideLength(bool calculate)
{
    this->calculateStrideLength = calculate;
}

float StrideTracker::getCenterOffset()
{
    return centerOffset;
}

void StrideTracker::setCenterOffset(float offset)
{
    this->centerOffset = offset;
}