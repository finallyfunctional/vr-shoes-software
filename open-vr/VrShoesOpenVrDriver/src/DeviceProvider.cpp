#include <DeviceProvider.h>

EVRInitError DeviceProvider::Init(IVRDriverContext* pDriverContext)
{
    EVRInitError initError = InitServerDriverContext(pDriverContext);
    if (initError != EVRInitError::VRInitError_None)
    {
        return initError;
    }
    
    VRDriverLog()->Log("Initializing Finally Functional's VR Shoes");

    controllerDriver = new ControllerDriver();
    VRServerDriverHost()->TrackedDeviceAdded("ffVrShoes_controller", TrackedDeviceClass_Controller, controllerDriver); 

    return vr::VRInitError_None;
}

void DeviceProvider::Cleanup()
{
    delete controllerDriver;
    controllerDriver = NULL;
}
const char* const* DeviceProvider::GetInterfaceVersions()
{
    return k_InterfaceVersions;
}

void DeviceProvider::RunFrame()
{
    controllerDriver->RunFrame();
}

bool DeviceProvider::ShouldBlockStandbyMode()
{
    return false;
}

void DeviceProvider::EnterStandby() 
{
    controllerDriver->EnterStandby();
}

void DeviceProvider::LeaveStandby() 
{
    controllerDriver->LeaveStandby();
}