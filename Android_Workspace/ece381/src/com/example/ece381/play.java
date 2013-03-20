package com.example.ece381;

import android.app.Activity;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.support.v4.app.NavUtils;
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
	private boolean user;
	private Communication com = Communication.getInstance();
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
	greetMsg = (TextView) findViewById(R.id.textView1);
	Intent i = getIntent();
	String uname = (String) i.getSerializableExtra("USERNAME");
	if(com.getDB().getCurr_song_id() == 0)
		greetMsg.setText("No song has been selected");
	else
		greetMsg.setText("playing "+ com.getDB().getSongs()[com.getDB().getCurr_song_id()].getSongName());

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
	  return true;
 case R.id.menu_settings:
	  Toast.makeText(play.this,
	    item.getTitle(),
	     Toast.LENGTH_LONG).show();
	  return true;
 case R.id.playlists:
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
	 com.getDB().setCurr_song_id(0);
	 super.finish();
}
public void onPlay(View view){
	if(com.getDB().getCurr_song_id() != 0)
		Command.syncPlay(com.getDB().getCurr_song_id(), com.getDB().getSongs()[com.getDB().getCurr_song_id()].getVolume(), 0);
}

public void onPauseButton(View view) {
	if(com.getDB().getCurr_song_id() != 0)
		Command.syncPause(com.getDB().getCurr_song_id());
}

public void onNext(View view) {
	if(com.getDB().getCurr_song_id() >= com.getDB().getTotalSongs()) return;
	//if(com.getDB().getCurr_playlist_id() == 0) {
		greetMsg.setText("playing "+ com.getDB().getSongs()[com.getDB().getCurr_song_id()+1].getSongName());
		seekbar2.setMax(com.getDB().getSongs()[com.getDB().getCurr_song_id()+1].getSize());
		seekbar2.setProgress(0);
	//}
	Command.syncNext(com.getDB().getCurr_song_id());
}

public void onPrev(View view) {
	if(com.getDB().getCurr_song_id() <=1) return;
	//if(com.getDB().getCurr_playlist_id() == 0) {
		greetMsg.setText("playing "+ com.getDB().getSongs()[com.getDB().getCurr_song_id()-1].getSongName());
		seekbar2.setMax(com.getDB().getSongs()[com.getDB().getCurr_song_id()-1].getSize());
		seekbar2.setProgress(0);
	//}
	Command.syncPrev(com.getDB().getCurr_song_id());
}
private void addBarSsound(){  
	textview= (TextView)  findViewById(R.id.textView7);
	seekbar = (SeekBar) findViewById(R.id.seekBar1);
    seekbar.setOnSeekBarChangeListener( new OnSeekBarChangeListener(){
	   @Override 
	   public void onProgressChanged(SeekBar seekBar, int progress,
               boolean fromUser) {
		   //textview.setText(textview.getText()+"\n"+"SeekBar now at the value of:"+progress);
		   Command.syncSetVol(com.getDB().getCurr_song_id(), progress);
		   
          
       }

       @Override
       public void onStartTrackingTouch(SeekBar seekBar) {}
       @Override
       public void onStopTrackingTouch(SeekBar seekBar) {}}
   );
  }
private void addBarSeek(){  
	//textview= (TextView)  findViewById(R.id.textView7);
	seekbar2 = (SeekBar) findViewById(R.id.seekBar2);
	//seek_textview = (TextView) findViewById(R.id.textView10);
	if(com.getDB().getCurr_song_id() != 0)
		seekbar2.setMax(com.getDB().getSongs()[com.getDB().getCurr_song_id()].getSize());
		
		//seek_textview.setText(""+ com.getDB().getSongs()[com.getDB().getCurr_song_id()].getSize() *1000);}
	else
	{
		seekbar2.setMax(0);
		//seek_textview.setText(""+0);	
	}
	seekbar2.setProgress(0);
	
	//seekbar.setProgress(c+1);
	seekbar2.setOnSeekBarChangeListener( new OnSeekBarChangeListener(){
		@Override 
		public void onProgressChanged(SeekBar seekBar, int progress,
									  boolean fromUser) {
			if (fromUser){
				user =fromUser;
				//seek_textview.setText(""+progress);
				seek=progress;
			}
			//seek_textview.setText(""+progress);
			
			
		}
		
		
		@Override
		public void onStartTrackingTouch(SeekBar seekBar) {}
		@Override
		public void onStopTrackingTouch(SeekBar seekBar) {		
				if(com.getDB().getCurr_song_id() != 0)
					Command.syncPlay(com.getDB().getCurr_song_id(), 1, seek);           }
			
		}
											);
	}
}