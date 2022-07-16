package com.finallyfunctional.vr_shoes.ui;

import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;

import com.finallyfunctional.vr_shoes.R;
import com.finallyfunctional.vr_shoes.StoredSettings;

import java.util.ArrayList;
import java.util.Set;

public class PairVrShoesActivity extends AppCompatActivity
{
    private BluetoothAdapter btAdapter;
    private ListView list;
    private ArrayAdapter<String> listAdapter;
    private Button okBtn;
    private StoredSettings settings;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_pair_vr_shoes);
        settings = new StoredSettings(this);
        okBtn = findViewById(R.id.pairVrShoesOkBtn);
        okBtn.setEnabled(false);
        btAdapter = BluetoothAdapter.getDefaultAdapter();
    }

    @Override
    public void onResume()
    {
        super.onResume();
        checkForPairedVrShoes();
        initializeList();
    }

    private void checkForPairedVrShoes()
    {
        if(getVrShoeNames().size() > 1)
        {
            return;
        }
        DialogUtility.showOkayDialog(this, getString(R.string.no_vr_shoes_paired), new Runnable()
        {
            @Override
            public void run()
            {
                Intent intentOpenBluetoothSettings = new Intent();
                intentOpenBluetoothSettings.setAction(android.provider.Settings.ACTION_BLUETOOTH_SETTINGS);
                startActivity(intentOpenBluetoothSettings);
            }
        });
    }

    private void initializeList()
    {
        list = findViewById(R.id.pairVrShoesList);
        list.setChoiceMode(ListView.CHOICE_MODE_MULTIPLE);
        listAdapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_multiple_choice,  getVrShoeNames());
        list.setAdapter(listAdapter);
        list.setOnItemClickListener(new AdapterView.OnItemClickListener()
        {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id)
            {
                listItemClicked(parent, view, position, id);
            }
        });
    }

    private ArrayList<String> getVrShoeNames()
    {
        Set<BluetoothDevice> pairedDevices = btAdapter.getBondedDevices();
        ArrayList<String> deviceNames = new ArrayList<>();
        for(BluetoothDevice device : pairedDevices)
        {
            if(device.getName().contains(StoredSettings.VR_SHOE_DEVICE_ID_PREFIX))
            {
                deviceNames.add(device.getName());
            }
        }
        return deviceNames;
    }

    private void listItemClicked(AdapterView<?> parent, View view, int position, long id)
    {
        okBtn.setEnabled(list.getCheckedItemCount() == 2);
    }

    public void refreshBtnClicked(View view)
    {
        listAdapter.clear();
        listAdapter.addAll(getVrShoeNames());
    }

    public void okBtnClicked(View view)
    {
        String deviceId1 = "";
        String deviceId2 = "";
        for(int i = 0; i < list.getCheckedItemCount(); i++)
        {
            if(deviceId1.equals(""))
            {
                deviceId1 = (String)list.getItemAtPosition(i);
            }
            else
            {
                deviceId2 = (String)list.getItemAtPosition(i);
            }

        }
        settings.storePairedVrShoe1(deviceId1);
        settings.storePairedVrShoe2(deviceId2);
        finish();
    }
}
