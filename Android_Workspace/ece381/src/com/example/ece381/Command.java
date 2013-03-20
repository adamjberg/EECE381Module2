package com.example.ece381;

import java.util.ArrayList;

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
	}
	
	static public void play(int id, int vol, int pos) {
		Communication com = Communication.getInstance();
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
		if(com.getDB().getCurrSongsIds().contains(Integer.valueOf(id)))
			com.getDB().getCurrSongsIds().remove(Integer.valueOf(id));
		
	}
	
	static public void syncStop() {
		Communication com = Communication.getInstance();
		Command cmd = new Command(3);
		com.send(cmd);
		com.getSched().addCmd(cmd);
	}
	static public void stop() {
		
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
		Communication com = Communication.getInstance();
		Command cmd = new Command(6);
		cmd.addParameter(String.valueOf(song_id));
		com.send(cmd);
		com.getSched().addCmd(cmd);
	}
	static public void next(int song_id) {
		Communication com = Communication.getInstance();
		if(com.getDB().getCurr_playlist_id() != 0) {
			int curr_order =
					com.getDB().getSongOrderFromList()[com.getDB().getCurr_playlist_id()][song_id];
			com.getDB().setCurr_song_id(com.getDB().getSongIdFromOrder()[com.getDB().getCurr_playlist_id()][curr_order+1]);
		} else {
			com.getDB().setCurr_song_id(com.getDB().getCurr_song_id()+1);
		}
	}
	//index 7
	static public void syncPrev(int song_id) {
		Communication com = Communication.getInstance();
		Command cmd = new Command(7);
		cmd.addParameter(String.valueOf(song_id));
		com.send(cmd);
		com.getSched().addCmd(cmd);
	}
	static public void prev(int song_id) {
		Communication com = Communication.getInstance();
		if(com.getDB().getCurr_playlist_id() != 0) {
			int curr_order =
					com.getDB().getSongOrderFromList()[com.getDB().getCurr_playlist_id()][song_id];
			com.getDB().setCurr_song_id(com.getDB().getSongIdFromOrder()[com.getDB().getCurr_playlist_id()][curr_order-1]);
		} else {
			com.getDB().setCurr_song_id(com.getDB().getCurr_song_id()-1);
		}
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
	static public void syncRemoveSongFromList(int list_id, int song_id) {
		Communication com = Communication.getInstance();
		Command cmd = new Command(14);
		cmd.addParameter(String.valueOf(list_id));
		cmd.addParameter(String.valueOf(song_id));
		com.send(cmd);
		com.getSched().addCmd(cmd);
	}
	
	static public void removeSongFromList(int list_id, int song_id) {
		Communication com = Communication.getInstance();
		com.getDB().removeSongFromList(list_id, song_id);
	}
	
}
