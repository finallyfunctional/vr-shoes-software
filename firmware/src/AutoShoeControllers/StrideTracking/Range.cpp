#include "../../../include/AutoShoeControllers/StrideTracking/Range.h"

void Range::setRange(float min, float max)
{
    this->max = max;
    this->min = min;
}

bool Range::inRange(float number)
{
    return number >= min && number <= max;
}

bool Range::belowMinimum(float number)
{
    return number < min;
}

bool Range::aboveMaximum(float number)
{
    return number > max;
}