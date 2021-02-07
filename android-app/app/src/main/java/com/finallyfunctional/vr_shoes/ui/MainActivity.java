package com.finallyfunctional.vr_shoes.ui;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ProgressBar;
import android.widget.Toast;

import com.finallyfunctional.vr_shoes.R;
import com.finallyfunctional.vr_shoes.StoredSettings;
import com.finallyfunctional.vr_shoes.communication.CommunicationInitializer;
import com.finallyfunctional.vr_shoes.communication.bluetooth.BluetoothInitializer;
import com.finallyfunctional.vr_shoes.communication.exceptions.CommunicationNotEnabledException;
import com.finallyfunctional.vr_shoes.communication.exceptions.CommunicationNotSupportedException;
import com.finallyfunctional.vr_shoes.communication.exceptions.ConfigurationWithOtherActivityNeededException;
import com.finallyfunctional.vr_shoes.logging.monitor.VrShoesAggregateLogger;

import java.io.IOException;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity
{
    private CommunicationInitializer communicationInitializer;
    private ProgressBar progressBar;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        progressBar = findViewById(R.id.mainProgressBar);
        communicationInitializer = new BluetoothInitializer(new StoredSettings(this));
        VrShoesAggregateLogger.initialize();
    }

    @Override
    public void onResume()
    {
        super.onResume();
        ProgressBarUtility.showProgressBar(progressBar, this);
        new Runnable()
        {
            @Override
            public void run()
            {
                try
                {
                    communicationInitializer.initialize(new Runnable()
                    {
                        @Override
                        public void run()
                        {
                            MainActivity.this.runOnUiThread(new Runnable()
                            {
                                @Override
                                public void run()
                                {
                                    ProgressBarUtility.hideProgressBar(progressBar, MainActivity.this);
                                }
                            });
                        }
                    });
                }
                catch (IOException ex)
                {
                    ex.printStackTrace();
                    MainActivity.this.runOnUiThread(new Runnable()
                    {
                        @Override
                        public void run()
                        {
                            DialogUtility.showOkayDialog(MainActivity.this, getString(R.string.communication_error), new Runnable()
                            {
                                @Override
                                public void run()
                                {
                                    finish();
                                    System.exit(0);
                                }
                            });
                        }
                    });
                }
                catch (final CommunicationNotSupportedException ex)
                {
                    MainActivity.this.runOnUiThread(new Runnable()
                    {
                        @Override
                        public void run()
                        {
                            DialogUtility.showOkayDialog(MainActivity.this, getString(ex.dialogMessageId), new Runnable()
                            {
                                @Override
                                public void run()
                                {
                                    finish();
                                    System.exit(0);
                                }
                            });
                        }
                    });
                }
                catch(final CommunicationNotEnabledException ex)
                {
                    MainActivity.this.runOnUiThread(new Runnable()
                    {
                        @Override
                        public void run()
                        {
                            Intent enableBtIntent = new Intent(ex.requestToEnable);
                            startActivityForResult(enableBtIntent, 1);
                            Toast.makeText(getApplicationContext(), getString(ex.dialogMessageId), Toast.LENGTH_LONG).show();
                        }
                    });
                }
                catch(final ConfigurationWithOtherActivityNeededException ex)
                {
                    MainActivity.this.runOnUiThread(new Runnable()
                    {
                        @Override
                        public void run()
                        {
                            if(ex.showDialogFirst)
                            {
                                DialogUtility.showOkayDialog(MainActivity.this, getString(ex.dialogMessageId), new Runnable()
                                {
                                    @Override
                                    public void run()
                                    {
                                        Intent intent = new Intent(MainActivity.this, ex.otherActivity);
                                        startActivity(intent);
                                    }
                                });
                            }
                            else {
                                Intent intent = new Intent(MainActivity.this, ex.otherActivity);
                                startActivity(intent);
                            }
                        }
                    });
                }
            }
        }.run();
    }

    public void settingsBtnClicked(View view)
    {
        Intent intent = new Intent(MainActivity.this, SettingsActivity.class);
        startActivity(intent);
    }

    public void diagnosticsBtnClicked(View view)
    {
        Intent intent = new Intent(MainActivity.this, DiagnosticsActivity.class);
        startActivity(intent);
    }

    public void walkingTestBtnClick(View view)
    {
        Intent intent = new Intent(MainActivity.this, WalkingTestActivity.class);
        startActivity(intent);
    }

}
