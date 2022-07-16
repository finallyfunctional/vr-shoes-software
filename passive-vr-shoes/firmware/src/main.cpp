#include <Arduino.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

#define ENCODER_A 17
#define ENCODER_B 16

#define IMU_INTERRUPT_PIN 19

MPU6050 imu;
uint8_t imuStatus;
uint8_t imuIntStatus;
bool dmpReady = false;
uint16_t packetSize;
uint8_t fifoBuffer[64];
Quaternion imuQuaternion;

volatile bool imuInterrupt = false;
void dmpDataReady() {
    imuInterrupt = true;
}

void encoderAEvent() {
    Serial.println("encoder A event");
}

void encoderBEvent() {
    Serial.println("encoder B event");
}

void setup() {
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock

    Serial.begin(115200);

    Serial.println("Initializing MPU-6050");
    imu.initialize();
    pinMode(IMU_INTERRUPT_PIN, INPUT);

    Serial.println("Initializing DMP");
    imuStatus = imu.dmpInitialize();

    if(imuStatus == 0) {
        Serial.println("Enabling DMP");
        imu.setDMPEnabled(true);

        Serial.println("Enabling IMU interrupt");
        attachInterrupt(digitalPinToInterrupt(IMU_INTERRUPT_PIN), dmpDataReady, RISING);
        imuIntStatus = imu.getIntStatus();

        Serial.println("DMP ready");
        dmpReady = true;
        packetSize = imu.dmpGetFIFOPacketSize();

    } else {
        Serial.print("DMP intialization failed with code: ");
        Serial.println(imuStatus);
    }

    Serial.println("Enabling encoder interrupts");
    pinMode(ENCODER_A, INPUT);
    pinMode(ENCODER_B, INPUT);
    attachInterrupt(digitalPinToInterrupt(ENCODER_A), encoderAEvent, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_B), encoderBEvent, CHANGE);
}

void loop() {
    if (!dmpReady) {
        return;
    }
    // if (imu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
    //     imu.dmpGetQuaternion(&imuQuaternion, fifoBuffer);
    //     Serial.print("quat\t");
    //     Serial.print(imuQuaternion.w);
    //     Serial.print("\t");
    //     Serial.print(imuQuaternion.x);
    //     Serial.print("\t");
    //     Serial.print(imuQuaternion.y);
    //     Serial.print("\t");
    //     Serial.println(imuQuaternion.z);
    // }
}
