#ifndef VrShoeLogger_h
#define VrShoeLogger_h

#include<Arduino.h>
#include "btSerial.h"

class VrShoeLogger {
    public:
     void println(String message);
     void print(String message);
     void setBluetooth(BtSerial btSerial);
     void logTo(bool serial, bool bluetooth);
    private:
     bool serial, bt;
     BtSerial* btSerial;
};

#endif