#include "../../include/Sensors/SpeedController.h"

const char* SpeedController::DUTY_CYCLE_BOOST = "dcb";

SpeedController::SpeedController()
{
    boost = VrShoePreferences.getFloat(DUTY_CYCLE_BOOST, 0);
}

void SpeedController::setDutyCycleBoost(float boost)
{
    this->boost = boost;
    VrShoePreferences.putFloat(DUTY_CYCLE_BOOST, boost);
}