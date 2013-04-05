package com.example.ece381;

import android.app.Activity;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

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

		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
			getWindow().requestFeature(Window.FEATURE_ACTION_BAR);
		}
		setContentView(R.layout.play);
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
			// Show the Up button in the action bar.
			this.getActionBar().setDisplayHomeAsUpEnabled(true);
		}

		this.setContentView(R.layout.song_property);
		songId = this.getIntent().getIntExtra(ID_KEY, 0);
		initializeProgressBars();
		setSongName();
		pitchQuality = (EditText) findViewById(R.id.pitchQualityInput);
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
			return true;
		case R.id.songs:
			intent = new Intent(SongPropertyActivity.this, MainActivity.class);
			startActivity(intent);
			return true;
		case R.id.menu_settings:
			intent = new Intent(SongPropertyActivity.this, MixerActivity.class);
			startActivity(intent);
			return true;
		case R.id.playlists:
			intent = new Intent(SongPropertyActivity.this,
					PlaylistActivity.class);
			startActivity(intent);
			return true;
		case R.id.playMenu:
			intent = new Intent(SongPropertyActivity.this, play.class);
			startActivity(intent);
			return true;
		}
		return super.onOptionsItemSelected(item);
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
		if (pitchQuality.getText().length() > 0) {
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
		volumeBar.setProgress(100);
		pitchBar.setProgress(50);
		speedBar.setProgress(50);
	}

	public void onPlayPressed(View view) {
		Command.syncPlay(songId, volumeBar.getProgress(), 0);
	}
}
