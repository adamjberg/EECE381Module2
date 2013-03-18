package com.example.playlistactivity;

import java.util.ArrayList;

import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.view.Menu;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

import com.example.ece381.Communication;
import com.example.ece381.Database;
import com.example.ece381.Playlist;

public class PlaylistBuilderActivity extends FragmentActivity {
	
	private Communication com = Communication.getInstance();
	private Database db = com.getDB();
	
	// Create an arraylist for adding songs
	public ArrayList<String> list = new ArrayList<String>();
	  
	private ListView builderListView ;
	private ArrayAdapter<String> listAdapter;
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_playlist_builder);
		
	    builderListView = (ListView) findViewById( R.id.builderListView );
		
		TextEntryDialog plNameEntryDialog = new TextEntryDialog();
		plNameEntryDialog.show(getSupportFragmentManager(), "Dialog");
		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.playlist_builder, menu);
		return true;
	}
	
	// Callback method from TextEntryDialog, returning the value of user input
	public void onUserSelectValue(String selectedValue) {
		Toast.makeText(this, "Playlist created", Toast.LENGTH_SHORT).show();
		
		// create a new playlist with the name entered
		Playlist new_pl = new Playlist(selectedValue); 
		
		db.addList(new_pl);
	}

}
