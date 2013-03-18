package com.example.playlistactivity;

import java.util.ArrayList;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.ContextMenu;
import android.view.MenuItem;
import android.view.ContextMenu.ContextMenuInfo;
import android.view.View;
import android.view.View.OnCreateContextMenuListener;
import android.widget.AdapterView;
import android.widget.AdapterView.AdapterContextMenuInfo;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

import com.example.ece381.*;

public class PlaylistActivity extends Activity {
  
  private Communication com = Communication.getInstance();

  // create an instance of the database
  private Database db = com.getDB();
  
  // Create an arraylist of strings to display onto the Adapter
  ArrayList<String> playlist_names = new ArrayList<String>();
  
  private ListView mainListView ;
  private ArrayAdapter<String> listAdapter ;
  
  private boolean initialize = false;
  
  /** Called when the activity is first created. */
  @Override
  public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_playlist);
    
    // Find the ListView resource. 
    mainListView = (ListView) findViewById( R.id.mainListView );
    
    // Testing: add a playlist into the database
    // Playlist testpl = new Playlist("New Playlist"); 
    // db.addList(testpl); 
    
    // get the database playlist array
    Playlist[] pl = db.getPlaylistArray();

    // iterate thru playlist array; retrieve the names of each playlist, and add it to arraylist
    for(int i = 1; i < db.getNumLists()+1; i++ ) {
		   playlist_names.add(pl[i].getListName());
    }
     
    // add a placeholder to create a new playlist
    playlist_names.add("Create a new playlist");
    
    // Create ArrayAdapter using the play list names.
    listAdapter = new ArrayAdapter<String>(this, R.layout.simplerow, playlist_names); 
    

    // On click of an item:
    mainListView.setOnItemClickListener(new OnItemClickListener() {
    	@Override
    	public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
    		
    		Intent intent = null;
    		String selected = (String) (mainListView.getItemAtPosition(position));
    		
    		// Go to PlaylistBuilderActivity
    		if(selected == "Create a new playlist" ) {
    			intent = new Intent(getApplicationContext(), PlaylistBuilderActivity.class);
    		}
    		
    		// Go to SongActivity
    		else {
    			// Make an intent and start the Activity to view the songs of the playlist
    			intent =  new Intent(getApplicationContext(), SongActivity.class);
    		}
    		
			startActivity(intent);    	

    	}
  
    } );
    

    // On long press, create a context menu window
    mainListView.setOnCreateContextMenuListener(new OnCreateContextMenuListener() {
    	
    	@Override
        public void onCreateContextMenu(ContextMenu menu, View v, ContextMenuInfo menuInfo) {
  
            menu.setHeaderTitle("Options"); 
            menu.add(0, 1, 0, "Play playlist");
            menu.add(0, 2, 0, "Delete playlist"); 
        }
    }); 

    // Set the ArrayAdapter as the ListView's adapter.
    mainListView.setAdapter( listAdapter );      
    
    
    
  }
  // Handle context menu actions
  @Override
  public boolean onContextItemSelected(MenuItem item) {
	  
	  // get the menu position
	  AdapterView.AdapterContextMenuInfo info = (AdapterView.AdapterContextMenuInfo)item.getMenuInfo();
	  int position = info.position;
	  
	  if(item.getTitle() == "Play playlist") {
		  HandlePlayPlaylist(item.getItemId());
	  }
	  else if(item.getTitle() == "Delete playlist") {
		  HandleDeletePlaylist(position);
	  }
	  else
		  return false;
	  
	  return true;

  }
	  
  
  // Handler functions for context menus
  public void HandlePlayPlaylist(int id) {
	  // Do something
	  Toast.makeText(this, "Playing the playlist", Toast.LENGTH_SHORT).show();
  }
  
  public void HandleDeletePlaylist(int id) throws IndexOutOfBoundsException  {
	  // Do something
	  Toast.makeText(this, "Playlist deleted", Toast.LENGTH_SHORT).show();
	  /*	
	  // Remove the selected item from the ArrayList
	  try {
		  String toRemove = listAdapter.getItem(id);

	//	  Playlist[] pl = db.getPlaylistArray();
		  
		  for(int i = 1; i < db.getNumLists()+1; i++ ) {
			  if(playlist_names.get(i) == toRemove) 
				  playlist_names.remove(i);
			//  	  db.removeList(pl[i]);
		  }
	  }
	  catch(IndexOutOfBoundsException e) {
		  System.err.println("IndexOutOfBoundsException: " + e.getMessage());
		  throw new IndexOutOfBoundsException();
	  }
	
	  */
	  // Refresh adapter after deleting
	listAdapter.remove(listAdapter.getItem(id));
  }
  
  public void HandleGoToSongList(int id) {
	  
  }
}