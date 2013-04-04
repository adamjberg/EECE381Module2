package com.example.ece381;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.ProgressBar;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;

public class SongPropertyActivity extends Activity {

	private Clip clip;
	private SeekBar volumeBar;
	private SeekBar pitchBar;
	private SeekBar lengthBar;
	private SeekBar speedBar;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		this.setContentView(R.layout.song_property);
		clip = MixerActivity.selectedClip;
		initializeProgressBars();
		setSongName();
	}
	
	private void initializeProgressBars() {
		volumeBar = (SeekBar) findViewById(R.id.volumeBar);
		pitchBar = (SeekBar) findViewById(R.id.pitchBar);
		lengthBar = (SeekBar) findViewById(R.id.lengthBar);
		speedBar = (SeekBar) findViewById(R.id.speedBar);
		
		volumeBar.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
				clip.setVolume(volumeBar.getProgress());
			}

			@Override
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				
			}

			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
				
			}
		});
		
		pitchBar.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
				clip.setPitch(pitchBar.getProgress());
			}

			@Override
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				
			}

			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
				
			}
		});
		
		lengthBar.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
				clip.setLength(lengthBar.getProgress());
			}

			@Override
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				
			}

			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
				
			}
		});
		
		speedBar.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
				clip.setPlaybackSpeed(speedBar.getProgress());
			}

			@Override
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				
			}

			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
				
			}
		});
		
		volumeBar.setProgress(clip.getVolume());
		pitchBar.setProgress(clip.getPitch());
		lengthBar.setProgress(clip.getLength());
		speedBar.setProgress(clip.getPlaybackSpeed());
	}
	
	private void setSongName() {
		TextView songName = (TextView) findViewById(R.id.songName);
		songName.setText(clip.getName());
	}
	
	public void onVolumeChanged(ProgressBar bar) {
		clip.setVolume(bar.getProgress());
	}

	public void onPitchChanged(ProgressBar bar) {
		clip.setPitch(bar.getProgress());
	}

	public void onLengthChanged(ProgressBar bar) {
		clip.setLength(bar.getProgress());
	}

	public void onPlayBackSpeedChanged(ProgressBar bar) {
		clip.setPlaybackSpeed(bar.getProgress());
	}

	public void onSyncPressed(View view) {
		clip.updateSongInDatabase();
	}
	
	public void onReloadSong(View view) {
		clip.reload();
	}
	
	public void onPlayPressed(View view) {
		clip.play();
	}
}
