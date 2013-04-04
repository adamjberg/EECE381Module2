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
import android.widget.Switch;
import android.widget.ToggleButton;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;
import android.widget.Toast;

//import android.content.Intent;

public class play extends Activity {

	ImageButton back_button;
	TextView greetMsg;
	TextView textview;
	SeekBar seekbar_volume;
	SeekBar seekbar_pos;
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
		com.getDB().resume();
		handler = new Handler();
		greetMsg = (TextView) findViewById(R.id.pitchQuality);
		
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
		Intent intent;
		switch (item.getItemId()) {
		case android.R.id.home:
			finish();
			// NavUtils.navigateUpFromSameTask(this);
			// onBackPressed();
			return true;
		case R.id.songs:
			finish();
			intent = new Intent(play.this, MainActivity.class);
			intent.putExtra("subActivity", "true");
			startActivity(intent);
			return true;
		case R.id.menu_settings:
			intent = new Intent(play.this, MixerActivity.class);
			startActivity(intent);
			return true;
		case R.id.playlists:
			finish();
			intent = new Intent(play.this, PlaylistActivity.class);
			startActivity(intent);
			return true;
		case R.id.playMenu:
			Toast.makeText(play.this, "Already in play menu.",
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
			//if no song return 
			if(com.getDB().getCurr_song_id() >= com.getDB().getTotalSongs()) return;
			
			msg = "playing "+ com.getDB().getSongs()[com.getDB().getCurr_song_id()+1].getSongName();
			
			seekbar_pos.setMax(com.getDB().getSongs()[com.getDB().getCurr_song_id()+1].getSize());
			seekbar_volume.setProgress(com.getDB().getSongs()[com.getDB().getCurr_song_id()+1].getVolume());
		} else {
			if(db.getNextSongInList() == 0) return;
						
			msg = "playing "+ com.getDB().getSongs()[db.getNextSongInList()].getSongName();
			seekbar_pos.setMax(com.getDB().getSongs()[db.getNextSongInList()].getSize());
			seekbar_volume.setProgress(com.getDB().getSongs()[db.getNextSongInList()].getVolume());
		}
		greetMsg.setText(msg);
		Toast.makeText(getApplicationContext(), msg, Toast.LENGTH_SHORT).show();
		
		
		if(com.getDB().getIsEndOfPlaylist() && com.getDB().getRepeatPlaylistValue()) {
			// Then set the current song id 
			com.getDB().setCurr_song_id(db.getNextSongInList());
			Log.v("curr songid", ""+db.getCurr_song_id());
			Log.v("getNextSongInList", ""+db.getNextSongInList());
			
		}
		Command.syncNext(com.getDB().getCurr_song_id());
	}

	public void onPrev(View view) {
		Database db = com.getDB();
		String msg;
		if(com.getDB().getCurr_playlist_id() == 0) {
			if(com.getDB().getCurr_song_id() <=1) return;
			msg = "playing "+ com.getDB().getSongs()[com.getDB().getCurr_song_id()-1].getSongName();
			seekbar_pos.setMax(com.getDB().getSongs()[com.getDB().getCurr_song_id()-1].getSize());
			seekbar_volume.setProgress(com.getDB().getSongs()[com.getDB().getCurr_song_id()-1].getVolume());
		} else {
			if(db.getPrevSongInList() == 0) return;
			msg = "playing "+ com.getDB().getSongs()[db.getPrevSongInList()].getSongName();
			seekbar_pos.setMax(db.getSongs()[db.getPrevSongInList()].getSize());
			seekbar_volume.setProgress(db.getSongs()[db.getPrevSongInList()].getVolume());
		}
		greetMsg.setText(msg);
		Toast.makeText(getApplicationContext(), msg, Toast.LENGTH_SHORT).show();
		Command.syncPrev(com.getDB().getCurr_song_id());
	}
	public void repeatSongToggle(View view) {
		  // set db flag to reflect if it's toggled on or off
		  //com.getDB().setRepeatSong( ((ToggleButton) view).isChecked() );
		com.getDB().switchRepeatSong();
		 // Command.syncRepeatList(com.getDB().getCurr_playlist_id());
		 // Log.v("toggledRepeatPlaylist", ""+com.getDB().getRepeatPlaylistValue());
	  }
	  
	private void addBarSsound(){  
		textview= (TextView)  findViewById(R.id.textView7);
		seekbar_volume = (SeekBar) findViewById(R.id.seekBar1);
		seekbar_volume.setMax(100);
		seekbar_volume.setProgress(100);
	    seekbar_volume.setOnSeekBarChangeListener( new OnSeekBarChangeListener(){
		   @Override 
		   public void onProgressChanged(SeekBar seekBar, int progress,
	               boolean fromUser) {
	       }
	
	       @Override
	       public void onStartTrackingTouch(SeekBar seekBar) {}
	       @Override
	       public void onStopTrackingTouch(SeekBar seekBar) {
			   Command.syncSetVol(com.getDB().getCurr_song_id(), seekbar_volume.getProgress());}}
	   );
	}
	
	private void addBarSeek(){  
		seekbar_pos = (SeekBar) findViewById(R.id.seekBar2);
		if(com.getDB().getCurr_song_id() != 0)
			seekbar_pos.setMax(com.getDB().getSongs()[com.getDB().getCurr_song_id()].getSize());
		else
		{
			seekbar_pos.setMax(0);
		}
		seekbar_pos.setProgress(0);
		seekbar_pos.setOnSeekBarChangeListener( new OnSeekBarChangeListener(){
			@Override 
			public void onProgressChanged(SeekBar seekBar, int progress,
										  boolean fromUser) {
				seek=progress;
				if (fromUser){
					//user =fromUser;
					if(com.getDB().getCurr_song_id() != 0) {
						com.getDB().getSongs()[com.getDB().getCurr_song_id()].setPos(seek);
						seekbar_pos.setProgress(seek);
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
			if(com.getDB().getCurr_song_id() == 0 || com.getDB().isPaused()) return;
			
			seekbar_pos.setMax(com.getDB().getSongs()[com.getDB().getCurr_song_id()].getSize());
			if(com.getDB().getSongs()[com.getDB().getCurr_song_id()].getStart()) {
				com.getDB().getSongs()[com.getDB().getCurr_song_id()].incPosByMs(100);
				//seekbar_pos.setMax(com.getDB().getSongs()[com.getDB().getCurr_song_id()].getSize());
				seekbar_pos.setProgress(com.getDB().getSongs()[com.getDB().getCurr_song_id()].getPos());//}

			} else {
				seekbar_pos.setProgress(com.getDB().getSongs()[com.getDB().getCurr_song_id()].getSize());
				//com.getDB().getSongs()[com.getDB().getCurr_song_id()].setPos(0);
			}
						
			handler.post(new Runnable(){
                public void run() {
                   update();
                }}); 
			}
	}
	
}