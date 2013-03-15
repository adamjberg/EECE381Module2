package com.example.ece381;

public class Playlist {
	private String list_name;
	private int num_of_songs;
	private int id;
	private int num_of_lists = 0; // number of playlists
	
	// default constructor - creates a blank playlist
	public Playlist() { 

		this.list_name = String.format("New Playlist %d", num_of_lists);
		this.num_of_songs = 0;
		this.id = 0;
		num_of_lists++;
	}
	
	public Playlist(String name) {
		this.list_name = name;
		this.num_of_songs = 0;
		this.id = 0;
		num_of_lists++;
	}
	
	public Playlist(String name, int num_of_song, int id) {
		this.list_name = name;
		this.num_of_songs = num_of_song;
		this.id = id;
		num_of_lists++;
	}
	
	// Mutators
	public void setId(int id) {
		this.id = id;
	}
	
	public void setNumSongs(int num_of_songs) {
		this.num_of_songs = num_of_songs;
	}

	public void setListName(String new_name) {
		this.list_name = new_name;
	}
	
	// Observers
	public int getId() {
		return id;
	}
	
	public String getListName() {
		return list_name;
	}
	
	public int getNumSongs() {
		return num_of_songs;
	}
	
}
