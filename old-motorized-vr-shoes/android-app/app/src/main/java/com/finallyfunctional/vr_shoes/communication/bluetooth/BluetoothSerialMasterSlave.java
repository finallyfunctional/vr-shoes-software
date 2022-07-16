package com.finallyfunctional.vr_shoes.communication.bluetooth;

import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;

import com.finallyfunctional.vr_shoes.VrShoe;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class BluetoothSerialMasterSlave extends BluetoothSerial
{
    private InputStream inputStream;
    private OutputStream outputStream;
    private byte[] inputBuffer;
    private int inputBufferIndex;

    public BluetoothSerialMasterSlave(BluetoothDevice slaveDevice, String masterId) throws IOException
    {
        super(slaveDevice.getName(), masterId);
        BluetoothSocket socket = setupSocket(slaveDevice);
        inputStream = socket.getInputStream();
        outputStream = socket.getOutputStream();
        inputBuffer = new byte[1024];
        inputBufferIndex = 0;
    }

    @Override
    protected void readMessagesIntoQueue() throws IOException
    {
        inputBufferIndex = readMessagesIntoQueue(inputStream, inputBufferIndex, inputBuffer);
    }

    @Override
    protected void writeMessageImplementation(VrShoe vrShoe, String message) throws IOException
    {
        outputStream.write(message.getBytes());
    }

    @Override
    protected boolean forwardSensorDataToOtherShoe()
    {
        return false;
    }
}
