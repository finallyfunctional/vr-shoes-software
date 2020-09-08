#include "../../include/Sensors/VescPair.h"

VescPair::VescPair(Stream* serialForForwardVesc, Stream* serialForSidewaysVesc)
{
    forwardVesc.setSerialPort(serialForForwardVesc);
    sidewaysVesc.setSerialPort(serialForSidewaysVesc);
}

void VescPair::updateSpeed()
{
    //forwardVesc.getVescValues();
    //sidewaysVesc.getVescValues();
    //TODO
} 

Vector2D VescPair::getSpeed()
{
    return Vector2D(0,0); //TODO
}