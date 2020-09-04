package com.finallyfunctional.vr_shoes.communication;

import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;

import com.finallyfunctional.vr_shoes.communication.Communicator;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.UUID;

public class BluetoothSerial extends Communicator
{
    private OutputStream btOutputStream;
    private InputStream btInputStream;
    byte[] inputBuffer;
    int inputBufferIndex;
    private boolean keepLoopAlive;
    private Queue<String> recievedMessages;

    private static final String BT_SERIAL_PORT_SERVICE_ID = "00001101-0000-1000-8000-00805F9B34FB";
    private static final int NEW_LINE_ASCII_CODE = 10;

    public BluetoothSerial(BluetoothDevice device) throws IOException
    {
         inputBuffer = new byte[1024];
         recievedMessages = new PriorityQueue<>();
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
            if(b == NEW_LINE_ASCII_CODE)
            {
                byte[] messageBytes = new byte[inputBufferIndex];
                System.arraycopy(inputBuffer, 0, messageBytes, 0, messageBytes.length);
                String message = new String(messageBytes, "US-ASCII");
                inputBufferIndex = 0;
            }
            else
            {
                inputBuffer[inputBufferIndex++] = b;
            }
        }
    }

    protected String readNextMessage()
    {
        return recievedMessages.size() == 0 ? "" : recievedMessages.remove();
    }

    protected void writeMessage(String message) throws IOException
    {
        btOutputStream.write(message.getBytes());
    }
}
