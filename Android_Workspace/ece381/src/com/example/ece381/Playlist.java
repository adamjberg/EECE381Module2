package com.example.ece381;

public class Playlist {
	private String list_name;
	private int num_of_songs;
	private int id;
	public Playlist(String name) {
		this.list_name = name;
		this.num_of_songs = 0;
		this.id = 0;
	}
	public Playlist(String name, int num_of_song, int id) {
		this.list_name = name;
		this.num_of_songs = num_of_song;
		this.id = id;
	}
}
