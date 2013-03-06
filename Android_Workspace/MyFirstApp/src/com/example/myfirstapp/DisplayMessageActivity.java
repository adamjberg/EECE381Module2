package com.example.myfirstapp;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.support.v4.app.NavUtils;
import android.util.Log;
import android.view.MenuItem;
import android.widget.TextView;

public class DisplayMessageActivity extends Activity {

	@SuppressLint("NewApi")
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		Intent intent = getIntent();
		String message = intent.getStringExtra(MainActivity.EXTRA_MESSAGE);
		
		TextView textView = new TextView(this);
		textView.setTextSize(40);
		textView.setText(message);
		
		setContentView(textView);
		// Show the Up button in the action bar.
		setupActionBar();
		Log.i("MY_MESSAGE", "in onCreate(DisplayMessageActivity)");
	}

	@Override
	protected void onResume() {
		super.onResume();
		Log.i("MY_MESSAGE", "in onResume(DisplayMessageActivity)");
	}
	@Override
	protected void onPause() {
		super.onPause();
		Log.i("MY_MESSAGE", "in onPause(DisplayMessageActivity)");
	}
	@Override
	protected void onStop() {
		super.onStop();
		Log.i("MY_MESSAGE", "in onStop(DisplayMessageActivity)");
	}
	@Override
	protected void onDestroy() {
		super.onDestroy();
		Log.i("MY_MESSAGE", "in onDestory(DisplayMessageActivity)");
	}
	@Override
	public void finish() {
		Log.i("MY_MESSAGE", "in finish(DisplayMessageActivity)");
		super.finish();
	}
	/**
	 * Set up the {@link android.app.ActionBar}, if the API is available.
	 */
	@TargetApi(Build.VERSION_CODES.HONEYCOMB)
	private void setupActionBar() {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
			getActionBar().setDisplayHomeAsUpEnabled(true);
		}
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case android.R.id.home:
			// This ID represents the Home or Up button. In the case of this
			// activity, the Up button is shown. Use NavUtils to allow users
			// to navigate up one level in the application structure. For
			// more details, see the Navigation pattern on Android Design:
			//
			// http://developer.android.com/design/patterns/navigation.html#up-vs-back
			//
			NavUtils.navigateUpFromSameTask(this);
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

}
