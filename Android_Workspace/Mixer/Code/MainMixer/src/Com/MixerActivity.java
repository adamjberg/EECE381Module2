package Com;


import android.app.Activity;
import android.os.Bundle;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;

public class MixerActivity extends Activity{
	private static Mix theMix;
	private SeekBar Timeline;
	public static int clipIndex;
	
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		//this.setContentView();
		
		theMix = new Mix();
		addTimeline();
		
	}
	
	
	private void addTimeline() {
		//Timeline = (SeekBar) findViewById(com.example.mixer.R.id.Timeline);
		Timeline.setMax(theMix.lengthOf() + 30000 /*30 sec*/);
		Timeline.setProgress(0);
		
		Timeline.setOnSeekBarChangeListener(new OnSeekBarChangeListener(){
			@Override 
			public void onProgressChanged(SeekBar Timeline, int progress,
										  boolean fromUser) {
				if(fromUser){
					theMix.seekTime(progress);
					Timeline.setProgress(progress);
				}
			}
			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {}
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {}
		});
	}
	
	public void setUP(){
		theMix.addClip(new Clip("clip one", 5));
		theMix.addClip(new Clip("clip two", 10));
		theMix.addClip(new Clip("clip three", 2));
		
	}
	
}
