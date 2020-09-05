package com.finallyfunctional.vr_shoes.ui;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import com.finallyfunctional.vr_shoes.R;

public class DiagnosticsActivity extends AppCompatActivity
{

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_diagnostics);
    }

    public void showLogsBtnClick(View view)
    {
        Intent intent = new Intent(this, MonitorActivity.class);
        startActivity(intent);
    }
}
