#ifndef VrShoeSensorData_h
#define VrShoeSensorData_h

#include "floatUtils.h"

struct VrShoeSensorData {
    float qw, qx, qy, qz;
    int encoderTicks;

    bool operator==(const VrShoeSensorData& other) {
		return FloatUtils::equals(this->qw, other.qw) 
            && FloatUtils::equals(this->qx, other.qx)
            && FloatUtils::equals(this->qy, other.qy)
            && FloatUtils::equals(this->qz, other.qz)
            && this->encoderTicks == other.encoderTicks;
	}

    bool operator!=(const VrShoeSensorData& other) {
        return !(*this == other);
    }
};

#endif