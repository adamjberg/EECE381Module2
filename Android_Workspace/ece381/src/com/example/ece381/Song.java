package com.example.ece381;

public class Song {
	private String song_name;
	private int volume;
	private int pos;
	private int size;
	private int id;
	public Song(String song_name) {
		this.song_name = song_name;
		this.volume = 1;
	}
	public int getVolume() {
		return volume;
	}
	public void setVolume(int volume) {
		this.volume = volume;
	}
	public int getPos() {
		return pos;
	}
	public void setPos(int pos) {
		this.pos = pos;
	}
	public int getId() {
		return id;
	}
	public void setId(int id) {
		this.id = id;
	}
	public String getSongName() {
		return this.song_name;
	}
	public int getSize() {
		return size;
	}
	public void setSize(int size) {
		this.size = size;
	}
	
}
