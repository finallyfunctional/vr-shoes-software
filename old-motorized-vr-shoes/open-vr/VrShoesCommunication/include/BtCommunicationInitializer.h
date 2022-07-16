#pragma once

#include <VrShoesCommunication.h>
#include <CommunicationInitializer.h>
#include <Communicator.h>
#include <BtCommunicator.h>
#include <Winsock2.h>
#include <Ws2bth.h>
#include <BluetoothAPIs.h>
#include <comdef.h>

class VRSHOESCOMMUNICATION_API BtCommunicationInitializer : public CommunicationInitializer
{
protected:
	void setup();

private:
	bool findPairedVrShoes();
	bool startupWindowsSocket();
	SOCKET connectToVrShoe(BTH_ADDR vrShoeBtAddress);
	const char* convertWcharToCharPointer(const wchar_t* wideChar);

	const char* vrShoe1Id;
	BTH_ADDR vrShoe1Address;
	SOCKET vrShoe1BtSocket;
	const char* vrShoe2Id;
	BTH_ADDR vrShoe2Address;
	SOCKET vrShoe2BtSocket;
};