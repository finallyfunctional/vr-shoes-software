#include <Arduino.h>
#include "Wire.h"
#include <Adafruit_AHRS.h>
#include <Adafruit_SensorLab.h>
#include <Adafruit_Sensor_Calibration.h>
#include "simpleQuaternion.h"
#include "floatUtils.h"
#include "pair.h"
#include "arrayUtils.h"

class IMU {
    public:
    bool initialize();
     void updateOrientation();

     bool calibrate();

     void printQuaternion();
     void printCalibration();

     SimpleQuaternion orientation;

    private:
     void calibrateGyro();
     bool calibrateMag();
     void storeMagCalibrationValue(int value, int counts[], char valueDescriptor);
     Pair calculateMagTrimmedMiddle(int counts[]);

     void printRawValues(sensors_event_t accel, sensors_event_t gyro, sensors_event_t mag);

     static const int updateRate = 100;
     static const int magCalValueCountsSize = 1001;

     Adafruit_SensorLab lab;
     Adafruit_Sensor *accelerometer, *gyroscope, *magnetometer;
     Adafruit_Madgwick filter;
     unsigned long lastUpdateTimestamp;

     Adafruit_Sensor_Calibration_EEPROM cal;
     int magCalXValueCounts[magCalValueCountsSize], magCalYValueCounts[magCalValueCountsSize], magCalZValueCounts[magCalValueCountsSize];
};