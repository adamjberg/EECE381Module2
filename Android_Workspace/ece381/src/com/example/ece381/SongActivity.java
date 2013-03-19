package com.example.ece381;

import java.util.ArrayList;

import android.app.Activity;
import android.content.Intent;
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
  
    // Create an arraylist of strings to display onto the Adapter
    ArrayList<String> song_names = new ArrayList<String>();
   
    // create an instance of the database
    final Database db = new Database();
	
  private ListView songListView;
  private ArrayAdapter<String> listAdapter;
  
  public final String addsong = "Add a song to this playlist...";
  
  /** Called when the activity is first created. */
  @Override
  public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_song);

    
    // Find the ListView resource. 
    songListView = (ListView) findViewById( R.id.songListView );

    // Create ArrayAdapter using the planet list.
    listAdapter = new ArrayAdapter<String>(this, R.layout.simplerow, song_names);    
    
	refreshSonglist();
    
    // On click of an item
    songListView.setOnItemClickListener(new OnItemClickListener() {
    	@Override
    	public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
    		
    		// Show a toast box with the name of the item
		   // Toast.makeText(getApplicationContext(), ((TextView) view).getText(), Toast.LENGTH_SHORT).show();

    		Intent intent = null;
    		String selected = (String) (songListView.getItemAtPosition(position));

    		// Go to AllSongsListActivity
    		if(selected == addsong ) {
       			intent =  new Intent(getApplicationContext(), AllSongsListActivity.class);
    			startActivityForResult(intent, 2); 
    		}

    		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    		//TODO: Add the intent to play music here
    		
    		// Play music
    		else {
    			// Pop up a toast message that says "Playing <item name>"
    			Toast.makeText(getApplicationContext(), "Playing "+((TextView) view).getText(), Toast.LENGTH_SHORT).show();
    		}
    	}
    });
    

    // On long press, create a context menu window
    songListView.setOnCreateContextMenuListener(new OnCreateContextMenuListener() {
    	
    	@Override
        public void onCreateContextMenu(ContextMenu menu, View v, ContextMenuInfo menuInfo) {
    		
    		if(db.getTotalSongs() > 0) {
    			menu.setHeaderTitle("Options"); 
    			menu.add(0, 1, 0, "Play song");
    			menu.add(0, 2, 0, "Delete song from playlist"); 
    			menu.add(0, 3, 0, "Properties");
    		}
        }
    }); 

    // Set the ArrayAdapter as the ListView's adapter.
    songListView.setAdapter( listAdapter );      
    
    
    
  }
  protected void onActivityResult(int requestCode, int resultCode, Intent data ) {

	  refreshSonglist();
	  
  }
  
  public void onResume() {
	  super.onResume();
	  refreshSonglist();
  }
  
  
  public void refreshSonglist() {
	  	listAdapter.clear();
	    // "add a song" button
	  	listAdapter.add(addsong);
	    
	  	// Current playlist ID
	  	int curr_plid = db.getCurr_playlist_id();
	  	int ids[] = db.getSongsFromList(curr_plid);
	  	// Add the array to listAdapter
	  	for(int i = 1; i <= db.getTotalSongs(); i++){
		  	if(	ids[i] > 0 )
		  		listAdapter.add(""+ids[i]);
	  	}

	    listAdapter.notifyDataSetChanged();
  }
  
}