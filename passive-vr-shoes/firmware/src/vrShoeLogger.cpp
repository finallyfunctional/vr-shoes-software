#include "vrShoeLogger.h"

void VrShoeLogger::println(String message) {
    if(serial) {
        Serial.println(message);
    }
    if(bt && btSerial != nullptr) {
        btSerial->println(message);
    }
}

void VrShoeLogger::print(String message) {
    if(serial) {
        Serial.print(message);
    }
    if(bt && btSerial != nullptr) {
        btSerial->print(message);
    }
}

void VrShoeLogger::setBluetooth(BtSerial btSerial) {
    this->btSerial = &btSerial;
}

void VrShoeLogger::logTo(bool serial, bool bluetooth) {
    this->serial = serial;
    this->bt = bluetooth;
}