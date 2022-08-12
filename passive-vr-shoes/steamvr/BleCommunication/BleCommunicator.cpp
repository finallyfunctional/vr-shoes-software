#include "pch.h"
#include "BleCommunicator.h"

bool BleCommunicator::getBleAdapter(SimpleBLE::Adapter& adapter) {
	auto adapters = SimpleBLE::Adapter::get_adapters();
	if (adapters.size() != 1) {
		return false;
	}
	adapter = adapters[0];
	return true;
}

bool BleCommunicator::isVrShoeDevice(SimpleBLE::Peripheral device) {
	return device.identifier().find("VR-Shoe") != std::string::npos;
}

bool BleCommunicator::hasVrShoeService(SimpleBLE::Peripheral device) {
	auto services = device.services();
	for (int i = 0; i < services.size(); i++) {
		auto service = services[i];
		if (service.uuid == SERVICE_ID) {
			for (int j = 0; j < service.characteristics.size(); j++) {
				if (service.characteristics[j] == CHARACTERISTIC_ID) {
					return true;
				}
			}
		}
	}
	return false;
}

bool BleCommunicator::initialize() {
	SimpleBLE::Adapter adapter;
	if (!getBleAdapter(adapter)) {
		return false;
	}
	adapter.scan_for(5000);
	auto devices = adapter.scan_get_results();
	for (int i = 0; i < devices.size(); i++) {
		auto device = devices[i];
		if (!isVrShoeDevice(device)) {
			continue;
		}
		device.connect();
		if (!hasVrShoeService(device)) {
			continue;
		}
		vrShoe = device;
		return true;
	}
	return false;
}

std::string BleCommunicator::read() {
	return vrShoe.read(SERVICE_ID, CHARACTERISTIC_ID);
}