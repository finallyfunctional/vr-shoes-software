#include "../include/Timer.h"

Timer::Timer()
{
    this->timeout = 0;
}

Timer::Timer(unsigned long timeout)
{
    this->timeout = timeout;
}

void Timer::start()
{
    startTime = millis();
}

void Timer::setTimeout(long timeout)
{
    this->timeout = timeout;
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

void Timer::printElapsedTime()
{
    unsigned long elapsedTime = millis() - startTime;
    Serial.print("Elapsed time - ");
    Serial.print(elapsedTime);
    Serial.println("ms");
}