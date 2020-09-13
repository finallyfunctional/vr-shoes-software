package com.finallyfunctional.vr_shoes.communication;

import com.finallyfunctional.vr_shoes.VrShoe;
import com.finallyfunctional.vr_shoes.communication.commands.CommandConstants;
import com.finallyfunctional.vr_shoes.communication.commands.Ping;
import com.finallyfunctional.vr_shoes.communication.commands.ReadDistanceFromOrigin;
import com.finallyfunctional.vr_shoes.communication.commands.ReadSensorData;
import com.finallyfunctional.vr_shoes.communication.commands.ResetOrigin;
import com.finallyfunctional.vr_shoes.communication.commands.SetRpm;
import com.google.gson.Gson;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Map;
import java.util.PriorityQueue;
import java.util.Queue;

public abstract class Communicator
{
    private ArrayList<ICommunicatorObserver> observers;
    private Queue<String> recievedMessages;
    private Queue<String> messagesToSend;
    private Gson gson;
    private boolean keepLoopAlive;
    private VrShoe vrShoe1;

    public static char MESSAGE_TERMINATOR = '\n';
    public static final int MESSAGE_TERMINATOR_ASCII = 10;

    public Communicator()
    {
        gson = new Gson();
        observers = new ArrayList<>();
        recievedMessages = new PriorityQueue<>();
        messagesToSend = new PriorityQueue<>();
        vrShoe1 = new VrShoe();
    }

    public void addObserver(ICommunicatorObserver observer)
    {
        observers.add(observer);
    }

    public void removeObserver(ICommunicatorObserver observer)
    {
        observers.remove(observer);
    }

    public void start()
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

    public void stop()
    {
        keepLoopAlive = false;
    }

    private void loop() throws IOException
    {
        readMessagesIntoQueue();
        while(recievedMessages.size() > 0)
        {
            handleRecievedMessage(recievedMessages.remove());
        }
        while(messagesToSend.size() > 0)
        {
            writeMessage(messagesToSend.remove());
        }
    }

    private void handleRecievedMessage(String message)
    {
        Map map = gson.fromJson(message, Map.class);
        String command = (String) map.get(CommandConstants.COMMAND);
        if(command == null)
        {
            return;
        }
        switch (command)
        {
            case Ping.PING_COMMAND:
                break;
            case ReadSensorData.READ_SENSOR_DATA_COMMAND:
                readSensorData(gson.fromJson(message, ReadSensorData.class));
                break;
            case ResetOrigin.RESET_ORIGIN_COMMAND:
                break;
            case ReadDistanceFromOrigin.READ_DISTANCE_FROM_ORIGIN_COMMAND:
                readDistanceFromOrigin(gson.fromJson(message, ReadDistanceFromOrigin.class));
                break;
        }
    }

    private void readSensorData(ReadSensorData message)
    {
        vrShoe1.setDeviceId(message.deviceId);
        vrShoe1.frontButtonPressed(message.frontButtonPressed);
        vrShoe1.rearButtonPressed(message.rearButtonPressed);
        vrShoe1.setForwardSpeed(message.forwardSpeed);
        vrShoe1.setSidewaySpeed(message.sidewaySpeed);

        for(ICommunicatorObserver observer : observers)
        {
            observer.sensorDataRead(vrShoe1);
        }
    }

    private void readDistanceFromOrigin(ReadDistanceFromOrigin message)
    {
        if(!message.reply)
        {
            return;
        }
        for(ICommunicatorObserver observer : observers)
        {
            observer.distanceFromOriginRead(message.deviceId, message.forwardDistance, message.sidewaysDistance);
        }
    }

    private void writeMessage(String message) throws IOException
    {
        if(message.charAt(message.length() - 1) != MESSAGE_TERMINATOR)
        {
            message += MESSAGE_TERMINATOR;
        }
        writeMessageImplementation(message);
        for(ICommunicatorObserver observer : observers)
        {
            observer.messageWritten(message);
        }
    }

    protected void storeReadMessage(String message)
    {
        recievedMessages.add(message);
        for(ICommunicatorObserver observer : observers)
        {
            observer.messageRead(message);
        }
    }

    public void ping()
    {
        String json = gson.toJson(new Ping());
        messagesToSend.add(json);
    }

    public void readSensorDataFromShoes()
    {
        String json = gson.toJson(new ReadSensorData());
        messagesToSend.add(json);
    }

    public void resetOrigin()
    {
        String json = gson.toJson(new ResetOrigin());
        messagesToSend.add(json);
    }

    public void readDistanceFromOrigin()
    {
        String json = gson.toJson(new ReadDistanceFromOrigin());
        messagesToSend.add(json);
    }

    public void setRpm(float forwardRpm, float sidewayRpm)
    {
        SetRpm command = new SetRpm();
        command.forwardRpm = forwardRpm;
        command.sidewayRpm = sidewayRpm;
        String json = gson.toJson(command);
        messagesToSend.add(json);
    }

    public VrShoe getVrShoe1()
    {
        return vrShoe1;
    }

    protected abstract void readMessagesIntoQueue() throws IOException;
    protected abstract void writeMessageImplementation(String message) throws IOException;
}
