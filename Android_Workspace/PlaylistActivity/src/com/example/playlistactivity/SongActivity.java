package com.example.playlistactivity;

import java.util.ArrayList;

import android.app.Activity;
import android.os.Bundle;
import android.view.ContextMenu;
import android.view.View;
import android.view.ContextMenu.ContextMenuInfo;
import android.view.View.OnCreateContextMenuListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.example.ece381.*;

public class SongActivity extends Activity {
  
  private ListView songListView;
  private ArrayAdapter<String> listAdapter;
  
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
    final Database db = new Database();
    
    // Testing: add a playlist into the database
    //Song testsong = new Song("Song1.wav");
    //db.addSong(testsong);
    
    Song[] pl = db.getSongArray();
    
    // iterate thru playlist array; retrieve the names of each playlist, and add it to arraylist
    for(int i = 1; i < db.getNumSongs()+1; i++ ) {
		   song_names.add(pl[i].getSongName());
    }
     
    song_names.add("Add a song to this playlist...");
    
    // Create ArrayAdapter using the planet list.
    listAdapter = new ArrayAdapter<String>(this, R.layout.simplerow, song_names);    
    
    // On click of an item
    songListView.setOnItemClickListener(new OnItemClickListener() {
    	@Override
    	public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
    		
    		// Show a toast box with the name of the item
		    Toast.makeText(getApplicationContext(), ((TextView) view).getText(), Toast.LENGTH_SHORT).show();
		    
		    // Replace with another intent to music loader
    	}
    });
    

    // On long press, create a context menu window
    songListView.setOnCreateContextMenuListener(new OnCreateContextMenuListener() {
    	
    	@Override
        public void onCreateContextMenu(ContextMenu menu, View v, ContextMenuInfo menuInfo) {
    		
    		if(db.getNumSongs() > 0) {
    			menu.setHeaderTitle("Options"); 
    			menu.add(0, 1, 0, "Play song");
    			menu.add(0, 2, 0, "Delete song"); 
    			menu.add(0, 3, 0, "Properties");
    		}
        }
    }); 

    // Set the ArrayAdapter as the ListView's adapter.
    songListView.setAdapter( listAdapter );      
    
    
    
  }
}