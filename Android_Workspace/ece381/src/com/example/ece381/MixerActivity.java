package com.example.ece381;


import java.util.ArrayList;
import java.util.Timer;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.Point;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;

public class MixerActivity extends Activity {	
	public static Mix theMix;
	private SeekBar Timeline;
	private SeekBar globalVolumeBar;
	public static MixerCanvas mixerCanvas;
	private TextView textprog;
	public static ListView songList;
	public static Clip selectedClip;
	public static int idOfSongSelected = 0;
	public static boolean selSong = false; //true = a song is selected| false = mixelement is selected if songid =0
	public static int indexOfSel = -1;
	private ArrayAdapter<String> listAdapter;
	private Timer play_timer = new Timer();
	
	private Communication com = Communication.getInstance();
	private Database db = com.getDB();
	
	@Override		
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		this.setContentView(R.layout.mixer2);
		setUP();
		refreshList();
	}

	@Override
	public void onResume() {
		//Log.v("PlaylistAcitivty Resume", "");
		super.onResume();
		refreshList();
		Command.syncOpenPlaylistsPanel();
	}
	
	@Override
	public void onPause(){
		refreshList();
		mixerCanvas.invalidate();
		super.onPause();
	}
	
	public void createClipsFromSongs() {
		Song[] songs = db.getSongs();
		Song nextSong;
		for(int i = 0; i < db.getTotalSongs(); i++) {
			nextSong = songs[i];
			if(nextSong != null) {
				theMix.addClip(new Clip(nextSong));
			}
		}
	}
	
	public void setUP() {
		textprog = (TextView) findViewById(R.id.progress);
		textprog.setTextColor(Color.BLUE);
		songList = (ListView) findViewById(R.id.listView1);
		theMix = new Mix();
		addTimeline();
		String temp =  "0" + " / " + Integer.toString(Timeline.getMax());
		textprog.setText(temp);
		listAdapter = new ArrayAdapter<String>(this, R.layout.simplerow, db.getSongsName());
				
		globalVolumeBar = (SeekBar) this.findViewById(R.id.volume);
		globalVolumeBar.setOnSeekBarChangeListener(new VolumeSeekChangeListener());
		
		songList.setOnItemClickListener(new OnItemClickListener(){

			@Override
			public void onItemClick(AdapterView<?> adv, View v, int index,
					long it) {
				indexOfSel = index;
				selectedClip = theMix.getClipat(index);
				idOfSongSelected = selectedClip.getID();
				mixerCanvas.selIndex = -1;
				//System.out.println( "When Clicked" + indexOfSel + " ID:" + idOfSongSelected  );
				mixerCanvas.invalidate();
			}
			
		});
		mixerCanvas = (MixerCanvas) this.findViewById(R.id.drawSurface);
		mixerCanvas.setBackgroundColor(Color.BLUE);
		mixerCanvas.setOnTouchListener(new OnTouchListener(){
			@Override
			public boolean onTouch(View v, MotionEvent mE) {
				Point p = new Point((int) mE.getX(), (int) mE.getY());
				p.y = decodePoint(p);
				if(selSong = true && idOfSongSelected != 0 && indexOfSel >= 0){
				mixerCanvas.addElement(p);
				//Timeline.setMax(theMix.lengthOf() + 3000 /*30 sec*/);
				}
				else if(mixerCanvas.trySelElement(p)){
						idOfSongSelected = 0;
						selSong = false;
					}
				else{
					mixerCanvas.replace(p);
				}
				return false;
			}
		});
		songList.setAdapter(listAdapter); 
		createClipsFromSongs();
	}
	
	private void addTimeline() {
		Timeline = (SeekBar) findViewById(R.id.scroll);
		Timeline.setMax(theMix.lengthOf() + 3000 /*30 sec*/);
		Timeline.setProgress(0);
		Timeline.setOnSeekBarChangeListener(new TimelineSeekChangeListener());
	}
	
	public int decodePoint(Point p){
		return (p.y/100);
	}
	
	public void onPlayPressed(View view) {
		theMix.play(globalVolumeBar.getProgress());
	}

	public void onStopPressed(View view) {
		theMix.stop();
	}

	public void onPropertiesPressed(View view) {
		Intent i = new Intent(this, SongPropertyActivity.class);
		i.putExtra(SongPropertyActivity.ID_KEY, selectedClip.getID());
		startActivity(i);
	}
	
	public void onSavePressed(View view) {
		//insert a name for the mix
		theMix.saveMix();
	}

	public void onDeletePressed(View view) {
		if (selSong == true && idOfSongSelected != 0) {
			mixerCanvas.deleteAllInstanceOf(idOfSongSelected);
		} else if (selSong == false && idOfSongSelected == 0) {
			mixerCanvas.deleteSelectedSeg();
		}

		else {
			selSong = false;
			idOfSongSelected = 0;
		}
	}
	
	public int getSelectedSongId(String selectedValue) {
		int songid = 0;
		for(int i = 1; i < db.getTotalSongs()+1; i++) {
			if(selectedValue == db.getSongs()[i].getSongName() )
				songid = i;
			}
		return songid;
	}
	  
	public void refreshList() {
		// listAdapter.clear();
		// listAdapter.addAll(db.getSongsName());
		// listAdapter.notifyDataSetChanged();
	}
	
	private class VolumeSeekChangeListener implements OnSeekBarChangeListener {
		@Override
		public void onProgressChanged(SeekBar volumeBar, int progress,
				boolean fromUser) {
		}

		@Override
		public void onStartTrackingTouch(SeekBar volumeBar) {
		}

		@Override
		public void onStopTrackingTouch(SeekBar volumeBar) {
			Clip clip;
			ArrayList<Clip> activeClips = theMix.findNowActiveClips();
			for(int i = 0; i < activeClips.size(); i++) {
				clip = activeClips.get(i);
				Command.syncSetVol(clip.getID(), (volumeBar.getProgress() + clip.getVolume()) / 2);
			}
		}
	}
	
	private class TimelineSeekChangeListener implements OnSeekBarChangeListener {
		@Override
		public void onProgressChanged(SeekBar Timeline, int progress,
				boolean fromUser) {
			if (fromUser) {
				theMix.seekTime(progress);
				Timeline.setProgress(progress);
				mixerCanvas.setProgress(progress);
				String temp = Integer.toString(progress) + " / "
						+ Integer.toString(Timeline.getMax());
				textprog.setText(temp);
				mixerCanvas.invalidate();
			}
		}

		@Override
		public void onStartTrackingTouch(SeekBar seekBar) {
		}

		@Override
		public void onStopTrackingTouch(SeekBar seekBar) {
		}
	}
}
