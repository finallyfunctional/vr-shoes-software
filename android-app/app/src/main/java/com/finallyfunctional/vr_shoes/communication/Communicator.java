package com.finallyfunctional.vr_shoes.communication;

import android.util.Pair;

import com.finallyfunctional.vr_shoes.VrShoe;
import com.finallyfunctional.vr_shoes.communication.commands.CommandConstants;
import com.finallyfunctional.vr_shoes.communication.commands.Ping;
import com.finallyfunctional.vr_shoes.communication.commands.PowerStatistics;
import com.finallyfunctional.vr_shoes.communication.commands.ResetDistanceTracker;
import com.finallyfunctional.vr_shoes.communication.commands.SensorData;
import com.finallyfunctional.vr_shoes.communication.commands.SetCommunicationMode;
import com.finallyfunctional.vr_shoes.communication.commands.SetRpm;
import com.finallyfunctional.vr_shoes.communication.commands.ShoeConfiguration;
import com.finallyfunctional.vr_shoes.communication.commands.StartNegatingMotion;
import com.finallyfunctional.vr_shoes.communication.commands.StopNegatingMotion;
import com.finallyfunctional.vr_shoes.communication.commands.TopSensorValues;
import com.finallyfunctional.vr_shoes.communication.observers.CommunicationObservers;
import com.finallyfunctional.vr_shoes.communication.observers.ICommunicatorObserver;
import com.finallyfunctional.vr_shoes.communication.observers.ISensorDataObserver;
import com.finallyfunctional.vr_shoes.communication.observers.IShoeConfigurationsObserver;
import com.google.gson.Gson;

import java.io.IOException;
import java.util.LinkedList;
import java.util.Map;
import java.util.NoSuchElementException;
import java.util.Queue;

public abstract class Communicator
{
    private Queue<String> recievedMessages;
    private Queue<Pair<VrShoe, String>> messagesToSend;
    private Gson gson;
    private boolean keepLoopAlive;
    private VrShoe vrShoe1, vrShoe2;

    protected CommunicationObservers observers;

    public static char MESSAGE_TERMINATOR = '\n';
    public static final int MESSAGE_TERMINATOR_ASCII = 10;

    public Communicator(String deviceId1, String deviceId2)
    {
        gson = new Gson();
        observers = new CommunicationObservers();
        recievedMessages = new LinkedList<>();
        messagesToSend = new LinkedList<>();
        vrShoe1 = new VrShoe();
        vrShoe1.setDeviceId(deviceId1);
        vrShoe2 = new VrShoe();
        vrShoe2.setDeviceId(deviceId2);
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
            handleReceivedMessage(recievedMessages.remove());
        }
        while(!messagesToSend.isEmpty())
        {
            Pair<VrShoe, String> pair = messagesToSend.remove();
            writeMessage(pair.first, pair.second);
        }
    }

    private void handleReceivedMessage(String message) throws IOException
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

        try
        {
            switch (command)
            {
                case SensorData.SENSOR_DATA_COMMAND:
                    readSensorData(gson.fromJson(message, SensorData.class), thisVrShoe, otherVrShoe);
                    break;
                case PowerStatistics.POWER_STATISTICS_COMMAND:
                    readPowerStatistics(gson.fromJson(message, PowerStatistics.class), thisVrShoe);
                    break;
                case ShoeConfiguration.SHOE_CONFIGURATION_COMMAND:
                    readShoeConfiguration(gson.fromJson(message, ShoeConfiguration.class), thisVrShoe);
            }
        }
        catch(IllegalStateException ex)
        {
            ex.printStackTrace();
        }
    }

    private void readSensorData(SensorData message, VrShoe thisVrShoe, VrShoe otherVrShoe) throws IOException
    {
        thisVrShoe.frontButtonPressed(message.fb);
        thisVrShoe.rearButtonPressed(message.rb);
        thisVrShoe.setForwardSpeed(message.fs);
        thisVrShoe.setSidewaysSpeed(message.ss);
        thisVrShoe.setForwardDistance(message.fd);
        thisVrShoe.setSidewaysDistance(message.sid);

        if(forwardSensorDataToOtherShoe() && message.d != null && message.d.equals(otherVrShoe.getDeviceId()))
        {
            message.r = false;
            writeMessage(otherVrShoe, gson.toJson(message));
        }

        for(ISensorDataObserver observer : observers.getSensorDataObservers())
        {
            observer.sensorDataRead(thisVrShoe);
        }
    }

    private void readShoeConfiguration(ShoeConfiguration message, VrShoe vrShoe)
    {
        vrShoe.setOtherShoeDeviceId(message.osi);
        vrShoe.setSide(message.si);
        vrShoe.setDutyCycleBoost(message.dcb);
        vrShoe.setSpeedMultiplier(message.spm);

        for(IShoeConfigurationsObserver observer : observers.getShoeConfigurationObservers())
        {
            observer.shoeConfigurationsRead(message, vrShoe);
        }
    }

    private void readPowerStatistics(PowerStatistics message, VrShoe vrShoe)
    {
        if(!message.r)
        {
            return;
        }
        vrShoe.setSidewaysPeakCurrent(message.spk);
        vrShoe.setSidewaysCurrentNow(message.scn);
        vrShoe.setSidewaysAverageCurrent(message.sac);
        vrShoe.setSidewaysAmpHours(message.sah);
        vrShoe.setSidewaysAmpCharged(message.sahc);

        vrShoe.setForwardPeakCurrent(message.fpc);
        vrShoe.setForwardCurrentNow(message.fcn);
        vrShoe.setForwardAverageCurrent(message.fac);
        vrShoe.setForwardAmpHours(message.fah);
        vrShoe.setForwardAmpCharged(message.fahc);
    }

    private void writeMessage(VrShoe vrShoe, String message) throws IOException
    {
        if(message.charAt(message.length() - 1) != MESSAGE_TERMINATOR)
        {
            message += MESSAGE_TERMINATOR;
        }
        writeMessageImplementation(vrShoe, message);
        for(ICommunicatorObserver observer : observers.getCommunicationObservers())
        {
            observer.messageWritten(vrShoe, message);
        }
    }

    protected void storeReadMessage(String message)
    {
        recievedMessages.add(message);
        for(ICommunicatorObserver observer : observers.getCommunicationObservers())
        {
            observer.messageRead(message);
        }
    }

    public void ping(VrShoe vrShoe)
    {
        Ping pingCommand = new Ping();
        pingCommand.d = vrShoe.getDeviceId();
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
        command.id = vrShoe.getDeviceId();
        command.g = true;
        command.d = vrShoe.getDeviceId();
        String json = gson.toJson(command);
        messagesToSend.add(new Pair<>(vrShoe, json));
    }

    public void resetDistance(VrShoe vrShoe)
    {
        ResetDistanceTracker command = new ResetDistanceTracker();
        command.d = vrShoe.getDeviceId();
        String json = gson.toJson(command);
        messagesToSend.add(new Pair<>(vrShoe, json));
    }

    public void setRpm(float forwardRpm, float sidewayRpm, VrShoe vrShoe)
    {
        SetRpm command = new SetRpm();
        command.fr = forwardRpm;
        command.sr = sidewayRpm;
        command.d = vrShoe.getDeviceId();
        String json = gson.toJson(command);
        messagesToSend.add(new Pair<>(vrShoe, json));
    }

    public void setCommunicationMode(VrShoe vrShoe, SetCommunicationMode command)
    {
        String json = gson.toJson(command);
        messagesToSend.add(new Pair<>(vrShoe, json));
    }

    public void configureShoe(VrShoe vrShoe, ShoeConfiguration command)
    {
        command.id = vrShoe.getDeviceId();
        command.d = vrShoe.getDeviceId();
        command.g = false;
        command.r = false;
        String json = gson.toJson(command);
        messagesToSend.add(new Pair<>(vrShoe, json));
    }

    public void getShoeConfigurations(VrShoe vrShoe)
    {
        ShoeConfiguration command = new ShoeConfiguration();
        command.id = vrShoe.getDeviceId();
        command.d = vrShoe.getDeviceId();
        command.g = true;
        command.r = false;
        String json = gson.toJson(command);
        messagesToSend.add(new Pair<>(vrShoe, json));
    }

    public void startNegatingMotion(VrShoe vrShoe)
    {
        StartNegatingMotion command = new StartNegatingMotion();
        command.d = vrShoe.getDeviceId();
        messagesToSend.add(new Pair<>(vrShoe, gson.toJson(command)));
    }

    public void stopNegatingMotion(VrShoe vrShoe)
    {
        StopNegatingMotion command = new StopNegatingMotion();
        command.d = vrShoe.getDeviceId();
        messagesToSend.add(new Pair<>(vrShoe, gson.toJson(command)));
    }

    public void getPowerStatistics(VrShoe vrShoe)
    {
        PowerStatistics command = new PowerStatistics();
        command.d = vrShoe.getDeviceId();
        messagesToSend.add(new Pair<>(vrShoe, gson.toJson(command)));
    }

    public void getTopSensorValues(VrShoe vrShoe)
    {
        TopSensorValues command = new TopSensorValues();
        command.id = vrShoe.getDeviceId();
        messagesToSend.add(new Pair<>(vrShoe, gson.toJson(command)));
    }

    public VrShoe getVrShoe1()
    {
        return vrShoe1;
    }

    public VrShoe getVrShoe2() {return vrShoe2;}

    public CommunicationObservers getObservers()
    {
        return observers;
    }

    protected abstract void readMessagesIntoQueue() throws IOException;
    protected abstract void writeMessageImplementation(VrShoe vrShoe, String message) throws IOException;
    protected abstract boolean forwardSensorDataToOtherShoe();
}
