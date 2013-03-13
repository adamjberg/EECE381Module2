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
	
	static public void syncPlay(String song_name, int pos) {
		Communication com = Communication.getInstance();
		Command cmd = new Command(1);
		cmd.addParameter(song_name);
		cmd.addParameter(String.valueOf(pos));
		com.send(cmd);
		com.getSched().addCmd(cmd);
	}
	
	static public void play(String song_name, int po) {
		
	}
	
	static public void synPause() {
		Communication com = Communication.getInstance();
		Command cmd = new Command(2);
		com.send(cmd);
		com.getSched().addCmd(cmd);
	}
	
	static public void pause() {
		
	}
	
	static public void syncStop() {
		Communication com = Communication.getInstance();
		Command cmd = new Command(3);
		com.send(cmd);
		com.getSched().addCmd(cmd);
	}
	static public void stop() {
		
	}
	
	static public void createExisitedPlaylist(String list_name, int num_of_songs, int id) {
		Playlist pl = new Playlist(list_name);
		pl.setId(id);
		pl.setNumSongs(num_of_songs);
		Communication com = Communication.getInstance();
		com.getDB().addExisitedList(pl, id);
	}
}
