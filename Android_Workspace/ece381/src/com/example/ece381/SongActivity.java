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
import android.view.View.OnCreateContextMenuListener;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

public class SongActivity extends Activity {
  
    // Create an arraylist of strings to display onto the Adapter
    ArrayList<String> song_names = new ArrayList<String>();
    
    private Communication com = Communication.getInstance();

    // create an instance of the database
    private Database db = com.getDB();
  
    // repeat toggle button
    private ToggleButton repeatBtn;
    
    
  private ListView songListView;
  private ArrayAdapter<String> listAdapter;
  
  private String selected_pl_name = null;
  private String songPicked = "";
  
  public final String addsong = "Add a song to this playlist...";
  
  public int lastItem = 0; // count of how many items in the ArrayAdapter; 
	   //= 1 if there are no songs due to the "add a song" item

  
  /** Called when the activity is first created. */
  @Override
  public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
		 getWindow().requestFeature(Window.FEATURE_ACTION_BAR);
	 }
    setContentView(R.layout.activity_song);
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
        // Show the Up button in the action bar.
        this.getActionBar().setDisplayHomeAsUpEnabled(true);
    }
    // Get the passed in playlist name
    selected_pl_name = getIntent().getExtras().getString("selected_pl_name");
    
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
        	
				Command.syncPlay(com.getDB().getCurr_song_id(), com.getDB().getSongs()[com.getDB().getCurr_song_id()].getVolume(), 0);
				
    			Intent goToPlay = new Intent(SongActivity.this, play.class);
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
    
    // Repeat button
    repeatBtn = (ToggleButton) findViewById(R.id.repeat_button);
  }
  protected void onActivityResult(int requestCode, int resultCode, Intent data ) {
	  refreshSonglist();
  }
  
 public void onResume() {
	  super.onResume();
	  refreshSonglist();
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
		  Intent intentsong = new Intent(SongActivity.this, MainActivity.class);
		  startActivity(intentsong);
	 	  return true;
	  case R.id.menu_settings:
	 	  Toast.makeText(SongActivity.this,
	 	    item.getTitle(),
	 	     Toast.LENGTH_LONG).show();
	 	  return true;
	  case R.id.playlists:
		  finish(); 
	 	  return true;
	  case R.id.playMenu:
		  Intent intent = new Intent(SongActivity.this, play.class);
		  startActivity(intent);   
	 	  return true;
	  }
	  return super.onOptionsItemSelected(item);
  } 
  public void refreshSonglist() {
	  	listAdapter.clear();
	    // "add a song" button
	  	listAdapter.add(addsong);
	  	listAdapter.addAll(db.querySongsBylist(db.getCurr_playlist_id()));
	  	// set the current last position (end of the list)
	  	this.lastItem = listAdapter.getCount(); 

	  	/*
	  	 * item 0 = "add a song"
	  	 * item 1 ... n = songs
	  	 */
	  	
	  	Log.v("songcount", ""+(this.lastItem-1));
	  	
	  	// refresh adapter
	    listAdapter.notifyDataSetChanged();
  }
  
  public void repeatPlaylistToggle(View view) {
	  // set db flag to reflect if it's toggled on or off
	  db.setRepeatPlaylist( ((ToggleButton) view).isChecked() );
	  Log.v("toggledRepeatPlaylist", ""+db.getRepeatPlaylistValue());
  }
  
}