package com.example.ece381;

import java.util.ArrayList;

//Clips are mutable Android objects representations of sound clips that are physically on the DE2. Manipulation of Clips
//in Android will directly or indirectly affect the physical representations on the DE2

public class Clip {
	private int ID;
	private String name;
	private int numberOfPlays = 0;
	private ArrayList<Integer> startTimes = new ArrayList<Integer>();
	private int length;
	private int volume;
	private int position = 0;
	private int playbackSpeed=50;
	private int pitch=50;


	// defualt constructor
	public Clip() {
		name = "default";
		numberOfPlays = 0;
	}

	// constructor with name and length of song
	public Clip(String name, int length) {
		this.name = name;
		this.length = length;
	}

	// conctructor to add 2 clips together
	public Clip(Clip a, Clip b) {
		name = "new appended clip";
		length = a.length + b.length;
	}

	// constructor to make a new sub clip
	public Clip(Clip a, int start, int end) {
		name = "newsubclip";
		length = end - start;
	}

	public Clip(String s, int length, int id) {
		this.name = s;
		this.length = length;
		this.ID = id;
	}

	public Clip(Song song) {
		this.name = song.getSongName();
		this.ID = song.getId();
		this.length = song.getSize() / 10;
		this.volume = song.getVolume();
		this.position = song.getPos();
		this.pitch = 50;
		this.playbackSpeed = 50;
	}

	// create a new clip from a old clip by chopping off the ending starting
	// from location
	public Clip split(int location) {
		if (length > location && location > 0) {
			return new Clip("new clip", length - location);
		} else {
			return null;
		}
	}

	// set up a clip to be played at a time
	public void setToPlayAt(int time) {
		numberOfPlays++;
		startTimes.add(time);
	}

	public void setToPlayAt(int time, int set) {
		numberOfPlays++;
		startTimes.add(time);
	}

	// remove a play time at an index
	public void removePlayAt(int index) {
		startTimes.remove(index);
		numberOfPlays--;
	}

	// chop current clip from both ends at start to end
	public void cutClip(int start, int end) {
		if (end - start < length) {
			length = end - start;
		} else {
			System.out.println("did not cut");
		}
	}

	// attach another clip to the end of current clip
	public void appendClip(Clip other) {
		if (this.numberOfPlays == 0) {
			length += other.length;
		}

		else {
			new Clip(this, other);
		}
	}

	// merge the sound of 2 clips
	public Clip mergeClips(Clip other) {
		Clip temp = (other.length > this.length) ? other : this;
		return temp;
	}

	public void wipePlayData() {
		numberOfPlays = 0;
		startTimes.clear();
	}
	
	public void reload() {
		Command.syncReloadSong(ID);
	}
	
	public void play(int globalVol) {
		if(startTimes.size() > 0) {
			Command.syncPlay(ID, (int)(volume*(globalVol/100.0)), -startTimes.get(0) * 10);			
		}
	}
		
	public void stop() {
		Command.syncPause(ID);
	}

	public int getVolume() {
		return volume;
	}

	public int getPosition() {
		return position;
	}

	public int getLength() {
		return length;
	}

	public String getName() {
		return name;
	}

	public int getPitch() {
		return pitch;
	}

	public int getPlaybackSpeed() {
		return playbackSpeed;
	}

	public void setVolume(int volume) {
		this.volume = volume;
	}

	public void setPosition(int position) {
		this.position = position;
	}

	public void setLength(int length) {
		this.length = length;
	}

	public void setPitch(int pitch) {
		this.pitch = pitch;
	}

	public void setPlaybackSpeed(int playbackSpeed) {
		this.playbackSpeed = playbackSpeed;
	}

	public void setName(String newName) {
		name = newName;
	}

	public ArrayList<Integer> timesUsed() {
		return (ArrayList<Integer>) startTimes.clone();
	}

	public void changePlayTime(int index, int time) {
		startTimes.add(index, time);
		startTimes.remove(index++);
	}

	public int getID() {
		return ID;
	}
}
