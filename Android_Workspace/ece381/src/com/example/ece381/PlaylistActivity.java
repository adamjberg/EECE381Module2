package com.example.ece381;

import java.util.ArrayList;

import android.app.Activity;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.ContextMenu;
import android.view.ContextMenu.ContextMenuInfo;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.view.View.OnCreateContextMenuListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

import com.example.ece381.Communication;
import com.example.ece381.Database;
import com.example.ece381.Playlist;
import com.example.ece381.Song;

public class PlaylistActivity extends Activity {
  
  private Communication com = Communication.getInstance();

  // create an instance of the database
  private Database db = com.getDB();
  
  // Create an arraylist of strings to display onto the Adapter
  ArrayList<String> playlist_names = new ArrayList<String>();
  
  private ListView mainListView ;
  private ArrayAdapter<String> listAdapter ;
  
  
  /** Called when the activity is first created. */
  @Override
  public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
		 getWindow().requestFeature(Window.FEATURE_ACTION_BAR);
	 }
    setContentView(R.layout.activity_playlist);
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
        // Show the Up button in the action bar.
        this.getActionBar().setDisplayHomeAsUpEnabled(true);
    }
    // Find the ListView resource. 
    mainListView = (ListView) findViewById( R.id.mainListView );
    
    
    // Create ArrayAdapter using the play list names.
    listAdapter = new ArrayAdapter<String>(this, R.layout.simplerow, playlist_names); 

//testing
//	for(int j = 1; j < 4; j++) {
//		db.addSong(new Song(String.format("Test%d", j)));
//	}   
	
	// Add elements to ArrayAdapter
	refreshPlaylists();

    // On click of an item:
    mainListView.setOnItemClickListener(new OnItemClickListener() {
    	@Override
    	public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
    		
    		Intent intent = null;
    		String selected = (String) (mainListView.getItemAtPosition(position));
    		
    		// Go to PlaylistBuilderActivity
    		if(selected == "Create a new playlist" ) {
    			intent = new Intent(getApplicationContext(), PlaylistBuilderActivity.class);
    			intent.putExtra("action", "create");
    		}
    		
    		// Go to SongActivity 
    		else {
    			// Make an intent and start the Activity to view the songs of the playlist
    			int lid = db.queryListByName(selected);
    			Command.selectList(lid);
    			Command.syncOpenSongsFromList(lid);
    			intent = new Intent(getApplicationContext(), SongActivity.class);
    			intent.putExtra("selected_pl_name", selected);
    		}
			startActivity(intent); // get the playlist id back
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
	  
  @Override
  public boolean onCreateOptionsMenu(Menu menu) {
  	getMenuInflater().inflate(R.menu.activity_main, menu);
  	return true;
  }

  @Override
  public boolean onOptionsItemSelected(MenuItem item) {
   // TODO Auto-generated method stub
   switch(item.getItemId()){
   case android.R.id.home:
  	 finish();
      // NavUtils.navigateUpFromSameTask(this);
  	 // onBackPressed();
       return true;
   case R.id.songs:
	  Intent intentsong = new Intent(PlaylistActivity.this, MainActivity.class);
	  intentsong.putExtra("subActivity", "true");
	  startActivity(intentsong);
  	  return true;
   case R.id.menu_settings:
  	  Toast.makeText(PlaylistActivity.this,
  	    item.getTitle(),
  	     Toast.LENGTH_LONG).show();
  	  return true;
   case R.id.playlists:
	   Toast.makeText(PlaylistActivity.this,
 			    "Already in Playlist.",
 			    Toast.LENGTH_LONG).show();   
  	  return true;
   case R.id.playMenu:
	  Intent intent = new Intent(PlaylistActivity.this, play.class);
	  startActivity(intent);   
  	  return true;
   }
   return super.onOptionsItemSelected(item);
  } 
  // Handler functions for context menus
  public void HandlePlayPlaylist(int id) {
	  // Do something
	  Toast.makeText(this, "Playing the playlist", Toast.LENGTH_SHORT).show();
  }
  
  public void HandleDeletePlaylist(int id) throws IndexOutOfBoundsException  {
	
	  // Remove the selected item from the ArrayList
		  String toRemove = listAdapter.getItem(id);
		 
		  int plid = db.queryListByName(toRemove);
		  int num_of_songs = db.getPlaylists()[plid].getNum_of_songs();
		  // Only handle deleting if playlist id isn't 0 
		  if(plid > 0) {
			  
			  for(int i = 1; i < num_of_songs; i++) {
				  // Remove all songs from list
				  Command.syncRemoveSongFromList(plid, i); 
				  
				  // Set current playlist id and songid to 0
			  }
			  //Command.removeList(plid);
			  Command.syncRemoveList(plid);
			  Command.syncSelectList(0);
			  listAdapter.remove(listAdapter.getItem(id));
			  
		  }
		  else {
			  return;
		  }
  }
  
  
  public void onResume() {
	  	Log.v("PlaylistAcitivty Resume", "");
	  	super.onResume();
	  	refreshPlaylists();
	  	Command.syncOpenPlaylistsPanel();
}
  
  protected void onActivityResult(int requestCode, int resultCode, Intent data ) {
	  	refreshPlaylists();
  }
  
  /*
   * Gets the playlist id (plid) of the selected listview item
   *//*
  public int getSelectedPlaylistId(String selectedValue) {
	   int plid = 0;
	   
	   for(int i = 1; i < db.getNumLists()+1; i++) {
		   if(selectedValue == db.getPlaylists()[i].getListName() )
			   plid = i;
	   }
	   
	   Log.d("current plid: ", ""+plid);
	   return plid;
  }
  */
  public int getSelectedSongId(String selectedValue) {
	  int songid = 0;
	   for(int i = 1; i < db.getTotalSongs()+1; i++) {
		   if(selectedValue == db.getSongs()[i].getSongName() )
			   songid = i;
	   }
	  // Log.d("current songid: ", ""+songid);
	   return songid;
  }
  
  public void refreshPlaylists() {
	  	listAdapter.clear();
	  	listAdapter.add("Create a new playlist");
	  	listAdapter.addAll(db.getListsName());
	    listAdapter.notifyDataSetChanged();
  }
  
}