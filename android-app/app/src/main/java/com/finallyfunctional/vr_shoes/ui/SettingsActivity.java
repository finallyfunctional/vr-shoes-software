package com.finallyfunctional.vr_shoes.ui;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Spinner;
import android.widget.TextView;

import com.finallyfunctional.vr_shoes.R;
import com.finallyfunctional.vr_shoes.VrShoe;
import com.finallyfunctional.vr_shoes.communication.CommunicationInitializer;
import com.finallyfunctional.vr_shoes.communication.Communicator;
import com.finallyfunctional.vr_shoes.communication.commands.ShoeSide;

import androidx.appcompat.app.AppCompatActivity;

public class SettingsActivity extends AppCompatActivity
{
    private VrShoeSettings vrShoe1Settings, vrShoe2Settings;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);
        Spinner vrShoe1SideSpinner = findViewById(R.id.settingsVrShoe1SideSpinner);
        Spinner vrShoe2SideSpinner = findViewById(R.id.settingsVrShoe2SideSpinner);
        TextView vrShoe1Header = findViewById(R.id.settingsVrShoe1Header);
        TextView vrShoe2Header = findViewById(R.id.settingsVrShoe2Header);
        Communicator communicator = CommunicationInitializer.getCommunicator();
        vrShoe1Settings = new VrShoeSettings(communicator.getVrShoe1(), communicator, this,
                vrShoe1SideSpinner, vrShoe1Header);
        vrShoe1Settings = new VrShoeSettings(communicator.getVrShoe2(), communicator, this,
                vrShoe2SideSpinner, vrShoe2Header);
    }

    public void backBtnClicked(View view)
    {
        finish();
    }

    private class VrShoeSettings
    {
        private VrShoe vrShoe;
        private Communicator communicator;

        public VrShoeSettings(VrShoe vrShoe, Communicator communicator, Activity activity,
                              Spinner spinner, TextView deviceIdHeader)
        {
            this.vrShoe = vrShoe;
            this.communicator = communicator;
            deviceIdHeader.setText(vrShoe.getDeviceId());
            ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(activity,
                    R.array.sides_array, android.R.layout.simple_spinner_item);
            adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
            spinner.setAdapter(adapter);
            switch(vrShoe.getSide())
            {
                case ShoeSide.LEFT_SIDE:
                    spinner.setSelection(2);
                    break;
                case ShoeSide.RIGHT_SIDE:
                    spinner.setSelection(1);
                    break;
                default:
                    spinner.setSelection(0);
            }
            spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener()
            {
                @Override
                public void onItemSelected(AdapterView<?> parent, View view, int position, long id)
                {
                    itemSelected((CharSequence)parent.getItemAtPosition(position));
                }

                @Override
                public void onNothingSelected(AdapterView<?> parent)
                {

                }
            });
        }

        private void itemSelected(CharSequence itemSelected)
        {
            if(itemSelected.equals("Right"))
            {
                communicator.setShoeSide(vrShoe, ShoeSide.RIGHT_SIDE);
            }
            else if(itemSelected.equals("Left"))
            {
                communicator.setShoeSide(vrShoe, ShoeSide.LEFT_SIDE);
            }
        }
    }
}
