#include "pch.h"
#include "BleCommunicator.h"

bool BleCommunicator::initialize() {
	auto adapters = SimpleBLE::Adapter::get_adapters();
	if (adapters.size() != 1) {
		return false;
	}
	auto adapter = adapters.at(0);
	std::cout << "Scanning for BLE devices for 5 seconds" << std::endl;
	adapter.scan_for(5000);
	auto devices = adapter.scan_get_results();
	for (int i = 0; i < devices.size(); i++) {
		auto device = devices[i];
		std::cout << device.identifier() << std::endl;
		std::cout << device.address() << std::endl;
		device.connect();
		auto services = device.services();
		for (int j = 0; j < services.size(); j++) {
			std::cout << services[j].uuid << std::endl;
		}
		device.disconnect();
	}
}
