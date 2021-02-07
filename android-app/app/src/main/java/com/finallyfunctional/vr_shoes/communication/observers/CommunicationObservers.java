package com.finallyfunctional.vr_shoes.communication.observers;

import java.util.ArrayList;

public class CommunicationObservers
{
    private ArrayList<ICommunicatorObserver> communicatorObservers;
    private ArrayList<ISensorDataObserver> sensorDataObservers;
    private ArrayList<IShoeConfigurationsObserver> shoeConfigurationsObservers;

    public CommunicationObservers()
    {
        communicatorObservers = new ArrayList<>();
        sensorDataObservers  = new ArrayList<>();
        shoeConfigurationsObservers = new ArrayList<>();
    }

    public void addCommunicationObserver(ICommunicatorObserver observer)
    {
        communicatorObservers.add(observer);
    }

    public void removeCommunicationObserver(ICommunicatorObserver observer)
    {
        communicatorObservers.remove(observer);
    }

    public void addSensorDataObserver(ISensorDataObserver observer)
    {
        sensorDataObservers.add(observer);
    }

    public void removeSensorDataObserver(ISensorDataObserver observer)
    {
        sensorDataObservers.remove(observer);
    }

    public void addShoeConfigurationObserver(IShoeConfigurationsObserver observer)
    {
        shoeConfigurationsObservers.add(observer);
    }

    public void removeShoeConfigurationObserver(IShoeConfigurationsObserver observer)
    {
        shoeConfigurationsObservers.remove(observer);
    }

    public ArrayList<ICommunicatorObserver> getCommunicationObservers()
    {
        return communicatorObservers;
    }

    public ArrayList<ISensorDataObserver> getSensorDataObservers()
    {
        return sensorDataObservers;
    }

    public ArrayList<IShoeConfigurationsObserver> getShoeConfigurationObservers()
    {
        return shoeConfigurationsObservers;
    }
}
