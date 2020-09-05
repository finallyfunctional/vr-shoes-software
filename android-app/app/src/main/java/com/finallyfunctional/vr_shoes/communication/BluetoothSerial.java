package com.finallyfunctional.vr_shoes.communication;

import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

public class BluetoothSerial extends Communicator
{
    private OutputStream btOutputStream;
    private InputStream btInputStream;
    byte[] inputBuffer;
    int inputBufferIndex;
    private boolean keepLoopAlive;

    private static final String BT_SERIAL_PORT_SERVICE_ID = "00001101-0000-1000-8000-00805F9B34FB";

    public BluetoothSerial(BluetoothDevice device) throws IOException
    {
        inputBuffer = new byte[1024];
        setupStreams(device);
    }

    private void setupStreams(BluetoothDevice device) throws IOException
    {
        UUID uuid = UUID.fromString(BT_SERIAL_PORT_SERVICE_ID);
        BluetoothSocket btSocket = device.createRfcommSocketToServiceRecord(uuid);
        btSocket.connect();
        btOutputStream = btSocket.getOutputStream();
        btInputStream = btSocket.getInputStream();
    }

    public void startReading()
    {
        keepLoopAlive = true;
        final Thread loop = new Thread(new Runnable()
        {
            @Override
            public void run()
            {
                while(keepLoopAlive && !Thread.currentThread().isInterrupted())
                {
                    try
                    {
                        loop();
                    }
                    catch(IOException ex)
                    {
                        ex.printStackTrace();
                        //TODO handle the error
                    }
                }
            }
        });
        loop.start();
    }

    public void stopReading()
    {
        keepLoopAlive = false;
    }

    private void loop() throws IOException
    {
        int bytesAvailable = btInputStream.available();
        if(bytesAvailable == 0)
        {
            return;
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
    }

    protected void writeMessageImplementation(String message) throws IOException
    {
        btOutputStream.write(message.toLowerCase().getBytes());
    }
}
