package com.example.ece381;

import java.util.ArrayList;
import java.util.Random;

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
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;
//import com.example.ece381.Playlist;
public class SongActivity extends Activity {
  
    // Create an arraylist of strings to display onto the Adapter
    ArrayList<String> song_names = new ArrayList<String>();
    
    private Communication com = Communication.getInstance();

    // create an instance of the database
    private Database db = com.getDB();
  
    // repeat toggle button
    private ToggleButton repeatBtn;
    
    // Progress dialog
    private ProgressDialog pd = null;
    
  private ListView songListView;
  private ArrayAdapter<String> listAdapter;
  
  private String selected_pl_name = null;
  private String songPicked = "";
  
  public final String addsong = "Add a song to this playlist...";
  
  // TouchInterceptor
  private TouchInterceptor mList;
  
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
    repeatBtn = (ToggleButton) findViewById( R.id.repeat_button );
    repeatBtn.setChecked(db.getRepeatPlaylistValue());
    // Get the passed in playlist name
    selected_pl_name = getIntent().getExtras().getString("selected_pl_name");
    
    // Find the ListView resource. 
    songListView = (ListView) findViewById( R.id.songListView );

    // Create ArrayAdapter using the planet list.
    listAdapter = new ArrayAdapter<String>(this, R.layout.drag_drop_row, song_names);    

    // Instantiate the progress dialog
    pd = new ProgressDialog(this);
    pd.setMessage("Updating database - please wait");
    pd.setProgressStyle(ProgressDialog.STYLE_SPINNER);
    
    
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
    			Command.syncSelectList(plid);
        	
				//Command.syncPlay(com.getDB().getCurr_song_id(), com.getDB().getSongs()[com.getDB().getCurr_song_id()].getVolume(), 0);
    			Command.syncPlaySongFromList(songid, plid);
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
	
	// TouchInterceptor
	mList = (TouchInterceptor) songListView;
    mList.setDropListener(mDropListener);
    registerForContextMenu(mList);
  }
  protected void onActivityResult(int requestCode, int resultCode, Intent data ) {
	  refreshSonglist();
  }
  
 public void onResume() {
	  refreshSonglist();
	  super.onResume();
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
			intent = new Intent(SongActivity.this, MainActivity.class);
			intent.putExtra("subActivity", "true");
			startActivity(intent);
			return true;
		case R.id.menu_settings:
			intent = new Intent(SongActivity.this, MixerActivity.class);
			startActivity(intent);
			return true;
		case R.id.playlists:
			finish();
			return true;
		case R.id.playMenu:
			intent = new Intent(SongActivity.this, play.class);
			startActivity(intent);
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

  public void refreshSonglist() {
	  	listAdapter.clear();
	    // "add a song" button
	  	listAdapter.add(addsong);
	  	listAdapter.addAll(db.querySongsBylist(db.getSelectedList()));
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
  public void onShuffle(View view ){
	  String[] x=db.querySongsBylist(db.getSelectedList()); 
	  shuffleArray(x);
	  
	  for(int j = 0; j < x.length; j++)
	  Log.v("songorder", x[j]);
	  
		 int i=1;
		
		 while (x.length >= i) {
			 Command.syncRemoveSongFromList(db.getSelectedList(),db.querySongByName(x[i-1]));
			 // Command.syncAddSongToList(db.getCurr_playlist_id(),db.querySongByName(x[i-1]));
			 i++;
		 }
		 int k=1;
		 while (x.length >= k){
			// Command.syncRemoveSongFromList(db.getCurr_playlist_id(),db.querySongByName(x[i-1]));
			 Command.syncAddSongToList(db.getSelectedList(),db.querySongByName(x[k-1]));
			 k++;
		 }
		// listAdapter = new ArrayAdapter<String>(this, R.layout.simplerow, song_names);    
		 
		// listAdapter = new ArrayAdapter<String>(this, R.layout.simplerow, song_names); 
		 Command.syncOpenSongsFromList(db.getSelectedList());
		
		 showDelayDialog(x.length);
		
	 }
  public static void shuffleArray(String[] ar)
  {
    Random rnd = new Random();
    for (int i = ar.length - 1; i >= 0; i--)
    {
      int index = rnd.nextInt(i + 1);
      // Simple swap
      String a = ar[index];
      //int x= f[index];
      ar[index]=ar[i];
     // f[index]=f[i];
      ar[i]= a;
      //f[i]=x;
    }
  }
  
  public void repeatPlaylistToggle(View view) {
	  // set db flag to reflect if it's toggled on or off
	  db.setRepeatPlaylist( ((ToggleButton) view).isChecked() );
	  Command.syncRepeatList(db.getCurr_playlist_id());
   }
  
  public void showDelayDialog(int numSongs) {
	  pd.show();
	  
	  int effective_delay = 100*numSongs; // 2 seconds per song
	  
	  	Handler handler = new Handler();
	  	handler.postDelayed(new Runnable() {
	  		public void run() {
	  			pd.dismiss();
	  			
	  			 refreshSonglist();
	  		}}, effective_delay);
  }

// TouchInterceptor for drag and drop
private TouchInterceptor.DropListener mDropListener =
		    new TouchInterceptor.DropListener() {
		        public void drop(int from, int to) {
		            System.out.println("Droplisten from:"+from+" to:"+to);

		            if(from == 0 || to == 0) {
		            	return;
		            }
		            
		            //Assuming that item is moved up the list
		            int loop_start = from;
		            int loop_end = to;
		            String[] temp = new String[ db.querySongsBylist(db.getSelectedList()).length +1 ];
		            temp = db.querySongsBylist(db.getSelectedList());
		            
		            String target = temp[from-1];
		            Log.v("target", temp[from-1]);
		            
		            //For instance where the item is dragged down the list
		            if(from < to) { // go down the list
			            for(int i = loop_start-1; i < loop_end-1; i++ ) {
			            	temp[i] = temp[i+1];
			            }
		            }
		            else { // go up the list
		            	for(int i = loop_start-1; i > loop_end-1; i--) {
		            		temp[i] = temp[i-1];
		            	}
		            }


		            temp[to-1] = target;
		        
		            showDelayDialog(temp.length);
		            
		            // android side
		            int j = 1;
		            while(temp.length >= j) {
		            	int id = db.querySongByName(temp[j-1]);
		            	Command.syncRemoveSongFromList(db.getSelectedList(), id);
		            //	Log.v("removeiteration", ""+j);
		            	j++;
		            }
		            
		            int k = 1;
		            while(temp.length >= k) {
		            	int id2 = db.querySongByName(temp[k-1]);
		            	Command.syncAddSongToList(db.getSelectedList(), id2);
		            //	Log.v("addIteration", ""+k);
		            	k++;
		            }
		            
		            
		            Command.syncOpenSongsFromList(db.getSelectedList());
		            showDelayDialog(temp.length);
					
		        }
		    };
  
  
}