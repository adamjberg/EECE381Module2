package com.example.ece381;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Timer;
import java.util.TimerTask;

import com.example.ece381.Communication.Stats;

import android.app.Activity;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.StrictMode;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends Activity {

	private Timer tcp_timer;
	private Communication com = Communication.getInstance();
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		
		// This call will result in better error messages if you
		// try to do things in the wrong thread.
		
		StrictMode.setThreadPolicy(new StrictMode.ThreadPolicy.Builder()
				.detectDiskReads().detectDiskWrites().detectNetwork()
				.penaltyLog().build());

		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		EditText et = (EditText) findViewById(R.id.RecvdMessage);
		et.setKeyListener(null);
		et = (EditText) findViewById(R.id.error_message_box);
		et.setKeyListener(null);

		// Set up a timer task.  We will use the timer to check the
		// input queue every 500 ms
		
		TCPReadTimerTask tcp_task = new TCPReadTimerTask();
		tcp_timer = new Timer();
		tcp_timer.schedule(tcp_task, 5000, 60);
		
		new SocketConnect().execute((Void) null);
		
		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}


	@Override
	public void finish() {
		tcp_timer.cancel();
		closeSocket();
		Log.i("LOG", "Socket Closed");
		com.resetCom();
		super.finish();
	}
	
	// Route called when the user presses "connect"
	
	public void openSocket(View view) {
		MyApplication app = (MyApplication) getApplication();
		TextView msgbox = (TextView) findViewById(R.id.error_message_box);

		// Make sure the socket is not already opened 
		
		if (app.sock != null && app.sock.isConnected() && !app.sock.isClosed()) {
			msgbox.setText("Socket already open"); //$NON-NLS-1$
			return;
		}
		
		// open the socket.  SocketConnect is a new subclass
	    // (defined below).  This creates an instance of the subclass
		// and executes the code in it.
		
		new SocketConnect().execute((Void) null);
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
		closeSocket();
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
	// Construct an IP address from the four boxes
	
	public String getConnectToIP() {
		String addr = ""; //$NON-NLS-1$
		EditText text_ip;
		text_ip = (EditText) findViewById(R.id.ip1);
		addr += text_ip.getText().toString();
		text_ip = (EditText) findViewById(R.id.ip2);
		addr += "." + text_ip.getText().toString(); //$NON-NLS-1$
		text_ip = (EditText) findViewById(R.id.ip3);
		addr += "." + text_ip.getText().toString(); //$NON-NLS-1$
		text_ip = (EditText) findViewById(R.id.ip4);
		addr += "." + text_ip.getText().toString(); //$NON-NLS-1$
		return addr;
	}

	// Gets the Port from the appropriate field.
	
	public Integer getConnectToPort() {
		Integer port;
		EditText text_port;

		text_port = (EditText) findViewById(R.id.port);
		port = Integer.parseInt(text_port.getText().toString());

		return port;
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
			Integer port = getConnectToPort();

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
									Log.i("MY_MESSAGE", "packet received: "+String.valueOf(com.getReceivePacketSize()));
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
						
						final String s = (String)PacketConverter.decode(com.getReceiveQueue());
						
						runOnUiThread(new Runnable() {
							public void run() {
								EditText et = (EditText) findViewById(R.id.RecvdMessage);
								et.setText(s);
							}
						});
						Log.i("MY_MESSAGE", s );
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
								Log.i("WARNING", "received bytes not match");
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
				
					
			}
		}
	}
	
}
