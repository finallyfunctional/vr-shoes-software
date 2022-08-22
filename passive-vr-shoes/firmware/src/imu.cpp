#include "imu.h"

bool IMU::initialize(Logger logger) {
    this->logger = &logger;
    this->logger->println("Initializing Adafruit LSM6DSOX + LIS3MDL - Precision 9 DoF IMU");
    magnetometer = lab.getMagnetometer();
    if (!magnetometer) {
        this->logger->println("ERROR - Could not find a magnetometer");
        return false;
    }

    accelerometer = lab.getAccelerometer();
    if(!accelerometer) {
        this->logger->println("ERROR - Could not find accelerometer");
        return false;
    }

    gyroscope = lab.getGyroscope();
    if(!gyroscope) {
        this->logger->println("ERROR - Could not find gyroscope");
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
    logger->print("Raw: ");
    logger->print(accel.acceleration.x); logger->print(", ");
    logger->print(accel.acceleration.y); logger->print(", ");
    logger->print(accel.acceleration.z); logger->print(", ");
    logger->print(gyro.gyro.x); logger->print(", ");
    logger->print(gyro.gyro.y); logger->print(", ");
    logger->print(gyro.gyro.z); logger->print(", ");
    logger->print(mag.magnetic.x); logger->print(", ");
    logger->print(mag.magnetic.y); logger->print(", ");
    logger->print(mag.magnetic.z); logger->println("");
}

void IMU::printQuaternion() {
  logger->print("Quaternion: ");
  logger->print(orientation.w);
  logger->print(", ");
  logger->print(orientation.x);
  logger->print(", ");
  logger->print(orientation.y);
  logger->print(", ");
  logger->println(orientation.z); 
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
    logger->println("Place IMU on flat, stable surface within 3 seconds");
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

    logger->println("Zero rate offset in radians/s: ");
    logger->print(midX); 
    logger->print(", ");
    logger->print(midY); 
    logger->print(", ");
    logger->println(midZ);

    cal.gyro_zerorate[0] = midX;
    cal.gyro_zerorate[1] = midY;
    cal.gyro_zerorate[2] = midZ;
}

void IMU::storeMagCalibrationValue(int value, int counts[], char valueDescriptor) {
    int maxValue = round(magCalValueCountsSize / 2);
    int minValue = maxValue * -1;
    if(value > maxValue || value < minValue) {
        logger->print("Magnetic ");
        logger->print(valueDescriptor);
        logger->println(" value out of range");
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
    logger->println("Please twist the IMU for 3 minutes");
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

    logger->print(" Hard offset: (");
    logger->print(midX); logger->print(", ");
    logger->print(midY); logger->print(", ");
    logger->print(midZ); logger->print(")"); 
    logger->print(" Field: (");
    logger->print(fieldX); logger->print(", ");
    logger->print(fieldY); logger->print(", ");
    logger->print(fieldZ); logger->println(")");

    if(FloatUtils::inRangeInclusive(fieldX, minField, maxField) &&
       FloatUtils::inRangeInclusive(fieldY, minField, maxField) &&
       FloatUtils::inRangeInclusive(fieldZ, minField, maxField)) {
        
        cal.mag_hardiron[0] = midX;
        cal.mag_hardiron[1] = midY;
        cal.mag_hardiron[2] = midZ;
        return true;
    }
    logger->println("Field values not in range (25-65)");
    return false;
}

bool IMU::calibrate() {
    logger->println("Starting IMU calibration");
    if(!cal.begin()) {
        logger->println("Calibration failed to start");
        return false;
    }
    cal.loadCalibration();
    calibrateGyro();
    if(!calibrateMag()) {
        logger->println("ERROR - Magnometer failed to calibrate");
        return false;
    }
    cal.saveCalibration();
    printCalibration();
    return true;
}

void IMU::printCalibration() {
    logger->print("Gyro cal: (");
    logger->print(cal.gyro_zerorate[0]);
    logger->print(", ");
    logger->print(cal.gyro_zerorate[1]);
    logger->print(", ");
    logger->print(cal.gyro_zerorate[2]);
    logger->println(")");
    logger->print("Mag cal: (");
    logger->print(cal.mag_hardiron[0]);
    logger->print(", ");
    logger->print(cal.mag_hardiron[1]);
    logger->print(", ");
    logger->print(cal.mag_hardiron[2]);
    logger->println(")");
    logger->println("Calibration successful");
}