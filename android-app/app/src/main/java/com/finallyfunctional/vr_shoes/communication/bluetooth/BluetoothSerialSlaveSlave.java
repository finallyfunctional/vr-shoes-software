package com.finallyfunctional.vr_shoes.communication.bluetooth;

import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;

import com.finallyfunctional.vr_shoes.VrShoe;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class BluetoothSerialSlaveSlave extends BluetoothSerial
{
    private BluetoothSerialForShoe shoe1BtSerial, shoe2BtSerial;

    public BluetoothSerialSlaveSlave(BluetoothDevice shoe1, BluetoothDevice shoe2) throws IOException
    {
        super(shoe1.getName(), shoe2.getName());
        shoe1BtSerial = initiailzeSerial(getVrShoe1(), shoe1);
        shoe2BtSerial = initiailzeSerial(getVrShoe2(), shoe2);
    }

    private BluetoothSerialForShoe initiailzeSerial(VrShoe vrShoe, BluetoothDevice device) throws IOException
    {
        BluetoothSocket socket = setupSocket(device);
        BluetoothSerialForShoe serial = new BluetoothSerialForShoe();
        serial.vrShoe = vrShoe;
        serial.inputStream = socket.getInputStream();
        serial.outputStream = socket.getOutputStream();
        serial.inputBuffer = new byte[1024];
        serial.inputBufferIndex = 0;
        return serial;
    }

    @Override
    protected void readMessagesIntoQueue() throws IOException
    {
        shoe1BtSerial.inputBufferIndex = readMessagesIntoQueue(shoe1BtSerial.inputStream,
                shoe1BtSerial.inputBufferIndex, shoe1BtSerial.inputBuffer);
        shoe2BtSerial.inputBufferIndex = readMessagesIntoQueue(shoe2BtSerial.inputStream,
                shoe2BtSerial.inputBufferIndex, shoe2BtSerial.inputBuffer);
    }

    @Override
    protected void writeMessageImplementation(VrShoe vrShoe, String message) throws IOException
    {
        if(vrShoe == shoe1BtSerial.vrShoe)
        {
            shoe1BtSerial.outputStream.write(message.getBytes());
        }
        else if(vrShoe == shoe2BtSerial.vrShoe)
        {
            shoe2BtSerial.outputStream.write(message.getBytes());
        }
    }

    @Override
    protected boolean forwardSensorDataToOtherShoe()
    {
        return true;
    }

    private class BluetoothSerialForShoe
    {
        private VrShoe vrShoe;
        private InputStream inputStream;
        private OutputStream outputStream;
        private byte[] inputBuffer;
        private int inputBufferIndex;
    }
}
