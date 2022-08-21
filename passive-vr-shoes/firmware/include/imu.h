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
     static bool initialize();
     static void updateOrientation();

     static bool calibrate();

     static void printQuaternion();
     static void printCalibration();

     static SimpleQuaternion orientation;

    private:
     static void calibrateGyro();
     static bool calibrateMag();
     static void storeMagCalibrationValue(int value, int counts[], char valueDescriptor);
     static Pair calculateMagTrimmedMiddle(int counts[]);

     static void printRawValues(sensors_event_t accel, sensors_event_t gyro, sensors_event_t mag);

     static const int updateRate = 100;
     static const int magCalValueCountsSize = 1001;

     static Adafruit_SensorLab lab;
     static Adafruit_Sensor *accelerometer, *gyroscope, *magnetometer;
     static Adafruit_Madgwick filter;
     static unsigned long lastUpdateTimestamp;

     static Adafruit_Sensor_Calibration_EEPROM cal;
     static int magCalXValueCounts[magCalValueCountsSize], magCalYValueCounts[magCalValueCountsSize], magCalZValueCounts[magCalValueCountsSize];
};