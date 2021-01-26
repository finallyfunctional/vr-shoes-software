package com.finallyfunctional.vr_shoes.ui;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import com.finallyfunctional.vr_shoes.R;

import androidx.appcompat.app.AppCompatActivity;

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

    public void buttonsTestBtnClick(View view)
    {
        Intent intent = new Intent(this, ConfigureButtonsActivity.class);
        startActivity(intent);
    }

    public void movementBtnClick(View view)
    {
        Intent intent = new Intent(this, MovementDiagnosticsActivity.class);
        startActivity(intent);
    }

    public void manualControlBtnClick(View view)
    {
        Intent intent = new Intent(this, ManualControlActivity.class);
        startActivity(intent);
    }

    public void breakInShoesBtnClick(View view)
    {
        Intent intent = new Intent(this, BreakShoesInActivity.class);
        startActivity(intent);
    }

    public void backBtnClicked(View view)
    {
        finish();
    }
}
