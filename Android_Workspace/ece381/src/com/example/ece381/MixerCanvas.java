package com.example.ece381;

import java.util.ArrayList;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Point;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class MixerCanvas extends SurfaceView implements SurfaceHolder.Callback{

	 private final int barwidth = 50;
	 public static int progress = 0;
	 DrawThread drawThread;
	 public int space = 50;
	 public static ArrayList <MixUIData> mD = new ArrayList <MixUIData>();
	 public int selIndex = -1;
	 
	 public static int playScroll;
	 public boolean toPlay = false;

	public MixerCanvas(Context context) {
		super(context);
	}
	
	public MixerCanvas(Context context, AttributeSet attributeSet) {
	    super(context, attributeSet);
	    getHolder().addCallback(this);
	}
	
	@Override
	protected void onDraw(Canvas canvas) {
		if(canvas == null) {
			return;
		}
		int offset = progress % 100;
		Paint paint = new Paint();
		paint.setStyle(Paint.Style.STROKE);
		paint.setColor(Color.YELLOW);
		paint.setStrokeWidth(5);

		//Draw the bar
		if(offset < 50 ){
			canvas.drawLine(50 - offset, 0, 50 - offset, barwidth/2 , paint);
		}
		for (int i = 0; i <= 20; i++) {
			if(i%2==0){
				int temp = i*space;
				canvas.drawLine(temp+(100-offset), 0, temp+(100-offset), barwidth , paint);
			}
			else{
				int temp = i*space;
				canvas.drawLine(temp+(100-offset), 0, temp+(100-offset), barwidth/2 , paint);
			}
		} 
		
		//draw the horizontal devides
		paint.setColor(Color.MAGENTA);
		for( int i = 1; i <= 6; i++){
			canvas.drawLine(0, i*100, 1000, i*100, paint );
		}
		
		
		//draw placed
		paint.setColor(Color.RED);
		paint.setStrokeWidth(60);
		for(MixUIData j: mD){
			canvas.drawLine(j.xstart - progress, (j.ystart *100)+ 50, (j.xstart+j.length)-progress, (j.ystart *100)+ 50, paint);
		}
		
		if(selIndex >= 0 && mD.size()>selIndex){
		paint.setColor(Color.CYAN);
		MixUIData j = mD.get(selIndex);
		canvas.drawLine(j.xstart - progress, (j.ystart *100)+ 50, (j.xstart+j.length)-progress, (j.ystart *100)+ 50, paint);}
		if(selIndex >= mD.size()){
			selIndex = -1;
		}
	
		if(toPlay){
			drawPlaybar(canvas);
			this.invalidate();
		}
	
	
	}

	@Override
	public void surfaceChanged(SurfaceHolder arg0, int arg1, int arg2, int arg3) {
		
	}

	@Override
	public void surfaceCreated(SurfaceHolder arg0) {
		drawThread = new DrawThread(this);
		drawThread.setRunning(true);
		drawThread.start();
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder arg0) {
	    boolean retry = true;
	    drawThread.setRunning(false);
	    while (retry) {
	        try {
	           	drawThread.join();
	            retry = false;
	        } catch (InterruptedException e) {
	        	e.printStackTrace();
	        }
	    }
	}
	
	
	private class DrawThread extends Thread{
		 private boolean toRun = false;
		 private MixerCanvas mixerCanvas;
		 private SurfaceHolder surfaceHolder;
		 
		 public DrawThread(MixerCanvas m) {
			mixerCanvas = m;
			surfaceHolder = mixerCanvas.getHolder();
		 }
		 public void setRunning(boolean run) {
		        toRun = run;
		 }
		
		@Override
		public void run() {
			 Canvas c = null;
			 while (toRun) {
				 try {
					 c = surfaceHolder.lockCanvas(null);
					 synchronized(this.surfaceHolder){
					 mixerCanvas.onDraw(c);}
				 } finally {
					 if (c != null) {
						 surfaceHolder.unlockCanvasAndPost(c);
					 }
				 }
			 }
		 }
	}

	public void setProgress(int progress) {
		this.progress = progress;
	}
	
	
	
	private class MixUIData{
		
		int length;
		int iD;
		int xstart;
		int ystart;
		int mixIndex;
		
		public MixUIData(){
			notifyClip();
		}
		
		public MixUIData( int x, int y ){
			iD = findID();
			mixIndex = MixerActivity.indexOfSel;
			xstart = x + progress;
			ystart = y;
			length = MixerActivity.theMix.getClipat(MixerActivity.indexOfSel).getLength();
			
			//System.out.println(MixerActivity.indexOfSel + "   " + MixerActivity.theMix.lengthOf()  + "  " + length);
			notifyClip();
		}
		
		public void saveClips(){
			
		}
		
		public int findID(){
			int temp = MixerActivity.theMix.getClipat(MixerActivity.indexOfSel).getID();
			return temp;
		}
		
		public void notifyClip(){
				MixerActivity.theMix.getClipat(mixIndex).setToPlayAt(xstart);
				//System.out.println(MixerActivity.theMix.getClipat(mixIndex).timesUsed().iterator());
		}
		
	}

	public void deleteAllInstanceOf(int idOfSongSelected) {
		for(int i = 0; i < mD.size(); i++){
			if(mD.get(i).iD == idOfSongSelected){
				mD.remove(i);
			}
		}		
		MixerActivity.theMix.removeAllInstanceOf(MixerActivity.indexOfSel);
	}

	public void deleteSelectedSeg() {
		int tempindex = -1;
		for(Clip p :MixerActivity.theMix.listContents()){
			if(p.getID() == mD.get(selIndex).iD){
				tempindex = MixerActivity.theMix.listContents().indexOf(p);
			}
		}
		if(tempindex !=-1){
		MixerActivity.theMix.removeClipPlay(tempindex, mD.get(selIndex).xstart);
		mD.remove(selIndex);
		selIndex = -1;}
		
		this.invalidate();
	}


	public void addElement(Point p) {
		if(!trySelElement(p)&& !aliastest(p) && p.y!= 0){
		mD.add(new MixUIData(p.x, p.y));
		this.invalidate();
		}
	}

	private boolean aliastest(Point p) {
		int temp =100;//length of currently select song
		temp = MixerActivity.theMix.getClipat(MixerActivity.indexOfSel).getLength();
		for(MixUIData j: mD){
			if( j.ystart == p.y && 
					p.x+progress + temp >= j.xstart &&
					p.x+progress + temp < j.xstart +j.length){
					//Log.v("selected", j.toString());
				return true;
			}
		}
		return false;
	}
	
	private boolean aliastest(Point p, int length) {
		int temp = length;
		
		if(mD.get(selIndex).ystart == p.y){
			for(MixUIData j: mD){
				if(mD.get(selIndex).xstart != j.xstart){
					if(j.ystart == p.y && 
							p.x+progress + temp >= j.xstart &&
							p.x+progress + temp < j.xstart +j.length){
						return true;
					}
				}
			}
			return false;
		}
		for(MixUIData j: mD){
			if( j.ystart == p.y && 
					p.x+progress + temp >= j.xstart &&
					p.x+progress + temp < j.xstart +j.length){
					//Log.v("selected", j.toString());
				
				return true;
			}
		}
		return false;
	}

	public boolean trySelElement(Point p) {
		for(MixUIData j: mD){
			if( (p.x + progress >= j.xstart)&& 
					(p.x+progress) <= (j.xstart+j.length)&&
					p.y == j.ystart){
				MixerActivity.idOfSongSelected = 0;
				MixerActivity.indexOfSel = -1;
				MixerActivity.selSong = false;
				MixerActivity.songList.setSelected(false);
				//Log.v("indexj", Integer.toString(mD.indexOf(j)));
				
				selIndex = mD.indexOf(j);
				this.invalidate();
				return true;
			}
		}
		
		this.invalidate();
		return false;
	}
	
	public void replace(Point p){
		if(selIndex >=0 && !trySelElement(p) && !aliastest(p , mD.get(selIndex).length)){
			MixerActivity.theMix.getClipat(mD.get(selIndex).mixIndex).setToPlayAt(p.x + progress);
			MixerActivity.theMix.removeClipPlay(mD.get(selIndex).mixIndex, mD.get(selIndex).xstart);
			mD.get(selIndex).xstart = p.x + progress;
			mD.get(selIndex).ystart = p.y;
		}
		this.invalidate();
	}
	
	public void drawPlaybar(Canvas c){
		Paint p = new Paint();
		p.setColor(Color.LTGRAY);
		p.setStrokeWidth(5);
		c.drawLine(playScroll, 0, playScroll, 600, p);
		/*if(playScroll%2 ==0){
			this.invalidate();
		}*/
		if(playScroll > 3000){
			toPlay = false;
		}
		playScroll++;
	}
	
	
	public void checkClips(){
		for(MixUIData u : mD){
			for(Clip p: MixerActivity.theMix.listContents()){
				if(u.iD == p.getID()){
					for(Integer t: p.timesUsed()){
						if(u.xstart == t){
							u.length = p.getLength();
							//Log.v("inside final checkclips", Integer.toString(u.length) + Integer.toString(p.getLength()));
						}
					}
				}
			}
		}
	}
	
	public void ClipEditedLength(){
		mD.get(selIndex).length = MixerActivity.theMix.getClipat(mD.get(selIndex).mixIndex).getLength();
	}
}
