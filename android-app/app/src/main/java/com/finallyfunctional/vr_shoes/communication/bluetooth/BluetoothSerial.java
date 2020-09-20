package com.finallyfunctional.vr_shoes.communication.bluetooth;

import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;

import com.finallyfunctional.vr_shoes.communication.Communicator;
import java.io.IOException;
import java.io.InputStream;
import java.util.UUID;

public abstract class BluetoothSerial extends Communicator
{
    private static final String BT_SERIAL_PORT_SERVICE_ID = "00001101-0000-1000-8000-00805F9B34FB";

    public BluetoothSerial(String deviceId1, String deviceId2)
    {
        super(deviceId1, deviceId2);
    }

    protected BluetoothSocket setupSocket(BluetoothDevice device) throws IOException
    {
        UUID uuid = UUID.fromString(BT_SERIAL_PORT_SERVICE_ID);
        BluetoothSocket btSocket = device.createRfcommSocketToServiceRecord(uuid);
        btSocket.connect();
        return btSocket;
    }

    protected int readMessagesIntoQueue(InputStream btInputStream, int inputBufferIndex, byte[] inputBuffer) throws IOException
    {
        int bytesAvailable = btInputStream.available();
        if(bytesAvailable == 0)
        {
            return inputBufferIndex;
        }
        byte[] packetBytes = new byte[bytesAvailable];
        btInputStream.read(packetBytes);
        for(int i = 0; i < bytesAvailable; i++)
        {
            byte b = packetBytes[i];
            if(b == MESSAGE_TERMINATOR_ASCII)
            {
                byte[] messageBytes = new byte[inputBufferIndex];
                System.arraycopy(inputBuffer, 0, messageBytes, 0, messageBytes.length);
                storeReadMessage(new String(messageBytes, "US-ASCII"));
                inputBufferIndex = 0;
            }
            else
            {
                inputBuffer[inputBufferIndex++] = b;
            }
        }
        return inputBufferIndex;
    }
}
