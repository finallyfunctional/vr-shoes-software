#include <BtCommunicationInitializer.h>

void BtCommunicationInitializer::setup()
{
    if (!findPairedVrShoes())
    {
        return;
    }
    if (!startupWindowsSocket())
    {
        return;
    }
    vrShoe1BtSocket = connectToVrShoe(vrShoe1Address);
    vrShoe2BtSocket = connectToVrShoe(vrShoe2Address);
    if (vrShoe1BtSocket == NULL || vrShoe2BtSocket == NULL)
    {
        return;
    }
    vrShoe1 = new VrShoe();
    vrShoe2 = new VrShoe();
    vrShoe1->deviceId = vrShoe1Id;
    vrShoe2->deviceId = vrShoe2Id;
    
    BtCommunicator* btCommunicator = new BtCommunicator();
    btCommunicator->initializeBt(vrShoe1, vrShoe2, vrShoe1BtSocket, vrShoe2BtSocket);
    communicator = btCommunicator;
}

bool BtCommunicationInitializer::findPairedVrShoes()
{
    BLUETOOTH_DEVICE_SEARCH_PARAMS btDeviceSearchParameters =
    {
      sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS),
      1, //return authenticated devices
      0, //return remembered devices
      1, //return unknown devices
      1, //return connected devices
      1, //issue inquery
      2, //timeout multipler. Multiply this value by 1.28 seconds to get timeout.
      NULL //radio handler
    };
    BLUETOOTH_DEVICE_INFO btDeviceInfo = { sizeof(BLUETOOTH_DEVICE_INFO),0 };
    HBLUETOOTH_DEVICE_FIND btDevice = NULL;
    btDevice = BluetoothFindFirstDevice(&btDeviceSearchParameters, &btDeviceInfo);
    if (btDevice == NULL)
    {
        printf("Could not find any bluetooth devices.\r\n");
        return false;
    }
    do
    {
        wprintf(L"Checking %s.\r\n", btDeviceInfo.szName);
        if (wcsncmp(btDeviceInfo.szName, L"VR-Shoe", 6) == 0)
        {
            printf("VrShoe found!\r\n");
            if (btDeviceInfo.fAuthenticated)
            {
                printf("VrShoe is paired.\r\n");
                if (vrShoe1Address == 0)
                {
                    vrShoe1Address = btDeviceInfo.Address.ullLong;
                    vrShoe1Id = convertWcharToCharPointer(btDeviceInfo.szName);
                }
                else if (vrShoe2Address == 0)
                {
                    vrShoe2Address = btDeviceInfo.Address.ullLong;
                    vrShoe2Id = convertWcharToCharPointer(btDeviceInfo.szName);
                }
            }
            else
            {
                printf("This VrShoe is not authenticated. Please pair with it first.\r\n");
            }
        }
        if (vrShoe1Address != 0 && vrShoe2Address != 0)
        {
            printf("Found 2 VrShoes that are paired.\r\n");
            return true;
        }
    } while (BluetoothFindNextDevice(btDevice, &btDeviceInfo));

    printf("Could not find 2 VrShoes that are paired.\r\n");
    return false;
}

const char* BtCommunicationInitializer::convertWcharToCharPointer(const wchar_t * wideChar)
{
    const int bufferSize = 256;
    char* buffer = new char[bufferSize];
    for (int i = 0; i < bufferSize; i++)
    {
        buffer[i] = wideChar[i];
        if (buffer[i] == '\0')
        {
            break;
        }
    }
    return buffer;
}

bool BtCommunicationInitializer::startupWindowsSocket()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    int wsaStartupError = WSAStartup(wVersionRequested, &wsaData);
    if (wsaStartupError != 0)
    {
        printf("WSAStartup failed with error: %d\r\n", wsaStartupError);
        return false;
    }
    return true;
}

SOCKET BtCommunicationInitializer::connectToVrShoe(BTH_ADDR vrShoeBtAddress)
{
    SOCKADDR_BTH btSocketAddress;
    SOCKET btSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
    memset(&btSocketAddress, 0, sizeof(btSocketAddress));
    btSocketAddress.addressFamily = AF_BTH;
    btSocketAddress.serviceClassId = RFCOMM_PROTOCOL_UUID;
    btSocketAddress.port = 0;
    btSocketAddress.btAddr = vrShoeBtAddress;
    if (connect(btSocket, (SOCKADDR*)&btSocketAddress, sizeof(btSocketAddress)) != 0)
    {
        printf("Could not connect socket to VrShoe. Error %ld\r\n", WSAGetLastError());
        return NULL;
    }
    unsigned long nonBlockingMode = 1;
    if (ioctlsocket(btSocket, FIONBIO, (unsigned long*)&nonBlockingMode) != 0)
    {                                                                                
        printf("Could not set socket to be non-blocking.\r\n");
        return NULL;
    }
    return btSocket;
}