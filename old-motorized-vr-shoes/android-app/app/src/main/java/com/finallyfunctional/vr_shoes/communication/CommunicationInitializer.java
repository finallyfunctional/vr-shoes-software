package com.finallyfunctional.vr_shoes.communication;

import com.finallyfunctional.vr_shoes.StoredSettings;
import com.finallyfunctional.vr_shoes.VrShoe;
import com.finallyfunctional.vr_shoes.communication.commands.ShoeConfiguration;
import com.finallyfunctional.vr_shoes.communication.exceptions.CommunicationNotEnabledException;
import com.finallyfunctional.vr_shoes.communication.exceptions.CommunicationNotSupportedException;
import com.finallyfunctional.vr_shoes.communication.exceptions.ConfigurationWithOtherActivityNeededException;
import com.finallyfunctional.vr_shoes.communication.observers.IShoeConfigurationsObserver;
import com.finallyfunctional.vr_shoes.logging.CommunicatorLogger;
import com.finallyfunctional.vr_shoes.logging.monitor.VrShoesAggregateLogger;
import com.finallyfunctional.vr_shoes.ui.PairVrShoesActivity;

import java.io.IOException;

public abstract class CommunicationInitializer implements IShoeConfigurationsObserver
{
    protected static Communicator communicator;
    protected StoredSettings settings;
    protected Runnable callback;

    protected CommunicationInitializer(StoredSettings settings)
    {
        this.settings = settings;
    }

    protected abstract void setup(String deviceId1, String deviceId2) throws
            IOException,
            CommunicationNotSupportedException,
            CommunicationNotEnabledException,
            ConfigurationWithOtherActivityNeededException;

    public void initialize(Runnable callbackOnSuccess)
            throws
            IOException,
            CommunicationNotSupportedException,
            CommunicationNotEnabledException,
            ConfigurationWithOtherActivityNeededException
    {
        if(communicator != null)
        {
            callback.run();
            return; //already initialized
        }
        this.callback = callbackOnSuccess;
        String deviceId1 = settings.getPairedVrShoe1();
        String deviceId2 = settings.getPairedVrShoe2();
        initializeCommunicator(deviceId1, deviceId2);
    }

    private void initializeCommunicator(String shoe1Id, String shoe2Id)
            throws ConfigurationWithOtherActivityNeededException,
            CommunicationNotEnabledException,
            IOException,
            CommunicationNotSupportedException
    {
        if(shoe1Id == null || shoe1Id.equals("") || shoe2Id == null || shoe2Id.equals(""))
        {
            throw new ConfigurationWithOtherActivityNeededException(PairVrShoesActivity.class);
        }
        setup(shoe1Id, shoe2Id);
        communicator.getObservers().addCommunicationObserver(new CommunicatorLogger(VrShoesAggregateLogger.getLogger()));
        communicator.getObservers().addShoeConfigurationObserver(this);
        communicator.start();
        initializeShoes();
    }

    private void initializeShoes()
    {
        VrShoe vrShoe1 = communicator.getVrShoe1();
        VrShoe vrShoe2 = communicator.getVrShoe2();
        communicator.getShoeConfigurations(vrShoe1);
        communicator.getShoeConfigurations(vrShoe2);
    }

    public void shoeConfigurationsRead(ShoeConfiguration message, VrShoe vrShoe)
    {
        if(message.osi == null || message.osi.trim().equals("") ||
           message.osi.equals(vrShoe.getDeviceId()))
        {
            if(vrShoe.getDeviceId().equals(communicator.getVrShoe1().getDeviceId()))
            {
                message.osi = communicator.getVrShoe2().getDeviceId();
            }
            else
            {
                message.osi = communicator.getVrShoe1().getDeviceId();
            }
            communicator.configureShoe(vrShoe, message);
            communicator.getShoeConfigurations(vrShoe);
        }
        if(communicator.getVrShoe1().getOtherShoeDeviceId() != null &&
           communicator.getVrShoe2().getOtherShoeDeviceId() != null &&
           !communicator.getVrShoe1().getOtherShoeDeviceId().equals("") &&
           !communicator.getVrShoe2().getOtherShoeDeviceId().equals(""))
        {
            callback.run();
            communicator.getObservers().removeShoeConfigurationObserver(this);
        }
    }

    public static Communicator getCommunicator()
    {
        return communicator;
    }
}
