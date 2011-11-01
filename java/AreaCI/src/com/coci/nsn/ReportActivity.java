package com.coci.nsn;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class ReportActivity extends Activity {
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        TextView textview = new TextView(this);
        textview.setText("This is the report tab");
        setContentView(textview);
    }
}
