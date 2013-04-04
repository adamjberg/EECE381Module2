package com.example.ece381;

import java.util.ArrayList;

import android.util.Log;

public class Command {

	private int cmd_index;
	private int num_of_parameters;
	private ArrayList<String> parameters;
	
	public Command(int cmd_index) {
		this.setCmdIndex(cmd_index);
		this.num_of_parameters = 0;
		this.parameters = new ArrayList<String>();
	}
	
	public void addParameter(String para) {
		this.parameters.add(para);
		this.num_of_parameters++;
	}
	
	public String getParameters() {
		int i;
		String result = "";
		for(i = 0; i < this.num_of_parameters; i ++) {
			result += this.parameters.get(i);
			result += " ";
		}
		return result;
	}

	public ArrayList<String> getParameterList() {
		return this.parameters;
	}
	public int getCmdIndex() {
		return cmd_index;
	}

	public void setCmdIndex(int cmd_index) {
		this.cmd_index = cmd_index;
	}
	
	public int getNumParameters() {
		return this.num_of_parameters;
	}
	
	public int getBytesLength() {
		int sum = 2;
		int i;
		for(i = 0; i < this.num_of_parameters; i++) {
			sum += this.parameters.get(i).length()+1;
		}
		return sum;
	}
	
	static public void syncPlay(int id, int vol, int pos) {
		Communication com = Communication.getInstance();	
		Command cmd = new Command(1);
		cmd.addParameter(String.valueOf(id));
		cmd.addParameter(String.valueOf(vol));
		cmd.addParameter(String.valueOf(pos));
		com.send(cmd);
		com.getSched().addCmd(cmd);	
		com.getDB().resume();
	}
	
	static public void play(int id, int vol, int pos) {
		Communication com = Communication.getInstance();
		com.getDB().setCurr_song_id(id);
		com.getDB().getCurrSongsIds().add(Integer.valueOf(id));
		com.getDB().getSongs()[id].setVolume(vol);
		com.getDB().getSongs()[id].setPos(pos);
	}
	
	static public void syncPause(int id) {
		Communication com = Communication.getInstance();
		Command cmd = new Command(2);
		cmd.addParameter(String.valueOf(id));
		com.send(cmd);
		com.getSched().addCmd(cmd);
	}
	
	static public void pause(int id) {
		Communication com = Communication.getInstance();
		if(com.getDB().isPaused() && !com.getDB().getRepeatSongValue()) return;
		if(com.getDB().getCurrSongsIds().contains(Integer.valueOf(id))) {
			if (com.getDB().getRepeatSongValue() && !com.getDB().getSongs()[com.getDB().getCurr_song_id()].getStart())
				Command.syncPlay(com.getDB().getCurr_song_id(), com.getDB().getSongs()[com.getDB().getCurr_song_id()].getVolume(), 0);
			com.getDB().getCurrSongsIds().remove(Integer.valueOf(id));
			com.getDB().setPaused();
		}
	}
	
	static public void syncStop() {
		Communication com = Communication.getInstance();
		Command cmd = new Command(3);
		com.send(cmd);
		com.getSched().addCmd(cmd);
	}
	static public void stop() {
		Communication com = Communication.getInstance();
		if(com.getDB().getCurr_song_id() == 0) return;
		com.getDB().getSongs()[com.getDB().getCurr_song_id()].setPos(0);
		com.getDB().getSongs()[com.getDB().getCurr_song_id()].setVolume(100);
	}
	//index 4
	static public void syncSetVol(int id, int vol) {
		Communication com = Communication.getInstance();
		Command cmd = new Command(4);
		cmd.addParameter(String.valueOf(id));
		cmd.addParameter(String.valueOf(vol));
		com.send(cmd);
		com.getSched().addCmd(cmd);
	}
	
	static public void setVol(int id, int vol) {
		Communication com = Communication.getInstance();
		com.getDB().getSongs()[id].setVolume(vol);
		
	}
	
	//index 6
	static public void syncNext(int song_id) {
		syncStop();
		Communication com = Communication.getInstance();
		Command cmd = new Command(6);
		cmd.addParameter(String.valueOf(song_id));
		com.send(cmd);
		com.getSched().addCmd(cmd);
	}
	static public void next(int song_id) {
		Communication com = Communication.getInstance();
		
		// Check for end of playlist
		int curr_plid = com.getDB().getCurr_playlist_id();
		int curr_songid = com.getDB().getCurr_song_id();
		int last_songInList, last_songid;
		
		int next_id = 0;
		if (com.getDB().getRepeatSongValue()) {
			Command.syncPlay(song_id, com.getDB().getSongs()[song_id].getVolume(), 0);
			return;
		} else if(com.getDB().getCurr_playlist_id() != 0) {
			last_songInList = com.getDB().getPlaylists()[curr_plid].getNum_of_songs();
			last_songid = com.getDB().getSongsFromList(curr_plid)[last_songInList];
			
			if(curr_songid == last_songid && com.getDB().getRepeatPlaylistValue()) {
				next_id = com.getDB().getSongsFromList(curr_plid)[1];			
				com.getDB().setIsEndOfPlaylist(true);	
				Command.syncPlay(next_id, com.getDB().getSongs()[next_id].getVolume(), 0);
				return;
			} else {
					com.getDB().setIsEndOfPlaylist(false);
					int curr_order =
							com.getDB().getSongOrderFromList()[com.getDB().getCurr_playlist_id()][song_id];
					next_id = com.getDB().getSongIdFromOrder()[com.getDB().getCurr_playlist_id()][curr_order+1];	
				}
			}
		 else if(song_id < com.getDB().getTotalSongs()){
			next_id = com.getDB().getCurr_song_id()+1;
		}
		if(next_id == 0) return;
		
		com.getDB().setCurr_song_id(next_id);
		com.getDB().getSongs()[next_id].setPos(0);
		//com.getDB().getSongs()[next_id].setVolume(100);
	}
	//index 7
	static public void syncPrev(int song_id) {
		syncStop();
		Communication com = Communication.getInstance();
		Command cmd = new Command(7);
		cmd.addParameter(String.valueOf(song_id));
		com.send(cmd);
		com.getSched().addCmd(cmd);
	}
	static public void prev(int song_id) {
		Communication com = Communication.getInstance();
		int next_id = 0;
		if(com.getDB().getCurr_playlist_id() != 0) {
			int curr_order =
					com.getDB().getSongOrderFromList()[com.getDB().getCurr_playlist_id()][song_id];
			next_id = com.getDB().getSongIdFromOrder()[com.getDB().getCurr_playlist_id()][curr_order-1];
		} else if(song_id > 1){
			next_id = com.getDB().getCurr_song_id()-1;
		}
		if(next_id == 0) return;
		com.getDB().setCurr_song_id(next_id);
		com.getDB().getSongs()[next_id].setPos(0);
		//com.getDB().getSongs()[next_id].setVolume(100);
	}
	// index 8	
	static public void syncCreatePlaylist(String list_name) {
		Communication com = Communication.getInstance();
		Command cmd = new Command(8);
		cmd.addParameter(list_name);
		com.send(cmd);
		com.getSched().addCmd(cmd);
	}
	
	static public void createPlaylist(String list_name) {
		Communication com = Communication.getInstance();
		Playlist pl = new Playlist(list_name);
		com.getDB().addList(pl);		
	}
 	
	//index 9
	static public void createExisitedPlaylist(String list_name, int num_of_songs, int id) {
		Playlist pl = new Playlist(list_name);
		pl.setId(id);
		pl.setNumSongs(num_of_songs);
		Communication com = Communication.getInstance();
		com.getDB().addExisitedList(pl, id);
	}

	// index 10
	static public void createSong(String song_name, int len) {
		Communication com = Communication.getInstance();
		Song song = new Song(song_name);
		song.setSize(len);
		com.getDB().addSong(song);
	}
	
	//index 11
	static public void syncSelectList(int id) {
		Communication com = Communication.getInstance();
		Command cmd = new Command(11);
		cmd.addParameter(String.valueOf(id));
		com.send(cmd);
		com.getSched().addCmd(cmd);
	}
	
	static public void selectList(int id) {
		Communication com = Communication.getInstance();
		com.getDB().setCurr_playlist_id(id);
	}
	
	//index 12
	static public void hasSync() {
		Communication com = Communication.getInstance();
		com.setSync();
	}
	
	//index 13
	static public void syncAddSongToList(int list_id, int song_id) {
		Communication com = Communication.getInstance();
		Command cmd = new Command(13);
		cmd.addParameter(String.valueOf(list_id));
		cmd.addParameter(String.valueOf(song_id));
		com.send(cmd);
		com.getSched().addCmd(cmd);
	}
	
	static public void addSongToList(int list_id, int song_id) {
		Communication com = Communication.getInstance();
		com.getDB().addSongToList(list_id, song_id);
		
	}
	
	//index 14
	static public void addExisitedSongToList(int list_id, int song_id) {
		Communication com = Communication.getInstance();
		com.getDB().addExisitedSongToList(list_id, song_id);
	}
	//index 15
	static public void syncRemoveSongFromList(int list_id, int song_id) {
		Communication com = Communication.getInstance();
		Command cmd = new Command(15);
		cmd.addParameter(String.valueOf(list_id));
		cmd.addParameter(String.valueOf(song_id));
		com.send(cmd);
		com.getSched().addCmd(cmd);
	}
	
	static public void removeSongFromList(int list_id, int song_id) {
		Communication com = Communication.getInstance();
		com.getDB().removeSongFromList(list_id, song_id);
	}
	//index 16
	static public void syncUpdatePos(int song_id, int pos) {
		
	}
	static public void updatePos(int song_id, int pos, int isStart) {
		Log.i("UpdatePos", String.valueOf(isStart));
		Communication com = Communication.getInstance();
		if(com.getDB().getSongs()[song_id] == null) return;
		com.getDB().getSongs()[song_id].setPos(pos);
		com.getDB().getSongs()[song_id].setTrigger(isStart);
		
	}
	//index 17
	static public void syncRepeatList(int list_id) {
		Communication com = Communication.getInstance();
		Command cmd = new Command(17);
		cmd.addParameter(String.valueOf(list_id));
		com.send(cmd);
	}
	
	//index 18
	static public void syncModifyListName(int list_id, String listname) {
		Communication com = Communication.getInstance();
		Command cmd = new Command(18);
		cmd.addParameter(String.valueOf(list_id));
		cmd.addParameter(listname);
		com.send(cmd);
		com.getSched().addCmd(cmd);
		
	}
	
	//index 19
	static public void syncRemoveList(int list_id) {
		Communication com = Communication.getInstance();
		Command cmd = new Command(19);
		cmd.addParameter(String.valueOf(list_id));
		com.send(cmd);
		
	}
	
	static public void removeList(int list_id) {
		Communication com = Communication.getInstance();
		com.getDB().removeList(list_id);
	}
	
	//index 20
	static public void syncPlaySongFromAllSongs(int id, int vol, int pos) {
		Communication com = Communication.getInstance();
		Command cmd = new Command(20);
		cmd.addParameter(String.valueOf(id));
		cmd.addParameter(String.valueOf(vol));
		cmd.addParameter(String.valueOf(pos));
		com.send(cmd);
		com.getSched().addCmd(cmd);	

	}
	//index 21
	static public void syncOpenAllSongPanel() {
		Communication com = Communication.getInstance();
		Command cmd = new Command(21);
		com.send(cmd);

	}
	//index 22
	static public void syncOpenPlaylistsPanel() {
		Communication com = Communication.getInstance();
		Command cmd = new Command(22);
		com.send(cmd);

	}
	//index 23
	static public void syncOpenSongsFromList(int list_id) {

		Communication com = Communication.getInstance();
		Command cmd = new Command(23);
		cmd.addParameter(String.valueOf(list_id));
		com.send(cmd);
	}
	//index 24
	static public void syncPlaySongFromList(int song_id, int list_id) {
		Communication com = Communication.getInstance();
		Command cmd = new Command(24);
		cmd.addParameter(String.valueOf(song_id));
		cmd.addParameter(String.valueOf(list_id));
		com.send(cmd);

	}
	
	// index 25
	static public void syncSetPitch(int song_id, int pitch, int quality) {
		Communication com = Communication.getInstance();
		Command cmd = new Command(25);
		cmd.addParameter(String.valueOf(song_id));
		cmd.addParameter(String.valueOf(pitch));
		cmd.addParameter(String.valueOf(quality));
		com.send(cmd);
	}
	
	// index 26
	static public void syncSetPlaybackSpeed(int song_id, int speed) {
		Communication com = Communication.getInstance();
		Command cmd = new Command(26);
		cmd.addParameter(String.valueOf(song_id));
		cmd.addParameter(String.valueOf(speed));
		com.send(cmd);
	}
	
	// index 27
	static public void syncReloadSong(int song_id) {
		Communication com = Communication.getInstance();
		Command cmd = new Command(27);
		cmd.addParameter(String.valueOf(song_id));
		com.send(cmd);
	}
}
