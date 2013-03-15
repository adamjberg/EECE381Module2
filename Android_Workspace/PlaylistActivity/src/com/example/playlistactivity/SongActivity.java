package com.example.playlistactivity;

import java.util.ArrayList;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.example.ece381.Database;
import com.example.ece381.Song;

public class SongActivity extends Activity {
  
  private ListView songListView ;
  private ArrayAdapter<String> listAdapter ;
  
  /** Called when the activity is first created. */
  @Override
  public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_song);
    
    // Find the ListView resource. 
    songListView = (ListView) findViewById( R.id.songListView );

    // Create an arraylist of strings to display onto the Adapter
    ArrayList<String> song_names = new ArrayList<String>();
   
    // create an instance of the database
    Database db = new Database();
    
    // Testing: add a playlist into the database
    Song testsong = new Song("Song1.wav");
    db.addSong(testsong);
    
    Song[] pl = db.getSongArray();
    
    // iterate thru playlist array; retrieve the names of each playlist, and add it to arraylist
    for(int i = 1; i < db.getNumSongs()+1; i++ ) {
		   song_names.add(pl[i].getSongName());
    }
      
    
    // Create ArrayAdapter using the planet list.
    listAdapter = new ArrayAdapter<String>(this, R.layout.simplerow, song_names);
    
    // Set the ArrayAdapter as the ListView's adapter.
    songListView.setAdapter( listAdapter );      
    
    
    // On click of an item:
    songListView.setOnItemClickListener(new OnItemClickListener() {
    	@Override
    	public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
		    Toast.makeText(getApplicationContext(),
			((TextView) view).getText(), Toast.LENGTH_SHORT).show();
    	}
    });
    

    
    
  }
}