package com.example.ece381;

import java.util.ArrayList;
import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

public class Database {
	private ArrayList<Playlist> playlists;
	private int curr_playlist_id;
	private int num_of_lists;
	private ArrayList<Song> songs;
	private int curr_song_id;
	private int num_of_songs;
	private Queue<Integer> avail_list_index;
	private ArrayList<Integer> used_list_index;
	
	public Database() {
		this.playlists = new ArrayList<Playlist>();
		this.songs = new ArrayList<Song>();
		this.avail_list_index = new ConcurrentLinkedQueue<Integer>();
		this.used_list_index = new ArrayList<Integer>();
	}

}
