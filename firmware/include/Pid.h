#ifndef Pid_h
#define Pid_h

#include <Arduino.h>

class Pid
{
    public:
    void tune(float kp, float ki, float kd);
    float compute(float input, float setPoint);
    bool ready();

    private:
    float kp, ki, kd, errorSum, lastError;
    long lastTime;
};

#endif