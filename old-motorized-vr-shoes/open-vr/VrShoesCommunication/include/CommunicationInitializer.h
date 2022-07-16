#pragma once

#include <VrShoesCommunication.h>
#include <Communicator.h>
#include <VrShoe.h>

class VRSHOESCOMMUNICATION_API CommunicationInitializer
{
public:
	bool initialize();
	Communicator* getCommunicator();
	VrShoe* getVrShoe1();
	VrShoe* getVrShoe2();

protected:
	virtual void setup() = 0;

	Communicator* communicator;
	VrShoe* vrShoe1;
	VrShoe* vrShoe2;
};