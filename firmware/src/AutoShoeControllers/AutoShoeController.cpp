#include "../../include/AutoShoeControllers/AutoShoeController.h"

AutoShoeController::AutoShoeController(Sensors* sensors, Communicator* communicator)
{
    this->communicator = communicator;
    this->sensors = sensors;
}