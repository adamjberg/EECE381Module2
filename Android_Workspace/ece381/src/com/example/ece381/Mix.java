package com.example.ece381;

import java.util.ArrayList;

public class Mix {
	
	private String name = "The Mix";
	private ArrayList<Clip> usedClips = new ArrayList<Clip>();
	private int timeLine;
	private int length;
	
	
	public Mix(){
		timeLine = 0;
	}
	
	public Mix(String name){
		this.name = name;
		timeLine = 0;
	}
	
	//TODO implement and design functions
	
	public void removeAllInstanceOf(int index){
		usedClips.get(index).wipePlayData();
	}
	
	//TODO make segment
	public static Clip makeNewSegment(){
		return new Clip();
	}
	
	public Clip getClipat(int index){
		return usedClips.get(index);
	}
	
	public void deleteClip(int index){
		usedClips.remove(index);
	}
	
	public void repeatClip(int index, int times){
		//TODO repeat
		usedClips.get(index).setToPlayAt(usedClips.get(index).getLength());
	}
	
	public void addClip(Clip a){
		if(usedClips.size() <= 50){
			usedClips.add(a);
		}
	}
	
	public Clip cutSegment(int start, int end){
		//look through all clips for sound between start and end times
		return new Clip();
	}
	
	public void saveMix(){
		//TODO DE2 implement
		
	}
	public void setName(String name){
		this.name = name;
	}
	
	public void seekTime(int time){
		timeLine= time;
	}
	
	public ArrayList <Clip> listContents(){
		return (ArrayList<Clip>) usedClips.clone();
	}
	
	public int lengthOf(){
		int temp = 0;
		for(Clip c: usedClips){
			for(Integer t: c.timesUsed()){
				if( (t + c.getLength()) > temp){
					temp = t + c.getLength();
				}
			}
		}
		length = temp;
		return length;
	}
	
	public ArrayList<Clip> findNowActiveClips(){
		ArrayList temp = new ArrayList();
		for(Clip c: usedClips){
			for(Integer t: c.timesUsed()){
				if(t > timeLine && t < (timeLine+ //time segment 20 seconds
						20000)){
				temp.add(c);}
			}
		}
		return temp;
	}
	
	public void setCliptoPlatAt(int index, int time){
		if(index < usedClips.size()){
		usedClips.get(index).setToPlayAt(time);}
	}
	
	
	public void play() {
		for (int i = 0; i < lengthOf(); i++) {
			usedClips.get(i).play();
		}
	}
	
	public void stop() {
		for (int i = 0; i < lengthOf(); i++) {
			usedClips.get(i).stop();
		}
	}
	
	public void removeClipPlay(int index, int time) {
		// TODO Auto-generated method stub
		usedClips.get(index).timesUsed();
	}
}
