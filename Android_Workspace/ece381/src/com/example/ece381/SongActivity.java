package com.example.ece381;

import java.util.ArrayList;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.util.Log;
import android.view.ContextMenu;
import android.view.ContextMenu.ContextMenuInfo;
import android.view.View;
import android.view.View.OnCreateContextMenuListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

public class SongActivity extends Activity {
  
    // Create an arraylist of strings to display onto the Adapter
    ArrayList<String> song_names = new ArrayList<String>();
    
    private Communication com = Communication.getInstance();

    // create an instance of the database
    private Database db = com.getDB();
  
  private ListView songListView;
  private ArrayAdapter<String> listAdapter;
  
  private String selected_pl_name = null;
  private String songPicked = "";
  
  public final String addsong = "Add a song to this playlist...";
  
  /** Called when the activity is first created. */
  @Override
  public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_song);

    // Get the passed in playlist name
    selected_pl_name = getIntent().getExtras().getString("selected_pl_name");
    Log.v("selected_pl_name", selected_pl_name);
    
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
       			// selected playlist name
    			intent.putExtra("selected_pl_name", selected_pl_name);
       			startActivityForResult(intent, 2);
    		}

    		// Play music
    		else {
    			// Pop up a toast message that says "Playing <item name>"
    			Toast.makeText(getApplicationContext(), "Playing "+((TextView) view).getText(), Toast.LENGTH_SHORT).show();
    			
    			int plid, songid;
    			// Set current playlist 
     			plid = db.queryListByName(selected_pl_name);
        		songid = db.getSongsFromList(plid)[position]; // get song id from playlist
        		Log.v("clicked songid", songid+"");
        		
    			db.setCurr_song_id(songid);
    			db.setCurr_playlist_id(plid);
        		
    			//Log.v("setting current playlist", selected_pl_name);
    			
    			Intent goToPlay = new Intent(SongActivity.this, play.class);
    		
    			Command.syncPlay(db.getCurr_song_id(), 1, 0);
    			startActivity(goToPlay);
    				
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
	    
	  	// Add the songs of the playlist to the adapter
	  	
	  	int listid = db.queryListByName(selected_pl_name);
	  	
	  	if(db.getPlaylists()[listid].getNum_of_songs() > 0) {
	  	
	  		for(int i = 1; i <= db.getPlaylists()[listid].getNum_of_songs(); i++) {
	  			
	  			if(db.getSongsFromList(listid)[i] > 0) {
		  			listAdapter.add(String.format("%d",db.getSongsFromList(listid)[i]) );
		  		}
	  		}
	  	}
	  	
	  	// refresh adapter
	    listAdapter.notifyDataSetChanged();
  }
}