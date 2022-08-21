#include "imu.h"

bool IMU::initialize() {
    Serial.println("Initializing Adafruit LSM6DSOX + LIS3MDL - Precision 9 DoF IMU");
    magnetometer = lab.getMagnetometer();
    if (!magnetometer) {
        Serial.println("ERROR - Could not find a magnetometer");
        return false;
    }

    accelerometer = lab.getAccelerometer();
    if(!accelerometer) {
        Serial.println("ERROR - Could not find accelerometer");
        return false;
    }

    gyroscope = lab.getGyroscope();
    if(!gyroscope) {
        Serial.println("ERROR - Could not find gyroscope");
        return false;
    }

    magnetometer->printSensorDetails();
    accelerometer->printSensorDetails();
    gyroscope->printSensorDetails();

    filter.begin(updateRate);

    cal.saveCalibration();

    lastUpdateTimestamp = millis();

    return true;
}

void IMU::printRawValues(sensors_event_t accel, sensors_event_t gyro, sensors_event_t mag) {
    Serial.print("Raw: ");
    Serial.print(accel.acceleration.x, 4); Serial.print(", ");
    Serial.print(accel.acceleration.y, 4); Serial.print(", ");
    Serial.print(accel.acceleration.z, 4); Serial.print(", ");
    Serial.print(gyro.gyro.x, 4); Serial.print(", ");
    Serial.print(gyro.gyro.y, 4); Serial.print(", ");
    Serial.print(gyro.gyro.z, 4); Serial.print(", ");
    Serial.print(mag.magnetic.x, 4); Serial.print(", ");
    Serial.print(mag.magnetic.y, 4); Serial.print(", ");
    Serial.print(mag.magnetic.z, 4); Serial.println("");
}

void IMU::printQuaternion() {
  Serial.print("Quaternion: ");
  Serial.print(orientation.w, 4);
  Serial.print(", ");
  Serial.print(orientation.x, 4);
  Serial.print(", ");
  Serial.print(orientation.y, 4);
  Serial.print(", ");
  Serial.println(orientation.z, 4); 
}

void IMU::updateOrientation() {

    if ((millis() - lastUpdateTimestamp) < (1000 / updateRate)) {
        return;
    }
    lastUpdateTimestamp = millis();

    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t mag; 

    accelerometer->getEvent(&accel);
    gyroscope->getEvent(&gyro);
    magnetometer->getEvent(&mag);

    cal.calibrate(mag);
    cal.calibrate(gyro);

    filter.update(
        gyro.gyro.x * SENSORS_RADS_TO_DPS, 
        gyro.gyro.y * SENSORS_RADS_TO_DPS, 
        gyro.gyro.z * SENSORS_RADS_TO_DPS, 
        accel.acceleration.x, 
        accel.acceleration.y, 
        accel.acceleration.z, 
        mag.magnetic.x,
        mag.magnetic.y, 
        mag.magnetic.z
    );

    filter.getQuaternion(
        &orientation.w,
        &orientation.x,
        &orientation.y,
        &orientation.z
    );
}

void IMU::calibrateGyro() {
    Serial.println("Place IMU on flat, stable surface within 3 seconds");
    delay(3000);

    float minX = 0, maxX = 0, midX = 0;
    float minY = 0, maxY = 0, midY = 0;
    float minZ = 0, maxZ = 0, midZ = 0;
    sensors_event_t event;

    for(int i = 0; i < 10 * 100; i++) {
        gyroscope->getEvent(&event);
        minX = min(minX, event.gyro.x);
        minY = min(minY, event.gyro.y);
        minZ = min(minZ, event.gyro.z);
  
        maxX = max(maxX, event.gyro.x);
        maxY = max(maxY, event.gyro.y);
        maxZ = max(maxZ, event.gyro.z);
  
        midX = (maxX + minX) / 2;
        midY = (maxY + minY) / 2;
        midZ = (maxZ + minZ) / 2;
        delay(10);
    }

    Serial.println("Zero rate offset in radians/s: ");
    Serial.print(midX, 4); 
    Serial.print(", ");
    Serial.print(midY, 4); 
    Serial.print(", ");
    Serial.println(midZ, 4);

    cal.gyro_zerorate[0] = midX;
    cal.gyro_zerorate[1] = midY;
    cal.gyro_zerorate[2] = midZ;
}

void IMU::storeMagCalibrationValue(int value, int counts[], char valueDescriptor) {
    int maxValue = round(magCalValueCountsSize / 2);
    int minValue = maxValue * -1;
    if(value > maxValue || value < minValue) {
        Serial.print("Magnetic ");
        Serial.print(valueDescriptor);
        Serial.println(" value out of range");
    }
    else {
        counts[value + maxValue] = ++counts[value + maxValue];
    }
}

Pair IMU::calculateMagTrimmedMiddle(int counts[]) {
    int numValues = 0;
    for(int i = 0; i < magCalValueCountsSize; i++) {
        numValues += counts[i];
    }
    int fivePercentCount = numValues * 0.05;

    int valueIndex = 0;
    int trimmedMinIndex;
    for(int i = 0; i < magCalValueCountsSize; i++) {
        valueIndex += counts[i];
        if(valueIndex > fivePercentCount) {
            trimmedMinIndex = i;
            break;
        }
    }
    valueIndex = 0;
    int trimmedMaxIndex;
    for(int i = magCalValueCountsSize - 1; i >= 0; i--) {
        valueIndex += counts[i];
        if(valueIndex > fivePercentCount) {
            trimmedMaxIndex = i;
            break;
        }
    }
    int trimmedMin = trimmedMinIndex - round(magCalValueCountsSize / 2);
    int trimmedMax = trimmedMaxIndex - round(magCalValueCountsSize / 2);

    return {
        value1: trimmedMin,
        value2: trimmedMax
    };
}


bool IMU::calibrateMag() {
    Serial.println("Please twist the IMU for 3 minutes");
    sensors_event_t event;
    int minValue = -500;
    int maxValue = 500;
    float minField = 25;
    float maxField = 65;
    ArrayUtils::setDefaultValues(magCalXValueCounts, magCalValueCountsSize, 0);
    ArrayUtils::setDefaultValues(magCalYValueCounts, magCalValueCountsSize, 0);
    ArrayUtils::setDefaultValues(magCalZValueCounts, magCalValueCountsSize, 0);
    unsigned long start = millis();

    while(start + 1000 * 60 * 3 > millis()) {
        magnetometer->getEvent(&event);
        storeMagCalibrationValue(round(event.magnetic.x), magCalXValueCounts, 'X');
        storeMagCalibrationValue(round(event.magnetic.y), magCalYValueCounts, 'Y');
        storeMagCalibrationValue(round(event.magnetic.z), magCalZValueCounts, 'Z');
        delay(10);
    }
    Pair minMaxPairX = calculateMagTrimmedMiddle(magCalXValueCounts);
    Pair minMaxPairY = calculateMagTrimmedMiddle(magCalYValueCounts);
    Pair minMaxPairZ = calculateMagTrimmedMiddle(magCalZValueCounts);
    float minX = minMaxPairX.value1;
    float maxX = minMaxPairX.value2;
    float minY = minMaxPairY.value1;
    float maxY = minMaxPairY.value2;
    float minZ = minMaxPairZ.value1;
    float maxZ = minMaxPairZ.value2;
    float midX = (maxX + minX) / 2;
    float midY = (maxY + minY) / 2;
    float midZ = (maxZ + minZ) / 2;
    float fieldX = (maxX - minX) / 2;
    float fieldY = (maxY - minY) / 2;
    float fieldZ = (maxZ - minZ) / 2;

    Serial.print(" Hard offset: (");
    Serial.print(midX); Serial.print(", ");
    Serial.print(midY); Serial.print(", ");
    Serial.print(midZ); Serial.print(")"); 
    Serial.print(" Field: (");
    Serial.print(fieldX); Serial.print(", ");
    Serial.print(fieldY); Serial.print(", ");
    Serial.print(fieldZ); Serial.println(")");

    if(FloatUtils::inRangeInclusive(fieldX, minField, maxField) &&
       FloatUtils::inRangeInclusive(fieldY, minField, maxField) &&
       FloatUtils::inRangeInclusive(fieldZ, minField, maxField)) {
        
        cal.mag_hardiron[0] = midX;
        cal.mag_hardiron[1] = midY;
        cal.mag_hardiron[2] = midZ;
        return true;
    }
    Serial.println("Field values not in range (25-65)");
    return false;
}

bool IMU::calibrate() {
    Serial.println("Starting IMU calibration");
    if(!cal.begin()) {
        Serial.println("Calibration failed to start");
        return false;
    }
    cal.loadCalibration();
    calibrateGyro();
    if(!calibrateMag()) {
        Serial.println("ERROR - Magnometer failed to calibrate");
        return false;
    }
    cal.saveCalibration();
    printCalibration();
    return true;
}

void IMU::printCalibration() {
    Serial.print("Gyro cal: (");
    Serial.print(cal.gyro_zerorate[0]);
    Serial.print(", ");
    Serial.print(cal.gyro_zerorate[1]);
    Serial.print(", ");
    Serial.print(cal.gyro_zerorate[2]);
    Serial.println(")");
    Serial.print("Mag cal: (");
    Serial.print(cal.mag_hardiron[0]);
    Serial.print(", ");
    Serial.print(cal.mag_hardiron[1]);
    Serial.print(", ");
    Serial.print(cal.mag_hardiron[2]);
    Serial.println(")");
    Serial.println("Calibration successful");
}