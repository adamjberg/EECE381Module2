package Com;

import java.util.ArrayList;

public class Mix {
	
	private String name = "The Mix";
	private ArrayList<Clip> usedClips = new ArrayList();
	
	public Mix(){
		
	}
	
	public Mix(String name){
		this.name = name;
	}
	
	//TODO implement and design functions
	public static void setUP(Clip a){
		
	}
	
	public static Clip makeNewSegment(){
		return new Clip();
	}
	
	public void deleteClip(int index){
		
	}
	
	public void repeatClip(int index, int times){
		//TODO repeat
	}
	
	public void addClip(Clip a){
		if(usedClips.size() < 8){
			usedClips.add(a);
		}
	}
	
	public Clip cutSegment(int start, int end){
		//look through all clips for sound between start and end times
		return new Clip();
	}
	
	
	
}
