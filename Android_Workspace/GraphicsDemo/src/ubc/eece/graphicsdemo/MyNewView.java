package ubc.eece.graphicsdemo;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.LinearGradient;
import android.graphics.Paint;
import android.graphics.RectF;
import android.graphics.Shader;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;
public class MyNewView extends View {
	public MyNewView(Context context, AttributeSet attribute_set) {
		super(context, attribute_set);
	}
	
	@Override
	public void onDraw(Canvas canvas) {
		canvas.drawColor(Color.BLACK);
		
		Paint paint = new Paint();
		canvas.rotate(10);
		Bitmap b = null;
		try {
			b = BitmapFactory.decodeResource(getResources(), R.drawable.ic_launcher);
			canvas.drawBitmap(b, null, new RectF(0, 0, 512, 512), paint);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			Log.i("MY_MESSAGE", "Bitmap file error");
		}
		int max_x = getWidth()-1;
		int max_y = getHeight()-1;
		// Draw a white X
		paint.setColor(Color.WHITE);
		paint.setStrokeWidth(1);
		canvas.drawLine(0, 0, max_x, max_y, paint);
		canvas.drawLine(0, max_y, max_x, 0, paint);
		paint.setShader(new LinearGradient(
				40,
				40,
				100,
				100,
				Color.RED,
				Color.GREEN,
				Shader.TileMode.MIRROR) );
		// Draw a thick red vertical line
		paint.setColor(Color.RED);
		paint.setStrokeWidth(10);
		canvas.drawLine(max_x / 3, 0, max_x/3, max_y, paint);
		// Draw a series of green lines of various transparencies
		paint.setColor(Color.GREEN);
		paint.setStrokeWidth(5);
		int alpha = 255;
		for(int y = max_y / 10; y <= 9*max_y/10; y+= max_y/10) {
			paint.setAlpha(alpha);
			canvas.drawLine(0, y, max_x, y, paint);
			alpha = alpha - 30;
		}
		paint.setShadowLayer(1, -8, 7, Color.YELLOW);
		// Draw some text
		paint.setAlpha(255);
		canvas.drawText("EECE 381 is great", 50, 50, paint );
		
	}
}