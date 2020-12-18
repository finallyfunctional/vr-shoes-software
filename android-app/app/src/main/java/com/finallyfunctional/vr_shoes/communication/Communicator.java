package com.finallyfunctional.vr_shoes.communication;

import android.util.Pair;

import com.finallyfunctional.vr_shoes.VrShoe;
import com.finallyfunctional.vr_shoes.communication.commands.ButtonValues;
import com.finallyfunctional.vr_shoes.communication.commands.CommandConstants;
import com.finallyfunctional.vr_shoes.communication.commands.ConfigureButtons;
import com.finallyfunctional.vr_shoes.communication.commands.DutyCycleBoost;
import com.finallyfunctional.vr_shoes.communication.commands.ExtraSensorData;
import com.finallyfunctional.vr_shoes.communication.commands.OtherShoeId;
import com.finallyfunctional.vr_shoes.communication.commands.Ping;
import com.finallyfunctional.vr_shoes.communication.commands.PowerStatistics;
import com.finallyfunctional.vr_shoes.communication.commands.ResetOrigin;
import com.finallyfunctional.vr_shoes.communication.commands.SensorData;
import com.finallyfunctional.vr_shoes.communication.commands.SetCommunicationMode;
import com.finallyfunctional.vr_shoes.communication.commands.SetRpm;
import com.finallyfunctional.vr_shoes.communication.commands.ShoeSide;
import com.finallyfunctional.vr_shoes.communication.commands.SpeedMultiplier;
import com.finallyfunctional.vr_shoes.communication.commands.StartAlgorithm;
import com.finallyfunctional.vr_shoes.communication.commands.StopAlgorithm;
import com.finallyfunctional.vr_shoes.communication.commands.TunePidLoop;
import com.google.gson.Gson;

import java.io.IOException;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Map;
import java.util.NoSuchElementException;
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
                case ShoeSide.SHOE_SIDE_COMMAND:
                    readShoeSide(thisVrShoe, gson.fromJson(message, ShoeSide.class));
                    break;
                case ExtraSensorData.EXTRA_SENSOR_DATA_COMMAND:
                    readExtraSensorData(gson.fromJson(message, ExtraSensorData.class), thisVrShoe);
                    break;
                case PowerStatistics.POWER_STATISTICS_COMMAND:
                    readPowerStatistics(gson.fromJson(message, PowerStatistics.class), thisVrShoe);
                    break;
                case ButtonValues.BUTTON_VALUES_COMMAND:
                    readButtonValues(gson.fromJson(message, ButtonValues.class), thisVrShoe);
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
        thisVrShoe.setSidewaySpeed(message.ss);
        thisVrShoe.setForwardDistanceFromOrigin(message.fd);
        thisVrShoe.setSidewayDistanceFromOrigin(message.sid);

        if(forwardSensorDataToOtherShoe() && message.d != null && message.d.equals(otherVrShoe.getDeviceId()))
        {
            message.r = false;
            writeMessage(otherVrShoe, gson.toJson(message));
        }

        for(ICommunicatorObserver observer : observers)
        {
            observer.sensorDataRead(thisVrShoe);
        }
    }

    private void readExtraSensorData(ExtraSensorData message, VrShoe vrShoe)
    {
        if(!message.r)
        {
            return;
        }
        vrShoe.setForwardDesiredSpeed(message.fds);
        vrShoe.setSidewayDesiredSpeed(message.sds);
        vrShoe.setForwardDutyCycle(message.fdc);
        vrShoe.setSidewayDutyCycle(message.sdc);
    }

    private void readPowerStatistics(PowerStatistics message, VrShoe vrShoe)
    {
        if(!message.r)
        {
            return;
        }
        vrShoe.setSidewayPeakCurrent(message.spk);
        vrShoe.setSidewayCurrentNow(message.scn);
        vrShoe.setSidewayAverageCurrent(message.sac);
        vrShoe.setSidewayAmpHours(message.sah);
        vrShoe.setSidewayAmpCharged(message.sahc);

        vrShoe.setForwardPeakCurrent(message.fpc);
        vrShoe.setForwardCurrentNow(message.fcn);
        vrShoe.setForwardAverageCurrent(message.fac);
        vrShoe.setForwardAmpHours(message.fah);
        vrShoe.setForwardAmpCharged(message.fahc);
    }

    private void readShoeSide(VrShoe shoe, ShoeSide shoeSide)
    {
        shoe.setSide(shoeSide.si);
    }

    private void readButtonValues(ButtonValues values, VrShoe shoe)
    {
        for(ICommunicatorObserver observer : observers)
        {
            observer.buttonValuesRead(shoe, values.fbpv, values.rbpv, values.bmd);
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

    public void resetOrigin(VrShoe vrShoe)
    {
        ResetOrigin command = new ResetOrigin();
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

    public void sendOtherShoeId(VrShoe thisShoe, VrShoe otherShoe)
    {
        OtherShoeId command = new OtherShoeId();
        command.osi = otherShoe.getDeviceId();
        messagesToSend.add(new Pair<>(thisShoe, gson.toJson(command)));
    }

    public void setShoeSide(VrShoe shoe, int side)
    {
        if(side != ShoeSide.LEFT_SIDE && side != ShoeSide.RIGHT_SIDE)
        {
            throw new IllegalArgumentException();
        }
        shoe.setSide(side);
        ShoeSide command = new ShoeSide();
        command.si = side;
        command.d = shoe.getDeviceId();
        messagesToSend.add(new Pair<>(shoe, gson.toJson(command)));
    }

    public void getShoeSide(VrShoe vrShoe)
    {
        ShoeSide command = new ShoeSide();
        command.g = true;
        command.d = vrShoe.getDeviceId();
        messagesToSend.add(new Pair<>(vrShoe, gson.toJson(command)));
    }

    public void startAlgorithm(VrShoe vrShoe)
    {
        StartAlgorithm command = new StartAlgorithm();
        command.d = vrShoe.getDeviceId();
        messagesToSend.add(new Pair<>(vrShoe, gson.toJson(command)));
    }

    public void stopAlgorithm(VrShoe vrShoe)
    {
        StopAlgorithm command = new StopAlgorithm();
        command.d = vrShoe.getDeviceId();
        messagesToSend.add(new Pair<>(vrShoe, gson.toJson(command)));
    }

    public void getExtraSensorData(VrShoe vrShoe)
    {
        ExtraSensorData command = new ExtraSensorData();
        command.d = vrShoe.getDeviceId();
        messagesToSend.add(new Pair<>(vrShoe, gson.toJson(command)));
    }

    public void setDutyCycleBoost(VrShoe vrShoe, float boost)
    {
        DutyCycleBoost command = new DutyCycleBoost();
        command.d = vrShoe.getDeviceId();
        command.dcb = boost;
        messagesToSend.add(new Pair<>(vrShoe, gson.toJson(command)));
        vrShoe.setDutyCycleBoost(boost);
    }

    public void tunePidLoop(VrShoe vrShoe, float kp, float ki, float kd)
    {
        TunePidLoop command = new TunePidLoop();
        command.kp = kp;
        command.ki = ki;
        command.kd = kd;
        messagesToSend.add(new Pair<>(vrShoe, gson.toJson(command)));
        vrShoe.setPidParameters(kp, ki, kd);
    }

    public void setSpeedMultiplier(VrShoe vrShoe, float multiplier)
    {
        SpeedMultiplier command = new SpeedMultiplier();
        command.spm = multiplier;
        messagesToSend.add(new Pair<>(vrShoe, gson.toJson(command)));
        vrShoe.setSpeedMultiplier(multiplier);
    }

    public void getPowerStatistics(VrShoe vrShoe)
    {
        PowerStatistics command = new PowerStatistics();
        command.d = vrShoe.getDeviceId();
        messagesToSend.add(new Pair<>(vrShoe, gson.toJson(command)));
    }

    public void configureButtons(
            VrShoe vrShoe,
            int buttonMaxDifference)
    {
        ConfigureButtons command = new ConfigureButtons();
        command.bmd = buttonMaxDifference;
        messagesToSend.add(new Pair<>(vrShoe, gson.toJson(command)));
    }

    public void getButtonValues(VrShoe vrShoe)
    {
        ButtonValues command = new ButtonValues();
        command.id = vrShoe.getDeviceId();
        messagesToSend.add(new Pair<>(vrShoe, gson.toJson(command)));
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
