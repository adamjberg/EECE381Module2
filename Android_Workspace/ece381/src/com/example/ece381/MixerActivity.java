package com.example.ece381;


import java.util.ArrayList;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Point;
import android.os.Bundle;
import android.os.Debug;
import android.util.Log;
import android.view.DragEvent;
import android.view.MotionEvent;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;

public class MixerActivity extends Activity{
	public static Mix theMix;
	private SeekBar Timeline;
	public static int clipIndex;
	public static MixerCanvas mixerCanvas;
	private TextView textprog;
	private ListView songList;
	private Point p = new Point();
	public static ArrayList <String> songs = new ArrayList<String>();
	public static int idOfSongSelected = 1;
	public static boolean selSong = true; //true = a song is selected| false = mixelement is selected if songid =0
	private ArrayAdapter<String> listAdapter ;
	private Communication com = Communication.getInstance();
	
	
	@Override		
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		this.setContentView(R.layout.mixer);
		setUP();


	}
	
	private void addTimeline() {
		Timeline = (SeekBar) findViewById(R.id.scroll);
		Timeline.setMax(theMix.lengthOf() + 30000 /*30 sec*/);
		Timeline.setProgress(0);
		Timeline.setOnSeekBarChangeListener(new OnSeekBarChangeListener(){
			@Override 
			public void onProgressChanged(SeekBar Timeline, int progress,
										  boolean fromUser) {
				if(fromUser){
					theMix.seekTime(progress);
					Timeline.setProgress(progress);
					mixerCanvas.setProgress(progress);
					String temp = Integer.toString(progress) + " / " + Integer.toString(Timeline.getMax());
					textprog.setText(temp);
					mixerCanvas.invalidate();
				}
			}
			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {}
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {}
		});
	}
	
	public void setUP(){
		textprog = (TextView) findViewById(R.id.progress);
		textprog.setTextColor(Color.BLACK);
		songList = (ListView) findViewById(R.id.listView1);
		theMix = new Mix();
		populateSongs();
		addTimeline();
		String temp =  "0" + " / " + Integer.toString(Timeline.getMax());
		textprog.setText(temp);
		
		listAdapter = new ArrayAdapter<String>(this, 0, songs);
		
		mixerCanvas = (MixerCanvas) this.findViewById(R.id.drawSurface);
		mixerCanvas.setBackgroundColor(Color.BLUE);
		mixerCanvas.setOnTouchListener(new OnTouchListener(){
			@Override
			public boolean onTouch(View v, MotionEvent mE) {
				// TODO Auto-generated method stub
				p.x=(int) mE.getX();
				p.y=(int) mE.getY();
				p.y = decodePoint(p);
				if(selSong = true && idOfSongSelected != 0){
				mixerCanvas.addElement(p);}
				else{
					if(mixerCanvas.trySelElement(p)){
						idOfSongSelected = 0;
						selSong = false;
					}
				}
				return false;
			}
		});
		//may add more init in here
		
	}
	
	public int decodePoint(Point p){
		//Log.v("p", p.toString());
		return (p.y/100);
	}
	
	public void populateSongs(){
	}
	
	public void onPlay(){
		
	}
	
	public void onStopPlay(){
		
	}
	
	public void onSave(){
		
	}
	
	public void onDelete(){
		if(selSong = true && idOfSongSelected != 0 ){
			mixerCanvas.deleteAllInstanceOf(idOfSongSelected);}
		else if(selSong = false && idOfSongSelected ==0){
			mixerCanvas.deleteSelectedSeg();
		}
		
		else{
		selSong = false;
		idOfSongSelected = 0;
		}
	}
	
	public void onVolumeChange(){
		
	}
	
	public void onProperties(){
		
	}
}
