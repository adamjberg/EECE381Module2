package com.example.myfirstapp;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.EditText;

public class MainActivity extends Activity {
	public final static String EXTRA_MESSAGE = "com.example.myfirstapp.MESSAGE";
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		Log.i("MY_MESSAGE", "in onCreate(MainActivity)");
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	protected void onResume() {
		super.onResume();
		Log.i("MY_MESSAGE", "in onResume(MainActivity)");
	}
	@Override
	protected void onPause() {
		super.onPause();
		Log.i("MY_MESSAGE", "in onPause(MainActivity)");
	}
	@Override
	protected void onStop() {
		super.onStop();
		Log.i("MY_MESSAGE", "in onStop(MainActivity)");
	}
	@Override
	protected void onDestroy() {
		super.onDestroy();
		Log.i("MY_MESSAGE", "in onDestory(MainActivity)");
	}
	@Override
	public void finish() {
		Log.i("MY_MESSAGE", "in finish(MainActivity)");
		super.finish();
	}
	public void sendMessage(View view) {
		Intent intent = new Intent(this, DisplayMessageActivity.class);
		EditText editText = (EditText) findViewById(R.id.edit_message);
		String message = editText.getText().toString();
		intent.putExtra(EXTRA_MESSAGE, message);
		startActivity(intent);
	}
}
