#include "imu.h"

MPU6050 IMU::imu;
uint8_t IMU::imuStatus;
uint8_t IMU::imuIntStatus;
uint16_t IMU::packetSize;
uint8_t IMU::fifoBuffer[64];
Quaternion IMU::orientation;

bool IMU::initialize() {
    Serial.println("Initializing MPU-6050");
    imu.initialize();
    Serial.println("Initializing DMP");
    imuStatus = imu.dmpInitialize();
    if(imuStatus != 0) {
        Serial.print("ERROR - DMP intialization failed with code: ");
        Serial.println(imuStatus);
        return false;
    }
    Serial.println("Enabling DMP");
    imu.setDMPEnabled(true);
    Serial.println("DMP ready");
    packetSize = imu.dmpGetFIFOPacketSize();
    return true;
}

bool IMU::isNewData() {
    return imu.dmpGetCurrentFIFOPacket(fifoBuffer);
}

void IMU::updateOrientation() {
    imu.dmpGetQuaternion(&orientation, fifoBuffer);
}