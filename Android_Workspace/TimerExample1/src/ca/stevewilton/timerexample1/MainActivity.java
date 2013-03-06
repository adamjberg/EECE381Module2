package ca.stevewilton.timerexample1;

import java.util.Timer;
import java.util.TimerTask;


import android.app.Activity;
import android.os.Bundle;
import android.support.v4.app.NavUtils;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

public class MainActivity extends Activity {

	
	Timer my_timer = new Timer();
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		MyTimerTask my_task = new MyTimerTask();
		my_timer.schedule(my_task, 3000, 500);
	}
	
	@Override
	protected void onStop() {
		super.onStop();
	}

	@Override
	public void finish() {

		my_timer.cancel();
		super.finish();
	}
	
	public class MyTimerTask extends TimerTask {
		int cnt = 0;
		public void run() {
			Log.i("MyTimerTask", Integer.toString(cnt++));
		}
	}
}
