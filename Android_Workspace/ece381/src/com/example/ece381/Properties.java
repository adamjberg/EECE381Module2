package com.example.ece381;

import android.app.Activity;
import android.os.Bundle;
import android.widget.SeekBar;

public class Properties extends Activity{
	private String name;
	private int length;
	private int songID;
	private int position;
	private Clip thisClip;
	private int start;
	private int ends;
	private SeekBar cropper;
	
	public void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
	}
	
	private void inputName(){
		
	}
	
	private void cutClip(){
		
	}
	
	private void addToMix( Mix mix){
		mix.addClip(thisClip);
	}
	
	private void toClip(){
		thisClip = new Clip(name, length);
	}
	
	private void playSegment(){
		
	}
	
	private void saveAs(){
		
	}
}
