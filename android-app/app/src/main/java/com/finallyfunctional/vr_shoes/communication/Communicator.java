package com.finallyfunctional.vr_shoes.communication;

import com.finallyfunctional.vr_shoes.communication.commands.PingCommand;
import com.google.gson.Gson;

import java.io.IOException;
import java.util.ArrayList;
import java.util.PriorityQueue;
import java.util.Queue;

public abstract class Communicator
{
    private ArrayList<ICommunicatorObserver> observers;
    private Queue<String> recievedMessages;

    public Communicator()
    {
        observers = new ArrayList<>();
        recievedMessages = new PriorityQueue<>();
    }

    public void addObserver(ICommunicatorObserver observer)
    {
        observers.add(observer);
    }

    public static char MESSAGE_TERMINATOR = '\n';
    public static final int MESSAGE_TERMINATOR_ASCII = 10;

    private String readNextMessage()
    {
        return recievedMessages.size() == 0 ? "" : recievedMessages.remove();
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

    public void ping() throws IOException
    {
        String json = new Gson().toJson(new PingCommand());
        writeMessage(json);
    }

    public abstract void startReading();
    public abstract void stopReading();

    protected abstract void writeMessageImplementation(String message) throws IOException;
}
