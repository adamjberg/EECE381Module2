package Com;

import java.util.ArrayList;

public class Mix {
	
	private String name = "The Mix";
	private ArrayList<Clip> usedClips = new ArrayList<Clip>();
	private int timeLine;
	
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
	
	public void deleteClip(int index){
		usedClips.remove(index);
	}
	
	public void repeatClip(int index, int times){
		//TODO repeat
		usedClips.get(index).setToPlayAt(usedClips.get(index).getLength());
	}
	
	public void addClip(Clip a){
		if(usedClips.size() <= 4){
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
	
	public void setUp(){
		//Iterate thro usedclips and set up the timeline
	}
	
	
	
}
