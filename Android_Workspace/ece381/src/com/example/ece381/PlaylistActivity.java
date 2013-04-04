package com.example.ece381;

import java.util.ArrayList;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
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

public class PlaylistActivity extends Activity {
  
  private Communication com = Communication.getInstance();

  // create an instance of the database
  private Database db = com.getDB();
  
  // Create an arraylist of strings to display onto the Adapter
  ArrayList<String> playlist_names = new ArrayList<String>();
  
  private ListView mainListView ;
  private ArrayAdapter<String> listAdapter ;
 
  private String selectedWord;
  
  // Create a ProgressDialog (for delete/rename playlists)
  private ProgressDialog pd = null;
  
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

    // Instantiate the progress dialog
    pd = new ProgressDialog(this);
    pd.setMessage("Updating database - please wait");
    pd.setProgressStyle(ProgressDialog.STYLE_SPINNER);

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
    			int plid = db.queryListByName(selected);
    			db.setSelectedList(plid);
    			Command.syncOpenSongsFromList(plid);
    			//Command.syncSelectList(db.queryListByName(selected));
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
  
    		AdapterView.AdapterContextMenuInfo info =
    		            (AdapterView.AdapterContextMenuInfo) menuInfo;
    		
    		selectedWord = ((TextView) info.targetView).getText().toString();

    		// set database selected playlist
    		db.setSelectedList(info.position);
    		
    		menu.setHeaderTitle("Options for "+selectedWord);
            menu.add(0, 1, 0, "Rename playlist");
            //menu.add(0, 2, 0, "Delete playlist"); 
        }
    }); 

    // Set the ArrayAdapter as the ListView's adapter.
    mainListView.setAdapter( listAdapter );      
  }
  /* End of onCreate */
  
  // Handle context menu actions
  @Override
  public boolean onContextItemSelected(MenuItem item) {
	  
	  // get the menu position
	  AdapterView.AdapterContextMenuInfo info = (AdapterView.AdapterContextMenuInfo)item.getMenuInfo();
	  int position = info.position;

	  if(item.getTitle() == "Rename playlist") {
		  Intent intent = new Intent(getApplicationContext(), PlaylistBuilderActivity.class);
		  intent.putExtra("action", "rename");
		  startActivity(intent);
	  }
	  /*else if(item.getTitle() == "Delete playlist") {
		  HandleDeletePlaylist(position);
	  }*/
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
		Intent intent;
		switch (item.getItemId()) {
		case android.R.id.home:
			finish();
			// NavUtils.navigateUpFromSameTask(this);
			// onBackPressed();
			return true;
		case R.id.songs:
			intent = new Intent(PlaylistActivity.this, MainActivity.class);
			startActivity(intent);
			return true;
		case R.id.menu_settings:
			intent = new Intent(PlaylistActivity.this, MixerActivity.class);
			startActivity(intent);
			return true;
		case R.id.playlists:
			Toast.makeText(PlaylistActivity.this, "Already in Playlist.",
					Toast.LENGTH_LONG).show();
			return true;
		case R.id.playMenu:
			intent = new Intent(PlaylistActivity.this, play.class);
			startActivity(intent);
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

  // Handler functions for context menus
  
  /*
  public void HandleDeletePlaylist(int id) {
	  
	  
	  // Remove the selected item from the ArrayList
		  String toRemove = listAdapter.getItem(id);
		  Log.v("listToDelete", toRemove );
		  
		  int plid = db.queryListByName(toRemove);
		  
		  // Only handle deleting if playlist id isn't 0 
		  if(plid > 0) {
			  
			  for(int i = 1; i < db.getTotalSongs()+1; i++) {
				  // Remove all songs from list
				  //Command.removeSongFromList(plid, i);
				  Command.syncRemoveSongFromList(plid, i); 
			  }
			  
			  // If it's the current playlist, set current to 0
			  if( db.getCurr_playlist_id() == plid ) {
				  db.setCurr_playlist_id(0);
				  db.setCurr_song_id(0);
			  }
			  Command.syncRemoveList(plid);
			  db.removeList(plid);
			  listAdapter.remove(toRemove);
			  
			  // Delay for 2 seconds

			  showDelayDialog();
			  refreshPlaylists();
		  }
  }
  
  */
  
  public void onResume() {
	  	Log.v("PlaylistAcitivty Resume", "");
	  	Command.syncOpenPlaylistsPanel();
	  	super.onResume();
	  	refreshPlaylists();
}
  
  protected void onActivityResult(int requestCode, int resultCode, Intent data ) {
	  	refreshPlaylists();
  }

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
  
  public void showDelayDialog() {
	  pd.show();
	  	Handler handler = new Handler();
	  	handler.postDelayed(new Runnable() {
	  		public void run() {
	  			pd.dismiss();
	  		}}, 2000);
  }
}