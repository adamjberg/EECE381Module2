package com.example.playlistactivity;

import java.util.ArrayList;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.example.ece381.Database;
import com.example.ece381.Playlist;

public class PlaylistActivity extends Activity {
  
  private ListView mainListView ;
  private ArrayAdapter<String> listAdapter ;
  
  /** Called when the activity is first created. */
  @Override
  public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_playlist);
    
    // Find the ListView resource. 
    mainListView = (ListView) findViewById( R.id.mainListView );

    // Create an arraylist of strings to display onto the Adapter
    ArrayList<String> playlist_names = new ArrayList<String>();
   
    // create an instance of the database
    Database db = new Database();
    
    // Testing: add a playlist into the database
    Playlist testpl = new Playlist("name");
    db.addList(testpl);
    
    Playlist[] pl = db.getPlaylistArray();
    
    // iterate thru playlist array; retrieve the names of each playlist, and add it to arraylist
    for(int i = 1; i < db.getNumLists()+1; i++ ) {
		   playlist_names.add(pl[i].getListName());
    }
      
    
    // Create ArrayAdapter using the planet list.
    listAdapter = new ArrayAdapter<String>(this, R.layout.simplerow, playlist_names);
    
    // Set the ArrayAdapter as the ListView's adapter.
    mainListView.setAdapter( listAdapter );      
    
    
    // On click of an item:
    mainListView.setOnItemClickListener(new OnItemClickListener() {
    	@Override
    	public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
    		Intent goToSongList = new Intent(getApplicationContext(), SongActivity.class);
    		startActivity(goToSongList);
    	}
  
    });
    

    
    
  }
}