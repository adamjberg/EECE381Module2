package com.example.playlistactivity;

import java.util.ArrayList;

import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.util.Log;
import android.view.Menu;
import android.widget.ArrayAdapter;
import android.widget.ListView;

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
		//Toast.makeText(this, "Playlist created", Toast.LENGTH_SHORT).show();
		
		// create a new playlist with the name entered
		Playlist new_pl = new Playlist(selectedValue); 
		db.addList(new_pl);
		
		// find the playlist id
			int plid = getSelectedPlaylistId(selectedValue);
		   
		   db.setCurr_playlist_id(plid);
		   // Log plid
	       Log.d("setCurr_plid: ", ""+plid); 
	       
		// return to PlaylistActivity
		Intent returnIntent = new Intent();
		returnIntent.putExtra("PLid", plid);
		setResult(RESULT_OK, returnIntent);
		this.finish(); 
		
//	Intent returnIntent = new Intent(getApplicationContext(),  PlaylistActivity.class);
//	startActivity(returnIntent);
	}

	  public int getSelectedPlaylistId(String selectedValue) {
		   int plid = 0;
		   
		   for(int i = 1; i < db.getNumLists()+1; i++) {
			   if(selectedValue == db.getPlaylists()[i].getListName() )
				   plid = i;
		   }
		   return plid;
	  }
	  
	  public int getSelectedSongId(String selectedValue) {
		  int songid = 0;
		   for(int i = 1; i < db.getTotalSongs()+1; i++) {
			   if(selectedValue == db.getSongs()[i].getSongName() )
				   songid = i;
		   }
		   return songid;
	  }
	 
	
}

