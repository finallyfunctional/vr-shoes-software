#ifndef Timer_h
#define Timer_h

#include <Arduino.h>

class Timer 
{
    public:

    Timer();
    Timer(unsigned long timeout);
    void start();
    bool timeIsUp();
    void setTimeout(long timeout);
    unsigned long getElapsedTime();
    void printElapsedTime();

    private:

    unsigned long startTime;
    unsigned long timeout;

};

#endif