#include "../../../include/AutoShoeControllers/StrideTracking/StrideTracker.h"

StrideTracker::StrideTracker(MovementTracker* movementTracker)
{
    this->movementTracker = movementTracker;
    centerRange = new Range();
    strideLengthIndex = 0;
    clearStrideLengths();
    movementTracker->resetDistance();
    reset();
}

void StrideTracker::reset()
{
    int position = getPosition();
    if(position == ShoePositionState::MIDDLE)
    {
        startedAtCenter = true;
        centerRange->setRange(-1 * APPROX_CENTER_RADIUS, APPROX_CENTER_RADIUS);
    }
    else 
    {
        startedAtCenter = false;
        float center = 0;
        if(position == ShoePositionState::FRONT)
        {
            center = 0 - getAverageStrideLength() / 2;
        }
        else 
        {
            center = getAverageStrideLength() / 2;
        }
        centerRange->setRange(center - APPROX_CENTER_RADIUS, center + APPROX_CENTER_RADIUS);
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

float StrideTracker::getAverageStrideLength()
{
    float sum = 0;
    int count = 0;
    for(int i = 0; i < NUM_STRIDES; i++)
    {
        if(recentStrideLengths[i] == 0)
        {
            continue;
        }
        sum += recentStrideLengths[i];
        count++;
    }
    return count == 0 ? DEFAULT_STRIDE_LENGTH : sum / count;
}

void StrideTracker::clearStrideLengths()
{
    for(int i = 0; i < NUM_STRIDES; i++)
    {
        recentStrideLengths[i] = 0;
    }
    strideLengthIndex = 0;
}

void StrideTracker::storeCurrentStrideLength()
{
    int position = getPosition();
    if(startedAtCenter && position == ShoePositionState::MIDDLE) //no stride
    {
        return;
    }
    float strideLength = fabs(movementTracker->getDistanceTracked().getX());
    if((startedAtCenter && (position == ShoePositionState::FRONT || position == ShoePositionState::BEHIND)) ||
       (!startedAtCenter && position == ShoePositionState::MIDDLE)) //half stride
    {
        strideLength *= 2;
    }
    recentStrideLengths[strideLengthIndex++] = strideLength;
    if(strideLength >= NUM_STRIDES)
    {
        strideLength = 0;
    }
}