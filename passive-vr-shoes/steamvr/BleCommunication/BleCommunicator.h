#pragma once

#include "pch.h"
#include <iostream>
#include "BleCommunication.h"
#include "simpleble/SimpleBLE.h"

using namespace SimpleBLE;

class BLECOMMUNICATION_API BleCommunicator {
public:
	bool initialize();
};