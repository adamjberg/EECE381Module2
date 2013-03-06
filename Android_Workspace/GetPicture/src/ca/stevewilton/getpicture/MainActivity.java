package ca.stevewilton.getpicture;


import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.ProgressBar;

public class MainActivity extends Activity {

	ImageView image_view_location;
	EditText input_line;
	ProgressBar progress_bar;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		image_view_location = (ImageView)findViewById(R.id.imageView1);
		input_line = (EditText)findViewById(R.id.editText1);
		progress_bar = (ProgressBar)findViewById(R.id.progressBar1);
	}

	public void onButtonClick(View view) {
		String website_location = input_line.getText().toString();
		
		new NewAsyncTask().execute(website_location);
	}
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	class NewAsyncTask extends AsyncTask<String, Integer, Bitmap> {
		@Override
		protected void onProgressUpdate(Integer... percent_done_array) {
			progress_bar.setProgress(percent_done_array[0]);
		}
		@Override
		protected Bitmap doInBackground(String... url_array) {
			URL url;
			Log.i("MainActivity", "Inside the asynchronous task");
			try {
				publishProgress(0);
				url = new URL(url_array[0]);
				HttpURLConnection connection = (HttpURLConnection) url.openConnection();
				publishProgress(25);
				connection.setDoInput(true);
				connection.connect();
				publishProgress(50);
				Log.i("MainActivity", "Successfully opened the web page");
				
				InputStream input = connection.getInputStream();
				publishProgress(75);
				Bitmap bitmap = BitmapFactory.decodeStream(input);
				input.close();
				publishProgress(100);
				return bitmap;
			} catch(IOException e) {
				e.printStackTrace();
				return null;
			}
		}
		
		@Override
		protected void onPostExecute(Bitmap result) {
			image_view_location.setImageBitmap(result);
		}
	}
}
