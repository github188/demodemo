package org.goku.client;


import java.util.List;

import org.goku.io.BaseStation;
import org.goku.io.GokuClient;
import org.goku.io.GokuResult;

import android.app.Activity;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

public class BTSActivity extends Activity {
	private static final String TAG = "bts";
	private GokuClient goku = null;
	private Handler handler = null;
	private BTSListAdapter adapter = null;
	private boolean inWaiting = false;
	private final int WAITING_DIALOG = 1;

	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.bts_list);
        
        TextView title = (TextView) findViewById(R.id.last_synced);
        title.setText("基站列表");
        
        ListView lv = (ListView) findViewById(R.id.task_list);
        //BTSListAdapter.
        goku = GokuClient.getInstance();
        adapter = new BTSListAdapter(goku.btsList);
        lv.setAdapter(adapter);        
    }
    
    @Override
    protected void onStart() {
        super.onStart();
        showDialog(WAITING_DIALOG);
        inWaiting = true;
        new LoadBTSTask().execute();
    }
   
    private class LoadBTSTask extends AsyncTask<GokuResult, String, GokuResult>{
		protected void onPostExecute(GokuResult r) {
        	if(r.status == 0){
        		//adapter.clear();
        		//for(BaseStation b: goku.btsList){
        		//	Log.d(TAG, "uuid:" + b.uuid);
        		//	adapter.add(b);
        		//}
        		adapter.notifyDataSetChanged();
        		showMessage("基站列表加载完成.");
        	}else {
        		showMessage(r.getMessage());
        	}			
		}

		@Override
		protected GokuResult doInBackground(GokuResult... params) {
			return goku.listBTS();
		}
    }
    
    protected Dialog onCreateDialog(int id) {
    	ProgressDialog dialog = null;
        switch(id) {
        case WAITING_DIALOG:
        	dialog = new ProgressDialog(this);
        	dialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
        	dialog.setMessage("加载中. 请稍等...");
        	dialog.setIndeterminate(false);
            return dialog;
        default:
            return null;
        }
    }    
    
	private void showMessage(final String message){
		runOnUiThread(
			new Runnable(){
				public void run(){
					if(inWaiting){
						dismissDialog(WAITING_DIALOG);
						inWaiting = false;
					}
					Context context = getApplicationContext();
					CharSequence text = message;
					int duration = Toast.LENGTH_LONG;
					Toast toast = Toast.makeText(context, text, duration);
					toast.show(); 
				}
			}
		);
	}    
    
    class ViewWrapper{
        View base;
        TextView name = null;
        TextView status = null;
        
        ViewWrapper(View base){
            this.base = base;
        }
        
        TextView getName(){
            if(name == null)
                name = (TextView)base.findViewById(R.id.name);
            return name;
        }

        TextView getStatus(){
            if(status == null)
            	status = (TextView)base.findViewById(R.id.status);
            return status;
        }        
    }
    
    class BTSListAdapter extends ArrayAdapter<BaseStation>{
    	BTSListAdapter(List<BaseStation> list){
            super(BTSActivity.this, R.layout.bts_item, list);
        }
    	
    	public View getView(int position, View convertView, ViewGroup parent) {
    		 View row = convertView;
    		 
    		 ViewWrapper wrapper = null;
    		 if(row == null){
                 LayoutInflater inflater = getLayoutInflater();
                 row = inflater.inflate(R.layout.bts_item, parent, false);
                 wrapper = new ViewWrapper(row);
                 row.setTag(wrapper);
             } else {
            	 wrapper = (ViewWrapper)row.getTag();
             }
    		 BaseStation bs = this.getItem(position);
    		 
    		 wrapper.getName().setText(bs.uuid + "_" + bs.name);
    		 wrapper.getStatus().setText(bs.status);
         
             return row;    		 
    	}    	
    }
}
