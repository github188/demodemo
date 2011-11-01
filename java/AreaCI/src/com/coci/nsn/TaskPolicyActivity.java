package com.coci.nsn;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class TaskPolicyActivity extends Activity {
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        TextView textview = new TextView(this);
        textview.setText("This is the policy tab");
        setContentView(textview);
    }
}
