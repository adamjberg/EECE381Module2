package com.example.ece381;

import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.widget.ImageButton;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;
import android.widget.Toast;

//import android.content.Intent;

public class play extends Activity {

	ImageButton back_button;
	TextView greetMsg;
	TextView textview;
	SeekBar seekbar;
	SeekBar seekbar2;
	private int seek;
	private Communication com = Communication.getInstance();
	private Timer play_timer;
	Handler handler;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
			 getWindow().requestFeature(Window.FEATURE_ACTION_BAR);
		 }
		setContentView(R.layout.play);
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
	        // Show the Up button in the action bar.
	        this.getActionBar().setDisplayHomeAsUpEnabled(true);
	    }
		addBarSsound();
		addBarSeek();
		handler = new Handler();
		greetMsg = (TextView) findViewById(R.id.textView1);
		
		Toast.makeText(getApplicationContext(), 
				update(), Toast.LENGTH_SHORT).show();
		
		SeekBarTimerTask seek_task = new SeekBarTimerTask();
		play_timer = new Timer();
		play_timer.schedule(seek_task, 100, 100);
	}

	public String update() {
		String msg = "No music is currently playing";
		if(com.getDB().getCurr_song_id() == 0)
			greetMsg.setText("No song has been selected");
		else {
			msg = "playing "+ com.getDB().getSongs()[com.getDB().getCurr_song_id()].getSongName();
			greetMsg.setText(msg);
		} return msg;

	}
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// TODO Auto-generated method stub
		switch(item.getItemId()){
		case android.R.id.home:
			finish();
		    // NavUtils.navigateUpFromSameTask(this);
			// onBackPressed();
		    return true;
		case R.id.songs:
			finish();
			Intent intentsong = new Intent(play.this, MainActivity.class);
		    startActivity(intentsong);   
		    return true;
		case R.id.menu_settings:
			Toast.makeText(play.this,
					item.getTitle(),Toast.LENGTH_LONG).show();
			return true;
		case R.id.playlists:
			finish();
			Intent intent = new Intent(play.this, PlaylistActivity.class);
		    startActivity(intent);   
		    return true;
		case R.id.playMenu:
			Toast.makeText(play.this,
				    "Already in play menu.",
				    Toast.LENGTH_LONG).show(); 
			return true;
		}
		return super.onOptionsItemSelected(item);
	} 

	@Override
	public void finish() {
		play_timer.cancel();
		super.finish();
	}
	public void onPlay(View view){
		if(com.getDB().getCurr_song_id() != 0)
			Command.syncPlay(com.getDB().getCurr_song_id(), com.getDB().getSongs()[com.getDB().getCurr_song_id()].getVolume(), com.getDB().getSongs()[com.getDB().getCurr_song_id()].getPos());
	}

	public void onPauseButton(View view) {
		if(com.getDB().getCurr_song_id() != 0)
			Command.syncPause(com.getDB().getCurr_song_id());
	}
	
	public void onNext(View view) {
		String msg;
		Database db = com.getDB();
		if(com.getDB().getCurr_playlist_id() == 0) {
			if(com.getDB().getCurr_song_id() >= com.getDB().getTotalSongs()) return;
			msg = "playing "+ com.getDB().getSongs()[com.getDB().getCurr_song_id()+1].getSongName();
			
			seekbar2.setMax(com.getDB().getSongs()[com.getDB().getCurr_song_id()+1].getSize());
			seekbar2.setProgress(com.getDB().getSongs()[com.getDB().getCurr_song_id()+1].getVolume());
		} else {
			if(db.getNextSongInList() == 0) return;
			msg = "playing "+ com.getDB().getSongs()[db.getNextSongInList()].getSongName();
			seekbar2.setMax(com.getDB().getSongs()[db.getNextSongInList()].getSize());
			seekbar2.setProgress(com.getDB().getSongs()[db.getNextSongInList()].getVolume());
		}
		greetMsg.setText(msg);
		Toast.makeText(getApplicationContext(), msg, Toast.LENGTH_SHORT).show();
		Command.syncNext(com.getDB().getCurr_song_id());
	}

	public void onPrev(View view) {
		Database db = com.getDB();
		String msg;
		if(com.getDB().getCurr_playlist_id() == 0) {
			if(com.getDB().getCurr_song_id() <=1) return;
			msg = "playing "+ com.getDB().getSongs()[com.getDB().getCurr_song_id()-1].getSongName();
			seekbar2.setMax(com.getDB().getSongs()[com.getDB().getCurr_song_id()-1].getSize());
			seekbar2.setProgress(com.getDB().getSongs()[com.getDB().getCurr_song_id()-1].getVolume());
		} else {
			if(db.getPrevSongInList() == 0) return;
			msg = "playing "+ com.getDB().getSongs()[db.getPrevSongInList()].getSongName();
			seekbar2.setMax(db.getSongs()[db.getPrevSongInList()].getSize());
			seekbar2.setProgress(db.getSongs()[db.getPrevSongInList()].getVolume());
		}
		greetMsg.setText(msg);
		Toast.makeText(getApplicationContext(), msg, Toast.LENGTH_SHORT).show();
		Command.syncPrev(com.getDB().getCurr_song_id());
	}
	private void addBarSsound(){  
		textview= (TextView)  findViewById(R.id.textView7);
		seekbar = (SeekBar) findViewById(R.id.seekBar1);
		seekbar.setMax(100);
		seekbar.setProgress(100);
	    seekbar.setOnSeekBarChangeListener( new OnSeekBarChangeListener(){
		   @Override 
		   public void onProgressChanged(SeekBar seekBar, int progress,
	               boolean fromUser) {
			   Command.syncSetVol(com.getDB().getCurr_song_id(), progress);
	       }
	
	       @Override
	       public void onStartTrackingTouch(SeekBar seekBar) {}
	       @Override
	       public void onStopTrackingTouch(SeekBar seekBar) {}}
	   );
	}
	
	private void addBarSeek(){  
		seekbar2 = (SeekBar) findViewById(R.id.seekBar2);
		if(com.getDB().getCurr_song_id() != 0)
			seekbar2.setMax(com.getDB().getSongs()[com.getDB().getCurr_song_id()].getSize());
		else
		{
			seekbar2.setMax(0);
		}
		seekbar2.setProgress(0);
		seekbar2.setOnSeekBarChangeListener( new OnSeekBarChangeListener(){
			@Override 
			public void onProgressChanged(SeekBar seekBar, int progress,
										  boolean fromUser) {
				seek=progress;
				if (fromUser){
					//user =fromUser;
					if(com.getDB().getCurr_song_id() != 0) {
						com.getDB().getSongs()[com.getDB().getCurr_song_id()].setPos(seek);
						seekbar2.setProgress(seek);
					}
				}		
			}
			
			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {}
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {		
				if(com.getDB().getCurr_song_id() != 0) {
					Command.syncPause(com.getDB().getCurr_song_id());
					Command.syncPlay(com.getDB().getCurr_song_id(), com.getDB().getSongs()[com.getDB().getCurr_song_id()].getVolume(), seek);      
				}
			}	
		});
	}
	
	public class SeekBarTimerTask extends TimerTask {
		public void run() {
			//update();
			if(com.getDB().getCurr_song_id() == 0) return;
			if(com.getDB().getSongs()[com.getDB().getCurr_song_id()].getStart()) {
				com.getDB().getSongs()[com.getDB().getCurr_song_id()].incPosByMs(100);
				seekbar2.setMax(com.getDB().getSongs()[com.getDB().getCurr_song_id()].getSize());
				seekbar2.setProgress(com.getDB().getSongs()[com.getDB().getCurr_song_id()].getPos());
			}
			handler.post(new Runnable(){
                public void run() {
                   update();
                }}); 
		}
	}
	
}