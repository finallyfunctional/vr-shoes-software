#ifndef Logger_h
#define Logger_h

#include<Arduino.h>
#include "btSerial.h"

class Logger {
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