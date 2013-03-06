package ca.stevewilton.slideshow;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.widget.ImageView;

public class MainActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		Timer my_timer = new Timer();
		MyTimerTask my_task = new MyTimerTask();
		my_timer.schedule(my_task, 3000, 3000);
	}

	public class MyTimerTask extends TimerTask {
		String bvase = "http://www.ece.ubc.ca/~eece381/";
		String names[] = {"1.jpg", "2.jpg", "3.jpg"};
		Bitmap bitmaps[] = {null, null, null};
		
		int cnt = 0;
		public void run() {
			if(bitmaps[cnt]==null) {
				try {
					URL url = new URL(bvase+names[cnt]);
					HttpURLConnection connection = (HttpURLConnection) url.openConnection();
					connection.setDoInput(true);
					connection.connect();
					InputStream input = connection.getInputStream();
					bitmaps[cnt] = BitmapFactory.decodeStream(input);
					input.close();
				} catch(IOException e) {
					e.printStackTrace();
					return;
				}
			}
		
			runOnUiThread(new Runnable() {
				public void run() {
					ImageView image_view_location = (ImageView)findViewById(R.id.imageView1);
					image_view_location.setImageBitmap(bitmaps[cnt]);
				}
			});
			cnt = (cnt+1)%3;
		}
	}
}
