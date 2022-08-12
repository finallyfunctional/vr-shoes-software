#pragma once

#include "pch.h"
#include "BleCommunication.h"
#include "simpleble/SimpleBLE.h"

using namespace SimpleBLE;

class BLECOMMUNICATION_API BleCommunicator {
public:
	bool initialize();
	std::string read();
private:
	bool getBleAdapter(SimpleBLE::Adapter& adapter);
	bool isVrShoeDevice(SimpleBLE::Peripheral device);
	bool hasVrShoeService(SimpleBLE::Peripheral device);

	SimpleBLE::Peripheral vrShoe;

	const char* SERVICE_ID = "a0661d83-4017-4d5e-a7a9-330c74de60a3";
	const char* CHARACTERISTIC_ID = "241ff4a0-c71f-4843-9c12-11e8cfea0079";
};