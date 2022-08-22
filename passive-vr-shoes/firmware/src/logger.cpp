#include "logger.h"

void Logger::println(String message) {
    if(serial) {
        Serial.println(message);
    }
    if(bt && btSerial != nullptr) {
        btSerial->println(message);
    }
}

void Logger::print(String message) {
    if(serial) {
        Serial.print(message);
    }
    if(bt && btSerial != nullptr) {
        btSerial->print(message);
    }
}

void Logger::setBluetooth(BtSerial btSerial) {
    this->btSerial = &btSerial;
}

void Logger::logTo(bool serial, bool bluetooth) {
    this->serial = serial;
    this->bt = bluetooth;
}