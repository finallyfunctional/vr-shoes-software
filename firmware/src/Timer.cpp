#include "../include/Timer.h"

Timer::Timer(unsigned long timeout)
{
    this->timeout = timeout;
}

void Timer::start()
{
    startTime = millis();
}

bool Timer::timeIsUp()
{
    unsigned long current = millis();
    return (current - startTime) >= timeout;
}

unsigned long Timer::getElapsedTime()
{
    unsigned long current = millis();
    return current - startTime;
}