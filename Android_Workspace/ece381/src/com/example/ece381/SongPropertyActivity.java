package com.example.ece381;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.TextView;

public class SongPropertyActivity extends Activity {

	public static final String ID_KEY = "ID";
	
	private int songId;
	private SeekBar volumeBar;
	private SeekBar pitchBar;
	private SeekBar speedBar;
	private EditText pitchQuality;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		this.setContentView(R.layout.song_property);
		songId = this.getIntent().getIntExtra(ID_KEY, 0);
		System.out.println("song id: " + songId);
		initializeProgressBars();
		setSongName();
		pitchQuality = (EditText) findViewById(R.id.pitchQualityInput);
	}
	
	private void initializeProgressBars() {
		volumeBar = (SeekBar) findViewById(R.id.volumeBar);
		pitchBar = (SeekBar) findViewById(R.id.pitchBar);
		speedBar = (SeekBar) findViewById(R.id.speedBar);
	}
	
	private void setSongName() {
		TextView songName = (TextView) findViewById(R.id.songName);
		Song[] songs = Communication.getInstance().getDB().getSongs();
		if (songs[songId] != null) {
			songName.setText(songs[songId].getSongName());
		} else {
			songName.setText("Invalid song name");
		}

	}

	public void onSyncPressed(View view) {
		int pitchQual;
		if(pitchQuality.getText().length() > 0) {
			pitchQual = Integer.parseInt(pitchQuality.getText().toString());
		} else {
			pitchQual = 0;
		}
		Command.syncSetPitch(songId, pitchBar.getProgress(), pitchQual);
		Command.syncSetPlaybackSpeed(songId, speedBar.getProgress());
		Command.syncSetVol(songId, volumeBar.getProgress());
	}
	
	public void onReloadSong(View view) {
		Command.syncReloadSong(songId);
	}
	
	public void onPlayPressed(View view) {
		Command.syncPlay(songId, volumeBar.getProgress(), 0);	
	}
}
