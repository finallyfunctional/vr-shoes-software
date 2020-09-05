package com.finallyfunctional.vr_shoes.ui;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.finallyfunctional.vr_shoes.R;
import com.finallyfunctional.vr_shoes.communication.CommunicationInitializer;
import com.finallyfunctional.vr_shoes.communication.Communicator;
import com.finallyfunctional.vr_shoes.logging.monitor.IMonitorLogObserver;
import com.finallyfunctional.vr_shoes.logging.monitor.MonitorLog;
import com.finallyfunctional.vr_shoes.logging.monitor.VrShoesMonitor;

import java.io.IOException;

import androidx.appcompat.app.AppCompatActivity;

public class MonitorActivity extends AppCompatActivity implements IMonitorLogObserver
{
    private TextView logsView;
    private VrShoesMonitor monitor;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_monitor);
        logsView = findViewById(R.id.monitorTextView);
        monitor = VrShoesMonitor.getLogger();
        monitor.addObserver(this);
        updateTextView();
    }

    private void updateTextView()
    {
        StringBuilder logs = new StringBuilder();
        for(MonitorLog log : monitor.getLogs())
        {
            logs.append(log.message);
            if(log.message.charAt(log.message.length() - 1) != '\n')
            {
                logs.append('\n');
            }
        }
        logsView.setText(logs.toString());
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
        monitor.removeObserver(this);
    }

    @Override
    public void messageLogged(MonitorLog log)
    {
        this.runOnUiThread(new Runnable()
        {
            public void run()
            {
                updateTextView();
            }
        });
    }

    public void backBtnClicked(View view)
    {
        Intent intent = new Intent(this, DiagnosticsActivity.class);
        startActivity(intent);
    }

    public void pingBtnClicked(View view)
    {
        Communicator communicator = CommunicationInitializer.getCommunicator();
        try
        {
            communicator.ping();
        }
        catch(IOException ex)
        {
            ex.printStackTrace();
            //TODO handle error
            Toast.makeText(getApplicationContext(), getString(R.string.unknownError), Toast.LENGTH_LONG).show();
        }
    }
}
