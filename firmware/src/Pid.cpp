#include "../include/Pid.h"

void Pid::tune(float kp, float ki, float kd)
{
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}

float Pid::compute(float input, float setPoint)
{
    long now = millis();
    float timeChange = (float)(now - lastTime);
  
    float error = setPoint - input;
    errorSum += (error * timeChange);
    float dErr = (error - lastError) / timeChange;

    lastError = error;
    lastTime = now;
  
    return kp * error + ki * errorSum + kd * dErr;
}

bool Pid::ready()
{
    return kp != 0;
}