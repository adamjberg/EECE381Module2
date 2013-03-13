package com.example.ece381;

import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

import android.util.Log;

public class CmdScheduler {
	private Queue<Command> cmds;
	
	public CmdScheduler() {
		cmds = new ConcurrentLinkedQueue<Command>();
	}
	
	public void addCmd(Command cmd) {
		cmds.add(cmd);
	}
	
	public void clear() {
		cmds.clear();
	}
	
	public void cmdProcessing() {
		if(this.cmds.size() <= 0) return;
		int size = this.cmds.size();
		int i;
		Command cmd;
		for(i = 0; i< size; i++) {
			cmd = cmds.poll();
			switch(cmd.getCmdIndex()) {
			case 1:
				Command.play(Integer.valueOf(cmd.getParameterList().get(0)).intValue(), Integer.valueOf(cmd.getParameterList().get(1)).intValue());
				Log.i("COMMAND", "The Song "+cmd.getParameterList().get(0)+" is playing at "+cmd.getParameterList().get(1));
				break;
			case 2:
				Command.pause();
				Log.i("COMMAND", "The song is puased.");
				break;
			case 3:
				Command.stop();
				Log.i("COMMAND", "The song is stopped.");
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				Log.i("COMMAND", "Next song is selected.");
				break;
			case 7:
				Log.i("COMMAND", "previous song is selected.");
				break;
			case 8:
				break;
			case 9:
				Command.createExisitedPlaylist(cmd.getParameterList().get(0),
						Integer.valueOf(cmd.getParameterList().get(1)).intValue(),  Integer.valueOf(cmd.getParameterList().get(2)).intValue());
				Log.i("COMMAND", "playlist "+cmd.getParameterList().get(0)+ " is created");
				break;
			case 10:
				break;
			case 11:
				break;
			case 12:
				break;
			default:
				break;
			}
		}
	}
}
