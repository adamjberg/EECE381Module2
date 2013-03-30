package com.example.ece381;

import java.util.ArrayList;

import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.util.Log;
import android.view.Menu;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

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
		
		String action = null;
		// Check action; 0 = create; 1 = modify 
		action = getIntent().getStringExtra("action");
		
		Log.v("action", action);
		
		if( checkNameValid(selectedValue) ) {
			// Check whether or not the list already exists
			for(int j = 0; j < db.getListsName().length; j++) {
				if(db.getListsName()[j] == selectedValue) {
					Toast.makeText(this, "Playlist already exists!", Toast.LENGTH_SHORT).show();
					return;
				}
			}
			if(action.equals("create")) {
				// create a new playlist with the name entered
				Command.syncCreatePlaylist(selectedValue);
			}
			else if(action.equals("rename")) {
				// modify playlist name
				Log.v("selectedlist", db.getSelectedList()+"");
				
				db.getPlaylists()[db.getSelectedList()].setListName(selectedValue);
			}
		}
		else {
			Toast.makeText(this, "Invalid playlist name!", Toast.LENGTH_SHORT).show();
		}
		
		// return to PlaylistActivity
		Intent returnIntent = new Intent();
		setResult(RESULT_OK, returnIntent);
		this.finish();
		
	}
	  
	  public int getSelectedSongId(String selectedValue) {
		  int songid = 0;
		   for(int i = 1; i < db.getTotalSongs()+1; i++) {
			   if(selectedValue == db.getSongs()[i].getSongName() )
				   songid = i;
		   }
		   return songid;
	  }
	 
	public boolean checkNameValid(String name) {
		// Check if the name is illegal
		if (name.equals("")  || name == null || name.equals("0") )
			return false;
		return true;
	}
}

