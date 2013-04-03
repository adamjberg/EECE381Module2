package com.example.ece381;

import java.util.ArrayList;

import android.R.color;
import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.Point;
import android.graphics.Rect;
import android.graphics.drawable.Drawable.Callback;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Display;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

public class MixerCanvas extends SurfaceView implements SurfaceHolder.Callback{

	 private final int barwidth = 50;
	 public static int progress = 0;
	 DrawThread drawThread;
	 public int space = 50;
	 public static ArrayList <MixUIData> mD = new ArrayList <MixUIData>();
	 public int selIndex = -1;

	public MixerCanvas(Context context) {
		super(context);
	}
	
	public MixerCanvas(Context context, AttributeSet attributeSet) {
	    super(context, attributeSet);
	    getHolder().addCallback(this);
	}
	
	@Override
	protected void onDraw(Canvas canvas) {
		//Bitmap sweet = BitmapFactory.decodeResource(getResources(), R.drawable.ic_launcher);
		//canvas.drawColor(color.black);
		//canvas.drawBitmap(sweet, (float) Math.random()*100, (float) Math.random()*100, null);
		
		
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
		
		if(selIndex >= 0){
		paint.setColor(Color.CYAN);
		MixUIData j = mD.get(selIndex);
		canvas.drawLine(j.xstart - progress, (j.ystart *100)+ 50, (j.xstart+j.length)-progress, (j.ystart *100)+ 50, paint);}
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
			//TODO fix constructor
			iD = findID();
			xstart = x + progress;
			ystart = y;
			length = MixerActivity.theMix.getClipat(MixerActivity.indexOfSel).getLength();
			notifyClip();
		}
		
		public void saveClips(){
			
		}
		
		public int findID(){
			return -1;
		}
		
		public void notifyClip(){
			
		}
		
	}

	public void deleteAllInstanceOf(int idOfSongSelected) {
		for(int i = 0; i < mD.size(); i++){
			if(mD.get(i).iD == idOfSongSelected){
				mD.remove(i);
			}
		}
	}

	public void deleteSelectedSeg() {
		mD.remove(selIndex);
	}

	public void addElement(Point p) {
		if(!trySelElement(p)&& !aliastest(p)){
		mD.add(new MixUIData(p.x, p.y));
		this.invalidate();
		}
	}

	private boolean aliastest(Point p) {
	//TODO set 1000 to equal current selected song.length
		int temp =1000;
		for(MixUIData j: mD){
			if( j.ystart == p.y && 
					p.x+progress + temp >= j.xstart &&
					p.x+progress + temp < j.xstart +j.length){
					Log.v("selected", j.toString());
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
				MixerActivity.selSong = false;
				Log.v("indexj", Integer.toString(mD.indexOf(j)));
				selIndex = mD.indexOf(j);
				this.invalidate();
				return true;
			}
		}
		
		//TODO change when implemented
		MixerActivity.idOfSongSelected = 1;
		MixerActivity.selSong = true;
		selIndex = -1;
		this.invalidate();
		return false;
	}
}
