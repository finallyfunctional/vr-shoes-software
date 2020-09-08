#ifndef Vesc_h
#define Vesc_h

#include <Arduino.h>
#include <VescUart.h>
#include "../Vector2D.h"
#include "./Sensors/SpeedSensor.h"

class VescPair : public SpeedSensor
{
    public:
    VescPair(Stream* serialForForwardVesc, Stream* serialForSidewaysVesc);
    void updateSpeed();
    Vector2D getSpeed();

    private:
    VescUart forwardVesc;
    VescUart sidewaysVesc;
};

#endif