package com.example.ece381;

import java.util.ArrayList;
import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

import android.util.Log;


public class Database {
	public static final int MAX_LISTS = 51;
	public static final int MAX_SONGS = 101;
	private Playlist[] playlists;
	private int curr_playlist_id;
	private int num_of_lists;
	private Song[] songs;
	private int curr_song_id;
	private int num_of_songs;
	private Queue<Integer> avail_list_index;
	private int used_list_index[];
	private int[][] list_song_order;
	private int[][] list_order_song;
	
	public Database() {
		this.playlists = new Playlist[MAX_LISTS];
		this.songs = new Song[MAX_SONGS];
		this.avail_list_index = new ConcurrentLinkedQueue<Integer>();
		this.avail_list_index.clear();
		this.used_list_index = new int[MAX_LISTS];
		this.list_order_song = new int[MAX_LISTS][MAX_SONGS];
		this.list_song_order = new int[MAX_LISTS][MAX_SONGS];
		int i;
		for(i = 1; i < MAX_LISTS; i++) {
			this.avail_list_index.add(Integer.valueOf(i));
		}
	}
	
	public Queue<Integer> getAvail_list() {
		return this.avail_list_index;
	}
	
	public int[] getUsed_list() {
		return this.used_list_index;
	}
	
	public void addList(Playlist pl) {
		if(this.avail_list_index.size() <= 0) {
			Log.i("ERROR", "Added list failed\n");
			return;
		}
		int id = this.avail_list_index.poll().intValue();
		pl.setId(id);
		this.playlists[id] = pl;
		this.used_list_index[id] = 1;
		int i;
		for(i = 0; i < MAX_SONGS; i++) {
			this.list_order_song[id][i] = 0;
			this.list_song_order[id][i] = 0;
		}
		this.num_of_lists++;
	}
	public void addExisitedList(Playlist pl, int id) {
		if(!this.avail_list_index.contains(Integer.valueOf(id))) {
			Log.i("ERROR", "Added exisited list failed\n");
			return;
		}
		this.avail_list_index.remove(Integer.valueOf(id));
		pl.setId(id);
		this.playlists[id] = pl;
		this.used_list_index[id] = 1;
		this.num_of_lists++;
	}

	public void clear() {
		int i, j;
		for(i = 0; i < MAX_LISTS; i++) {
			this.playlists[i] = null;
			for(j = 0; j < MAX_SONGS; j++) {
				this.list_order_song[i][j] = 0;
				this.list_song_order[i][j] = 0;
			}
			this.used_list_index[i] = 0;
		}
		this.avail_list_index.clear();
		for(i = 1; i < MAX_LISTS; i++) {
			this.avail_list_index.add(Integer.valueOf(i));
		}
		for(j = 0; j < MAX_SONGS; j++) {
			this.songs[j] = null;
 		}
		this.curr_playlist_id = this.curr_song_id = this.num_of_lists = this.num_of_songs = 0;
	}
}
