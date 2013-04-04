package com.example.ece381;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.StrictMode;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.SimpleAdapter;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ViewSwitcher;

import com.example.ece381.Communication.Stats;

public class MainActivity extends Activity {

	private Timer tcp_timer;
	private Communication com = Communication.getInstance();
	//creates a ViewSwitcher object, to switch between Views
	private ViewSwitcher viewSwitcher;
	
	private TextView textview;
	private ListView m_listview;
	private boolean isSubActivity;
	@Override
	public void onCreate(Bundle savedInstanceState) {
		
		// This call will result in better error messages if you
		// try to do things in the wrong thread.
		StrictMode.setThreadPolicy(new StrictMode.ThreadPolicy.Builder()
				.detectDiskReads().detectDiskWrites().detectNetwork()
				.penaltyLog().build());

		super.onCreate(savedInstanceState);

		
		 if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
			 getWindow().requestFeature(Window.FEATURE_ACTION_BAR);
			 this.getActionBar().hide();
		 }
		 
		Bundle extras = getIntent().getExtras();
		if(extras == null) {
			this.isSubActivity = false;
			TCPReadTimerTask tcp_task = new TCPReadTimerTask();
			tcp_timer = new Timer();
			tcp_timer.schedule(tcp_task, 5000, 50);
			
			new SocketConnect().execute((Void) null);
			
			//Initialize a LoadViewTask object and call the execute() method
	    	new LoadViewTask().execute();    	
		} else {
			setContentView(R.layout.activity_main);addListView();  

			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
		         // Show the Up button in the action bar.
		         MainActivity.this.getActionBar().setDisplayHomeAsUpEnabled(true);
		         MainActivity.this.getActionBar().show();
		    }
			
			this.isSubActivity = true;
		}
	}

	@Override
	public void onResume() {
		  Command.syncOpenAllSongPanel();
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
			Toast.makeText(MainActivity.this, "Already in song list",
					Toast.LENGTH_LONG).show();
			return true;
		case R.id.menu_settings:
			intent = new Intent(MainActivity.this, MixerActivity.class);
			startActivity(intent);
			return true;
		case R.id.playlists:
			intent = new Intent(MainActivity.this, PlaylistActivity.class);
			startActivity(intent);
			return true;
		case R.id.playMenu:
			// com.getDB().setCurr_playlist_id(0);
			intent = new Intent(MainActivity.this, play.class);
			startActivity(intent);
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

	@Override
	public void finish() {
		if(!this.isSubActivity) {
			tcp_timer.cancel();
			closeSocket();
			Log.i("LOG", "Socket Closed");
			com.resetCom();
		}
		super.finish();
	}
	
	 //Override the default back key behavior
    @Override
    public void onBackPressed() 
    {
    	//Emulate the progressDialog.setCancelable(false) behavior
    	//If the first view is being shown
    	if(viewSwitcher.getDisplayedChild() == 0)
    	{
    		//Do nothing
    		return;
    	}
    	else
    	{
    		//Finishes the current Activity
    		super.onBackPressed();
    	}
    }

	//  Called when the user wants to send a message
	
	public void sendMessage(View view) {
		
		// Get the message from the box
		
		EditText et = (EditText) findViewById(R.id.MessageText);
		String msg = et.getText().toString();
		com.send(msg);
	}

	// Called when the user closes a socket
	
	public void closeSocket(View view) {
		//closeSocket();
		EditText et = (EditText) findViewById(R.id.MessageText);
		String msg = et.getText().toString();
		Command.syncCreatePlaylist(msg);
	}

	public void closeSocket() {

		MyApplication app = (MyApplication) getApplication();
		Socket s = app.sock;
		try {
			s.getOutputStream().close();
			s.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	private void addListView(){
		
		
		 m_listview = (ListView) findViewById(R.id.listView1);
		 
		  	    //}
		  List<Map<String, String>> data = new ArrayList<Map<String, String>>();
		  for (int i = 1; i <=  com.getDB().getTotalSongs(); i++  ) {
		      Map<String, String> datum = new HashMap<String, String>(2);
		      datum.put("songname", "Song Name: "+com.getDB().getSongs()[i].getSongName());
		      datum.put("length", "Song Length: "+ com.getDB().getSongs()[i].getSize()/1000);
		      data.add(datum);
		  }
		   //String[] from = { "songname","length",};
		   //int[] to = { R.id.txt,R.id.cur};
		  SimpleAdapter adapter = new SimpleAdapter(this, data,
		                                            R.layout.row_layout,
		                                            new String[] {"songname", "length"},
		                                            new int[] { R.id.txt,R.id.cur});
		  	    
		  	    
		 // m_listview.setAdapter(adapter);
		    
		 //   ArrayAdapter<String> adapter2 =
		  	//      new ArrayAdapter<String>(this, android.R.layout.simple_list_item_2, items);
		    //setListAdapter(new ArrayAdapter<String>(this, R.layout.simple_list_item_1,items));
		 
		    m_listview.setAdapter(adapter);
		 //   m_listview.setAdapter(adapter2);
		    m_listview.setOnItemClickListener(new OnItemClickListener() {
	         @Override
	         public void onItemClick(AdapterView<?> parent, View view, int position,
	                 long id) {
	        	 com.getDB().setCurr_song_id(position+1);
	        	 Command.syncSelectList(0);
	        	 Command.syncPlaySongFromAllSongs(com.getDB().getCurr_song_id(),  com.getDB().getSongs()[com.getDB().getCurr_song_id()].getVolume(), 0);
	   			 //Command.syncPlay(com.getDB().getCurr_song_id(), com.getDB().getSongs()[com.getDB().getCurr_song_id()].getVolume(), 0);
	         	 Intent i = new Intent(MainActivity.this,play.class);
	         	// String item = ((TextView)view).getText().toString();
	         	 //i.putExtra("USERNAME", item);
	         	 startActivity(i);
	             //String item = ((TextView)view).getText().toString();
	             
	            // Toast.makeText(getBaseContext(), item, Toast.LENGTH_LONG).show();
	             
	         }
	     });
		
	}
	 //To use the AsyncTask, it must be subclassed
    private class LoadViewTask extends AsyncTask<Void, Integer, Void>
    {
    	//A TextView object and a ProgressBar object
    	private TextView tv_progress;
    	private ProgressBar pb_progressBar;
    	
    	//Before running code in the separate thread
		@Override
		protected void onPreExecute() 
		{
			//Initialize the ViewSwitcher object
	        viewSwitcher = new ViewSwitcher(MainActivity.this);
	        /* Initialize the loading screen with data from the 'loadingscreen.xml' layout xml file. 
	         * Add the initialized View to the viewSwitcher.*/
			viewSwitcher.addView(ViewSwitcher.inflate(MainActivity.this, R.layout.loadingscreen, null));
			
			//Initialize the TextView and ProgressBar instances - IMPORTANT: call findViewById() from viewSwitcher.
			tv_progress = (TextView) viewSwitcher.findViewById(R.id.tv_progress);
			pb_progressBar = (ProgressBar) viewSwitcher.findViewById(R.id.pb_progressbar);
			//Sets the maximum value of the progress bar to 100 			
			pb_progressBar.setMax(100);
			
			//Set ViewSwitcher instance as the current View.
			setContentView(viewSwitcher);
			
		}

		//The code to be executed in a background thread.
		@Override
		protected Void doInBackground(Void... params) 
		{
				
				//Get the current thread's token
				synchronized (this) 
				{
					//Initialize an integer (that will act as a counter) to zero
					int counter = 0;
					//While the counter is smaller than four
					while(counter <= 5)
					{
						switch(com.getState()) {
						case START:
							counter = 1;
							break;
						case waitStart:
							counter = 2;
							break;
						case startInit:
							counter = 3;
							break;
						case checkServer:
							counter = 4;
						case sendStates:
							counter = 5;
						default:
							break;
						}
						if(com.isSync()) 
							counter = 6;
						
						
						//Set the current progress. 
						//This value is going to be passed to the onProgressUpdate() method.
						publishProgress(counter*13);
					}
				}
			return null;
		}

		//Update the TextView and the progress at progress bar
		@Override
		protected void onProgressUpdate(Integer... values) 
		{
			//Update the progress at the UI if progress value is smaller than 100
			if(values[0] <= 100)
			{
				tv_progress.setText("Progress: " + Integer.toString(values[0]) + "%");
				pb_progressBar.setProgress(values[0]);
			}
		}
		
		//After executing the code in the thread
		@Override
		protected void onPostExecute(Void result) 
		{
			/* Initialize the application's main interface from the 'main.xml' layout xml file. 
	         * Add the initialized View to the viewSwitcher.*/
			viewSwitcher.addView(ViewSwitcher.inflate(MainActivity.this, R.layout.activity_main, null));
			//Switch the Views
			viewSwitcher.showNext();
			 
			addListView();  

			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
		         // Show the Up button in the action bar.
		         MainActivity.this.getActionBar().setDisplayHomeAsUpEnabled(true);
		         MainActivity.this.getActionBar().show();
		    }
		}
    }

    // This is the Socket Connect asynchronous thread.  Opening a socket
	// has to be done in an Asynchronous thread in Android.  Be sure you
	// have done the Asynchronous Tread tutorial before trying to understand
	// this code.
	
	public class SocketConnect extends AsyncTask<Void, Void, Socket> {

		// The main parcel of work for this thread.  Opens a socket
		// to connect to the specified IP.
		
		protected Socket doInBackground(Void... voids) {
			Socket s = null;
			String ip = Messages.getString("MainActivity.IP_Address"); //$NON-NLS-1$
			Integer port = Integer.valueOf(50002);

			try {
				s = new Socket(ip, port);
			} catch (UnknownHostException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
			return s;
		}

		// After executing the doInBackground method, this is 
		// automatically called, in the UI (main) thread to store
		// the socket in this app's persistent storage
		
		protected void onPostExecute(Socket s) {
			MyApplication myApp = (MyApplication) MainActivity.this
					.getApplication();
			myApp.sock = s;
		}
	}

	// This is a timer Task.  Be sure to work through the tutorials
	// on Timer Tasks before trying to understand this code.
	
	public class TCPReadTimerTask extends TimerTask {
		public void run() {
			MyApplication app = (MyApplication) getApplication();
			if (app.sock != null && app.sock.isConnected()
					&& !app.sock.isClosed()) {
				switch(com.getState()) {
				case START:
					//Clean buffer if any
					try {
						InputStream in = app.sock.getInputStream();
						int bytes_avail = in.available();
						if(bytes_avail > 0) {
							byte buf[] = new byte[bytes_avail];
							in.read(buf);
						}
					} catch (IOException e) {
						e.printStackTrace();
					}
					com.setState(Stats.waitStart);
				case waitStart:
					try {
						InputStream in = app.sock.getInputStream();
						int bytes_avail = in.available();
						if(bytes_avail >= 3) {
							byte buf[] = new byte[3];
							in.read(buf);
							if(com.checkStartAck(buf[0])) {
								com.setState(Stats.startInit);
							} 							
						} 
					} catch (IOException e) {
						e.printStackTrace();
					} 
					break;
				case startInit:
					try {
						OutputStream out;
						out = app.sock.getOutputStream();
						out.write(com.sendStartInit(), 0, 3);
						com.setState(Stats.checkServer);
					} catch (IOException e1) {
						e1.printStackTrace();
					}
					break;
				case sendStates:
					try {
						OutputStream out;
						out = app.sock.getOutputStream();
						out.write(com.sendStats(), 0, 3);
					} catch (IOException e1) {
						e1.printStackTrace();
					}
					
					break;
				case checkServer:
					try {
						InputStream in = app.sock.getInputStream();
						int bytes_avail = in.available();
						if(bytes_avail >= 3) {
							byte buf[] = new byte[3];
							in.read(buf);
							if(com.checkAck(buf[0])) {
								if(com.getBit(buf[0], 1) == 1 && ((int)buf[1] + (int)buf[2]) != 0) {
									com.setState(Stats.sendAck0);
									com.setReceivePacketSize((int)(buf[1]<<8)+(int)buf[2]);
									//Log.i("MY_MESSAGE", "packet received: "+String.valueOf(com.getReceivePacketSize()));
								} else
									com.setState(Stats.sendStates);
							} else {
								com.setState(Stats.sendStates);
							}
							
						} 
					} catch (IOException e) {
						e.printStackTrace();
					} 
					break;
				case waitServer:
					try {
						InputStream in = app.sock.getInputStream();
						int bytes_avail = in.available();
						if(bytes_avail == 3) {
							byte buf[] = new byte[3];
							in.read(buf);
							if(com.checkAck(buf[0]))
								com.setState(Stats.sendData0);
							else
								com.failReceive(Stats.sendStates);
						} else
							com.failReceive(Stats.sendStates);
					} catch (IOException e) {
						e.printStackTrace();
					} 
					break;
				case sendData0:
					OutputStream out1;
					Packet p;
					try {
						out1 = app.sock.getOutputStream();
						if(com.getPacketBuf() == null) {
							if((p = com.getSendQueue().poll())==null) {
								com.setState(Stats.sendStates); //something is wrong
								Log.i("Error", "Send queue is empty");
								return;
							} else {
								com.addPacketBuffer(p);
								com.setIndex_send_packets(com.getIndex_send_packets()+1);
							}
						} else //send the same packet multiply times due to fail of acknowledge
							p = com.getPacketBuf();
						p.setAck(com.getAck());
						out1.write(p.getPacket(), 0, p.length() + Packet.HEADERSIZE);
						com.setState(Stats.waitAck0);
					} catch (IOException e) {
						e.printStackTrace();
					}
					break;
				case waitAck0:
					try {
						InputStream in = app.sock.getInputStream();
						int bytes_avail = in.available();
						if(bytes_avail == 3) {
							byte buf[] = new byte[3];
							in.read(buf);
							com.checkAckAndQueue(buf[0]);
						} 
					} catch (IOException e) {
						e.printStackTrace();
					} 
					break;
				case sendAck0:
					try {
						OutputStream out2;
						out2 = app.sock.getOutputStream();
						out2.write(com.sendAck(), 0, 3);
					} catch (IOException e1) {
						e1.printStackTrace();
					}
					if(com.getIndex_packets() >= com.getReceivePacketSize()) {
						com.setState(Stats.checkServer);
						com.setIndex_packets(0);
						
						Object o = PacketConverter.decode(com.getReceiveQueue());
						
						if(o instanceof String) {
							final String s = (String)o;
							runOnUiThread(new Runnable() {
								public void run() {
									EditText et = (EditText) findViewById(R.id.RecvdMessage);
									et.setText(s);
								}
							});
						}
					} else
						com.setState(Stats.receiveData0);
					break;
				case receiveData0:
					try {
						InputStream in = app.sock.getInputStream();

						int bytes_avail = in.available();
						if (bytes_avail > 2) {
							byte header[] = new byte[2];
							in.read(header, 0, 2);		
							while((int)header[1] != bytes_avail) {
								//Log.i("WARNING", "received bytes not match");
								bytes_avail = in.available()+2;
							}
							byte buf[] = new byte[bytes_avail];
							buf[0] = header[0];
							buf[1] = header[1];
							in.read(buf, 2, bytes_avail-2);
							com.receive(new Packet(buf));
						
						}
					} catch (IOException e) {
						e.printStackTrace();
					}
					break;
				default:
					break;
				}
				
				com.getSched().cmdProcessing();
					
			}
		} 
	}
	
	
}
