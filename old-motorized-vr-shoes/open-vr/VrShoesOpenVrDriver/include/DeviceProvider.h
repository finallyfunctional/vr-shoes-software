#pragma once
#include <ControllerDriver.h>
#include <openvr_driver.h>
//Windows.h must always be included last
#include <windows.h>

using namespace vr;

class DeviceProvider : public IServerTrackedDeviceProvider
{
public:
	EVRInitError Init(IVRDriverContext* pDriverContext);
	void Cleanup();
	const char* const* GetInterfaceVersions();
	void RunFrame();
	bool ShouldBlockStandbyMode();
	void EnterStandby();
	void LeaveStandby();

private:
	ControllerDriver* controllerDriver;
};