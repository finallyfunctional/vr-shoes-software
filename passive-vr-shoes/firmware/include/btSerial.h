#ifndef BtSerial_h
#define BtSerial_h

#include<Arduino.h>
#include <Preferences.h>
#include <BluetoothSerial.h>

class BtSerial {
    public:
     void initialize();
     void println(String message);
     void print(String message);

    private:
     String getShoeId();

     BluetoothSerial serialBt;
     Preferences btPerferences;

     static const char* deviceIdPrefix;
     static const char* shoeIdKey;
};

#endif