#include <Arduino.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

class IMU {
    public:
     static bool initialize();
     static bool isNewData();
     static void updateOrientation();

     static Quaternion orientation;

    private:
     static MPU6050 imu;
     static uint8_t imuStatus;
     static uint8_t imuIntStatus;
     static uint16_t packetSize;
     static uint8_t fifoBuffer[64];
};