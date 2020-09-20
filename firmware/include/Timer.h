#ifndef Timer_h
#define Timer_h

#include <Arduino.h>

class Timer 
{
    public:

    Timer(unsigned long timeout);
    void start();
    bool timeIsUp();
    unsigned long getElapsedTime();
    void printElapsedTime();

    private:

    unsigned long startTime;
    unsigned long timeout;
    bool temp;

};

#endif