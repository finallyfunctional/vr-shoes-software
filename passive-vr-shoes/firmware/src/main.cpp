#include <Arduino.h>
#include "Wire.h"
#include "imu.h"
#include "encoder.h"

#define ENCODER_A 17
#define ENCODER_B 16

bool imuReady = false;

void setup() {
    Wire.begin();
    Wire.setClock(400000);

    Serial.begin(115200);

    Serial.println("Initializing components");
    imuReady = IMU::initialize();
    Encoder::initialize(ENCODER_A, ENCODER_B);
}

void loop() {
    if (!imuReady) {
        return;
    }
    IMU::printOrientation();
    Serial.print("Encoder ticks: ");
    Serial.println(Encoder::ticks);
}
