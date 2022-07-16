#include <DeviceProvider.h>
#include <openvr_driver.h>
//Windows.h must always be included last
#include <windows.h>

#define HMD_DLL_EXPORT extern "C" __declspec( dllexport )

DeviceProvider deviceProvider;

HMD_DLL_EXPORT
void* HmdDriverFactory(const char* interfaceName, int* returnCode)
{
	if (strcmp(interfaceName, IServerTrackedDeviceProvider_Version) == 0) 
	{
		return &deviceProvider;
	}
	if (returnCode)
	{
		*returnCode = vr::VRInitError_Init_InterfaceNotFound;
	}
	return NULL;
}