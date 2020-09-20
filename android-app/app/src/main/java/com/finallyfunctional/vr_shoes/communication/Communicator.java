package com.finallyfunctional.vr_shoes.communication;

import android.util.Pair;

import com.finallyfunctional.vr_shoes.VrShoe;
import com.finallyfunctional.vr_shoes.communication.commands.CommandConstants;
import com.finallyfunctional.vr_shoes.communication.commands.OtherShoeId;
import com.finallyfunctional.vr_shoes.communication.commands.Ping;
import com.finallyfunctional.vr_shoes.communication.commands.DistanceFromOrigin;
import com.finallyfunctional.vr_shoes.communication.commands.SensorData;
import com.finallyfunctional.vr_shoes.communication.commands.ResetOrigin;
import com.finallyfunctional.vr_shoes.communication.commands.SetCommunicationMode;
import com.finallyfunctional.vr_shoes.communication.commands.SetRpm;
import com.google.gson.Gson;

import java.io.IOException;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Map;
import java.util.NoSuchElementException;
import java.util.PriorityQueue;
import java.util.Queue;

public abstract class Communicator
{
    private ArrayList<ICommunicatorObserver> observers;
    private Queue<String> recievedMessages;
    private Queue<Pair<VrShoe, String>> messagesToSend;
    private Gson gson;
    private boolean keepLoopAlive;
    private VrShoe vrShoe1, vrShoe2;

    public static char MESSAGE_TERMINATOR = '\n';
    public static final int MESSAGE_TERMINATOR_ASCII = 10;

    public Communicator(String deviceId1, String deviceId2)
    {
        gson = new Gson();
        observers = new ArrayList<>();
        recievedMessages = new LinkedList<>();
        messagesToSend = new LinkedList<>();
        vrShoe1 = new VrShoe();
        vrShoe1.setDeviceId(deviceId1);
        vrShoe2 = new VrShoe();
        vrShoe2.setDeviceId(deviceId2);
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
                    catch(NoSuchElementException ex)
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
        while(!recievedMessages.isEmpty())
        {
            handleRecievedMessage(recievedMessages.remove());
        }
        while(!messagesToSend.isEmpty())
        {
            Pair<VrShoe, String> pair = messagesToSend.remove();
            writeMessage(pair.first, pair.second);
        }
    }

    private void handleRecievedMessage(String message) throws IOException
    {
        Map map = gson.fromJson(message, Map.class);
        if(map == null)
        {
            return;
        }
        String command = (String) map.get(CommandConstants.COMMAND);
        String shoeId = (String) map.get(CommandConstants.SHOE_ID);
        if(command == null || shoeId == null)
        {
            return;
        }
        VrShoe thisVrShoe, otherVrShoe;
        if(shoeId.equals(vrShoe1.getDeviceId()))
        {
            thisVrShoe = vrShoe1;
            otherVrShoe = vrShoe2;
        }
        else if(shoeId.equals(vrShoe2.getDeviceId()))
        {
            thisVrShoe = vrShoe2;
            otherVrShoe = vrShoe1;
        }
        else
        {
            return;
        }
        switch (command)
        {
            case Ping.PING_COMMAND:
                break;
            case SensorData.SENSOR_DATA_COMMAND:
                readSensorData(message, thisVrShoe, otherVrShoe);
                break;
            case ResetOrigin.RESET_ORIGIN_COMMAND:
                break;
            case DistanceFromOrigin.DISTANCE_FROM_ORIGIN_COMMAND:
                readDistanceFromOrigin(thisVrShoe, gson.fromJson(message, DistanceFromOrigin.class));
                break;
        }
    }

    private void readSensorData(String message, VrShoe thisVrShoe, VrShoe otherVrShoe) throws IOException
    {
        SensorData messageObj = gson.fromJson(message, SensorData.class);
        thisVrShoe.frontButtonPressed(messageObj.frontButtonPressed);
        thisVrShoe.rearButtonPressed(messageObj.rearButtonPressed);
        thisVrShoe.setForwardSpeed(messageObj.forwardSpeed);
        thisVrShoe.setSidewaySpeed(messageObj.sidewaySpeed);

        if(forwardSensorDataToOtherShoe())
        {
            writeMessage(otherVrShoe, message);
        }

        for(ICommunicatorObserver observer : observers)
        {
            observer.sensorDataRead(thisVrShoe);
        }
    }

    private void readDistanceFromOrigin(VrShoe vrShoe, DistanceFromOrigin message)
    {
        if(!message.reply)
        {
            return;
        }
        for(ICommunicatorObserver observer : observers)
        {
            observer.distanceFromOriginRead(vrShoe, message.forwardDistance, message.sidewaysDistance);
        }
    }

    private void writeMessage(VrShoe vrShoe, String message) throws IOException
    {
        if(message.charAt(message.length() - 1) != MESSAGE_TERMINATOR)
        {
            message += MESSAGE_TERMINATOR;
        }
        writeMessageImplementation(vrShoe, message);
        for(ICommunicatorObserver observer : observers)
        {
            observer.messageWritten(vrShoe, message);
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

    public void ping(VrShoe vrShoe)
    {
        Ping pingCommand = new Ping();
        pingCommand.to = vrShoe.getDeviceId();
        String json = gson.toJson(pingCommand);
        messagesToSend.add(new Pair<>(vrShoe, json));
    }

    public void readSensorDataFromShoes()
    {
        readSensorDataFromShoe(vrShoe1);
        readSensorDataFromShoe(vrShoe2);
    }

    private void readSensorDataFromShoe(VrShoe vrShoe)
    {
        SensorData command = new SensorData();
        command.deviceId = vrShoe.getDeviceId();
        command.get = true;
        String json = gson.toJson(command);
        messagesToSend.add(new Pair<>(vrShoe, json));
    }

    public void resetOrigin(VrShoe vrShoe)
    {
        ResetOrigin command = new ResetOrigin();
        command.to = vrShoe.getDeviceId();
        String json = gson.toJson(command);
        messagesToSend.add(new Pair<>(vrShoe, json));
    }

    public void readDistanceFromOrigin(VrShoe vrShoe)
    {
        DistanceFromOrigin command = new DistanceFromOrigin();
        command.to = vrShoe.getDeviceId();
        String json = gson.toJson(command);
        messagesToSend.add(new Pair<>(vrShoe, json));
    }

    public void setRpm(float forwardRpm, float sidewayRpm, VrShoe vrShoe)
    {
        SetRpm command = new SetRpm();
        command.forwardRpm = forwardRpm;
        command.sidewayRpm = sidewayRpm;
        command.to = vrShoe.getDeviceId();
        String json = gson.toJson(command);
        messagesToSend.add(new Pair<>(vrShoe, json));
    }

    public void setCommunicationMode(VrShoe vrShoe, SetCommunicationMode command)
    {
        String json = gson.toJson(command);
        messagesToSend.add(new Pair<>(vrShoe, json));
    }

    public void sendOtherShoeId(VrShoe thisShoe, VrShoe otherShoe)
    {
        OtherShoeId command = new OtherShoeId();
        command.otherShoeId = otherShoe.getDeviceId();
        messagesToSend.add(new Pair<>(thisShoe, gson.toJson(command)));
    }

    public VrShoe getVrShoe1()
    {
        return vrShoe1;
    }

    public VrShoe getVrShoe2() {return vrShoe2;}

    protected abstract void readMessagesIntoQueue() throws IOException;
    protected abstract void writeMessageImplementation(VrShoe vrShoe, String message) throws IOException;
    protected abstract boolean forwardSensorDataToOtherShoe();
}
