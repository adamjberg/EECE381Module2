package com.example.playlistactivity;

import java.util.ArrayList;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.example.ece381.Command;
import com.example.ece381.Communication;
import com.example.ece381.Database;
import com.example.ece381.Song;


public class AllSongsListActivity extends Activity {

	  // create an instance of the database
	  private Communication com = Communication.getInstance();
	  private Database db = com.getDB();
	  
	  // Create an arraylist of strings to display onto the Adapter
	  ArrayList<String> songs_list = new ArrayList<String>();

	  private ListView allSongsListView;
	  private ArrayAdapter<String> listAdapter;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_all_songs_list);
		
		 // Find the ListView resource. 
	    allSongsListView = (ListView) findViewById( R.id.allSongsListView );

	    // iterate thru playlist array; retrieve the names of each playlist, and add it to arraylist
	    for(int i = 1; i <= db.getTotalSongs(); i++ ) {
			   songs_list.add(db.getSongs()[i].getSongName());
	    }
	    
	    // Create ArrayAdapter using the song names.
	    listAdapter = new ArrayAdapter<String>(this, R.layout.simplerow, songs_list); 
	    
	    
	    // On click of an item
	    allSongsListView.setOnItemClickListener(new OnItemClickListener() {
	    	@Override
	    	public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
	    		
	    		// Show a toast box with the name of the item
			   //Toast.makeText(getApplicationContext(), ((TextView) view).getText() + "added", Toast.LENGTH_SHORT).show();
	    		
			   String selected = (String) (allSongsListView.getItemAtPosition(position));
	    		
			   // Add song to playlist, then go back
			   int songid = 0;
			   
			   for(int i = 1; i < db.getTotalSongs()+1; i++) {
				   if(selected == db.getSongs()[i].getSongName() )
					   songid = i;
			   }
			   
			   // Set current song ID to the one just added
			   db.setCurr_song_id(songid);
		       Log.d("setCurr_songid: ", ""+songid);
			   // add song to database
			   Command.syncAddSongToList(db.getCurr_playlist_id(), db.getCurr_song_id());
		       Log.d("getCurr_plid: ", ""+db.getCurr_playlist_id());
			//  db.addSongToList(db.getCurr_playlist_id(), db.getCurr_song_id());
			   
			   // Return intent
			   Intent returnIntent = new Intent();
			   returnIntent.putExtra("songID", songid);
			   setResult(RESULT_OK, returnIntent);
			   finish();
	    	}
	    });
	    
	    
	    // Set the ArrayAdapter as the ListView's adapter.
	    allSongsListView.setAdapter( listAdapter );      
	    
	    
	}
/*
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.all_songs_list, menu);
		return true;
	}
*/
}
