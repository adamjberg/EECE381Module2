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
				Command.play(Integer.valueOf(cmd.getParameterList().get(0)).intValue(),
								Integer.valueOf(cmd.getParameterList().get(1)).intValue(),
								 Integer.valueOf(cmd.getParameterList().get(2)).intValue());
				Log.i("COMMAND", "The Song "+cmd.getParameterList().get(0)+" is playing at "+cmd.getParameterList().get(1));
				break;
			case 2:
				Command.pause(Integer.valueOf(cmd.getParameterList().get(0)).intValue());
				Log.i("COMMAND", "The song "+cmd.getParameterList().get(0)+" is puased.");
				break;
			case 3:
				Command.stop();
				Log.i("COMMAND", "The song is stopped.");
				break;
			case 4:
				Command.setVol(Integer.valueOf(cmd.getParameterList().get(0)), Integer.valueOf(cmd.getParameterList().get(1)));
				break;
			case 5:
				break;
			case 6:
				Command.next(Integer.valueOf(cmd.getParameterList().get(0)));
				Log.i("COMMAND", "NEXT: "+cmd.getParameterList().get(0));
				break;
			case 7:
				Command.prev(Integer.valueOf(cmd.getParameterList().get(0)));
				Log.i("COMMAND", "PREV: "+cmd.getParameterList().get(0));
				break;
			case 8:
				Command.createPlaylist(cmd.getParameterList().get(0));
				break;
			case 9:
				Command.createExisitedPlaylist(cmd.getParameterList().get(0),
						Integer.valueOf(cmd.getParameterList().get(1)).intValue(),  Integer.valueOf(cmd.getParameterList().get(2)).intValue());
				Log.i("COMMAND", "playlist "+cmd.getParameterList().get(0)+ " is created");
				break;
			case 10:
				Command.createSong(cmd.getParameterList().get(0), Integer.valueOf(cmd.getParameterList().get(1)));
				Log.i("COMMAND", "Song "+cmd.getParameterList().get(0)+" is created");
				break;
			case 11:
				Command.selectList(Integer.valueOf(cmd.getParameterList().get(0)).intValue());
				Log.i("COMMAND", "List "+cmd.getParameterList().get(0) + " is selected");
				break;
			case 12:
				Command.hasSync();
				break;
			case 13:
				Command.addSongToList(Integer.valueOf(cmd.getParameterList().get(0)), Integer.valueOf(cmd.getParameterList().get(1)));
				break;
			case 14:
				Command.addExisitedSongToList(Integer.valueOf(cmd.getParameterList().get(0)), Integer.valueOf(cmd.getParameterList().get(1)));
				break;
			case 15:
				Command.removeSongFromList(Integer.valueOf(cmd.getParameterList().get(0)), Integer.valueOf(cmd.getParameterList().get(1)));
				break;
			case 16:
				Command.updatePos(Integer.valueOf(cmd.getParameterList().get(0)), Integer.valueOf(cmd.getParameterList().get(1)), Integer.valueOf(cmd.getParameterList().get(2)));
				break;
			case 17:
				break;
			case 18:
				break;
			case 19:
				Command.removeList(Integer.valueOf(cmd.getParameterList().get(0)));
				break;
			case 20:
				Command.play(Integer.valueOf(cmd.getParameterList().get(0)).intValue(),
					Integer.valueOf(cmd.getParameterList().get(1)).intValue(),
					 Integer.valueOf(cmd.getParameterList().get(2)).intValue());
				break;
			default:
				break;
			}
		}
	}
}
