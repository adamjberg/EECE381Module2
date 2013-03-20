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
	private Queue<Integer> curr_song_ids;
	private int used_list_index[];
	private int[][] list_song_order;
	private int[][] list_order_song;
	private ArrayList<String> songs_name;
	private ArrayList<String> lists_name;
	
	
	public Database() {
		this.playlists = new Playlist[MAX_LISTS];
		this.songs = new Song[MAX_SONGS];
		this.avail_list_index = new ConcurrentLinkedQueue<Integer>();
		this.curr_song_ids = new ConcurrentLinkedQueue<Integer>();
		this.avail_list_index.clear();
		this.used_list_index = new int[MAX_LISTS];
		this.list_order_song = new int[MAX_LISTS][MAX_SONGS];
		this.list_song_order = new int[MAX_LISTS][MAX_SONGS];
		this.songs_name = new ArrayList<String>();
		this.lists_name = new ArrayList<String>();
		int i;
		for(i = 1; i < MAX_LISTS; i++) {
			this.avail_list_index.add(Integer.valueOf(i));
		}
	}
	
	public int queryListByName(String list_name) {
		int i = 1;
		while(i <= this.num_of_lists) {
			if(this.used_list_index[i] == 1) {
				if(this.playlists[i].getListName().equals(list_name)) {
					return i;
				}
			} i++;
		} return -1;
	}
	public Queue<Integer> getCurrSongsIds() {
		return this.curr_song_ids;
	}
	public Queue<Integer> getAvail_list() {
		return this.avail_list_index;
	}
	
	public int[] getUsed_list() {
		return this.used_list_index;
	}
	
	public Song[] getSongs() {
		return this.songs;
	}
	public void addSong(Song song) {
		song.setId(++this.num_of_songs);
		this.songs[this.num_of_songs] = song;
		this.songs_name.add(song.getSongName());
	}
	
	public String[] getSongsName() {
		String result[] = new String[this.num_of_songs];

		int i;
		for(i = 0; i < this.num_of_songs; i++) {
			result[i] = (String)this.songs_name.get(i);
		}
		return result;
	}
	
	public String[] getSongsNameFromList(int list_id) {
		int size = this.playlists[list_id].getNum_of_songs();
		String result[] = new String[size];
		int i;
		for(i = 1; i<= size; i++) {
			result[i] = (String)this.songs[this.list_order_song[list_id][i]].getSongName();
		}
		return result;
	}
	public int getTotalSongs() {
		return this.num_of_songs;
	}
	
	public String[] getListsName() {
		String result[] = new String[this.num_of_lists];
		
		int i;
		for(i = 0; i < this.num_of_lists; i++) {
			result[i] = (String) this.lists_name.get(i);
		}
		return result;
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
		this.lists_name.add(pl.getListName());
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
		this.lists_name.add(pl.getListName());
		pl.setId(id);
		this.playlists[id] = pl;
		this.used_list_index[id] = 1;
		this.num_of_lists++;
	}

	public Playlist[] getPlaylists() {
		return playlists;
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
		this.setCurr_playlist_id(0);
		this.num_of_lists = this.num_of_songs = this.curr_song_id = 0;
		this.curr_song_ids.clear();
		this.songs_name.clear();
		this.lists_name.clear();
	}

	public int[][] getSongOrderFromList() {
		return this.list_song_order;
	}
	
	public int[][] getSongIdFromOrder() {
		return this.list_order_song;
	}
	
	public void removeList(int id) {
		int i;
		for(i = 1; i <= this.playlists[id].getNum_of_songs(); i++) {
			this.list_order_song[id][i] = 0;
			this.list_song_order[id][i] = 0;
		}

		this.playlists[id] = null;
		this.num_of_lists--;
	}
	
	public int getNumLists() {
		return num_of_lists;
	}
	
	public int getCurr_playlist_id() {
		return curr_playlist_id;
	}

	public void setCurr_playlist_id(int curr_playlist_id) {
		if(curr_playlist_id <= 0) return;
		this.curr_playlist_id = curr_playlist_id;
	}
	
	public void setCurr_song_id(int id) {
		this.curr_song_id = id;
	}
	public int getCurr_song_id() {
		return this.curr_song_id;
	}
	
	public int[] getSongsFromList(int list_id) {
		return this.list_order_song[list_id];
	}
	
	public void addSongToList(int list_id, int song_id) {
		this.playlists[list_id].setNum_of_songs(this.playlists[list_id].getNum_of_songs()+1);
		this.list_song_order[list_id][song_id] = this.playlists[list_id].getNum_of_songs();
		this.list_order_song[list_id][this.playlists[list_id].getNum_of_songs()] = song_id;
	}
	
	public void addExisitedSongToList(int list_id, int song_id) {
		int i;
		for(i = 1; i <= this.playlists[list_id].getNum_of_songs(); i++) {
			if(this.list_order_song[list_id][i] == 0) {
				this.list_order_song[list_id][i] = song_id;
				this.list_song_order[list_id][song_id] = i;
				return;
			}
		}
	}
	
	public void removeSongFromList(int list_id, int song_id) {
		int order = this.list_song_order[list_id][song_id];
		this.list_song_order[list_id][song_id] = 0;
		int i;
		for(i = order; i <= this.playlists[list_id].getNum_of_songs() - order; i++) {
			this.list_order_song[list_id][i] = this.list_order_song[list_id][i+1];
			this.list_song_order[list_id][this.list_order_song[list_id][i]] = i;
		} this.list_order_song[list_id][i] = 0;
		
	}

}
