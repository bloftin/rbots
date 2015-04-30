package com.examples.tutorial2d;

import android.app.Activity;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;

public class Tutorial2DActivity extends Activity {
	
	private float mCanvasWidth = 0.0f;
	private float mCanvasHeight = 0.0f;
	private float gridWidth = 60.0f;
	private float offset = 20.0f;
	private float touchX = 0.0f;
	private float touchY = 0.0f;
	private float touchXprev = 0.0f;
	private float touchYprev = 0.0f;
	private int touchCount = 0;
	private boolean targetColor = true;
	private boolean initTarget = true;
	
	
	 
	class Panel extends View {
	    public Panel(Context context) {
	        super(context);
	    }
	    
	    @Override
        public boolean onTouchEvent(MotionEvent e) {
	    	
	    	if( e.getAction()  == MotionEvent.ACTION_UP)
	    	{
		    	touchXprev = touchX;
		    	touchYprev = touchY;
		    	touchX = e.getX();
		    	touchY = e.getY();
		    	if (touchCount == 0 )
		    	{
		    		touchCount = 0;
		    		touchXprev = 0.0f;
		    		touchYprev = 0.0f;
		    		targetColor = false;
		    		touchCount++;
		    	}
		    	else if (touchCount == 1)
		    	{
		    		touchCount++;
		    		targetColor = true;
		    		
		    	}
		    	else
		    	{
		    		touchCount = 0;
		    		touchXprev = 0.0f;
		    		touchYprev = 0.0f;
		    		targetColor = false;
		    		
		    	}
		    	
	            invalidate();
	    	}
            return true;
        }
	 
	    @Override
	    public void onDraw(Canvas canvas) {

            canvas.drawColor(Color.WHITE);
            mCanvasHeight = this.getHeight();
            mCanvasWidth =  this.getWidth();
            
            int intWidthCount =  (int) Math.floor( (mCanvasWidth-2*offset) / gridWidth);
            int intHeigthCount = (int) Math.floor( (mCanvasHeight-2*offset) / gridWidth);
            
            Paint rectPaint = new Paint(Color.RED);
            rectPaint.setStyle(Style.STROKE);
            rectPaint.setColor(Color.RED);
            
            
            if (touchXprev >= offset && 
                	touchXprev <= intWidthCount*gridWidth+offset &&
                	touchYprev >= offset &&
                	touchYprev <= intHeigthCount*gridWidth+offset &&
                	touchCount != 0 )
            {
                	int touchXstart = (int) Math.floor( (touchXprev+offset) / gridWidth);
                	int touchYstart  = (int) Math.floor( (touchYprev+offset) / gridWidth);
                	
                	if (touchXstart*gridWidth+gridWidth <  intWidthCount*gridWidth+offset &&
                		touchYstart*gridWidth+gridWidth < intHeigthCount*gridWidth+offset)
                	{
                		Paint paintSq = new Paint();
                		paintSq.setStyle(Style.FILL_AND_STROKE);
                		paintSq.setColor(Color.GREEN);
                		canvas.drawRect(touchXstart*gridWidth+offset, 
                			        touchYstart*gridWidth+offset,
                			        touchXstart*gridWidth+gridWidth+offset,
                			        touchYstart*gridWidth+gridWidth+offset,
                			        paintSq);
                		//canvas.drawPaint(rectPaint); 
                	}
            }
            
            // if touch is within the rectangle, draw the target square
            if (touchX >= offset && 
            	touchX <= intWidthCount*gridWidth+offset &&
            	touchY >= offset &&
            	touchY <= intHeigthCount*gridWidth+offset &&
            	touchCount != 0 )
            {
            	int touchXstart = (int) Math.floor( (touchX+offset) / gridWidth);
            	int touchYstart  = (int) Math.floor( (touchY+offset) / gridWidth);
            	
            	if (touchXstart*gridWidth+gridWidth <  intWidthCount*gridWidth+offset &&
            		touchYstart*gridWidth+gridWidth < intHeigthCount*gridWidth+offset)
            	{
            		Paint paintSq = new Paint();
            		paintSq.setStyle(Style.FILL_AND_STROKE);
            		if(targetColor && !initTarget)
            			paintSq.setColor(Color.RED);
            		else
            		{
            			initTarget = false;
            			paintSq.setColor(Color.GREEN);
            		}
            		
            		canvas.drawRect(touchXstart*gridWidth+offset, 
            			        touchYstart*gridWidth+offset,
            			        touchXstart*gridWidth+gridWidth+offset,
            			        touchYstart*gridWidth+gridWidth+offset,
            			        paintSq);
            	}
            }
            // left, top, right, bottom, paint args
            canvas.drawRect(offset, offset , intWidthCount*gridWidth+offset, intHeigthCount*gridWidth+offset, rectPaint);
            //canvas.drawText("touchX = " + touchX + ", touchY = " + touchY, 50.0f, 50.0f, new Paint(Color.BLACK));
//            canvas.drawText("mCanvasHeight = " + mCanvasHeight, offset, offset, new Paint(Color.BLACK));
//            canvas.drawText("mCanvasWidth = " + mCanvasWidth, offset, offset+50, new Paint(Color.BLACK));
//            canvas.drawText("this.getHeight(); = " + this.getHeight(), offset, offset+100, new Paint(Color.BLACK));
//            canvas.drawText("this.getWidth() = " + this.getWidth(), offset, offset+150, new Paint(Color.BLACK));
//            canvas.drawText("intWidthCount = " + intWidthCount, offset, offset+200, new Paint(Color.BLACK));
//            canvas.drawText("intHeigthCount = " + intHeigthCount, offset, offset+250, new Paint(Color.BLACK));
//            canvas.drawText("widthExtra = " + widthExtra, offset, offset+300, new Paint(Color.BLACK));
//            canvas.drawText("heigthExtra = " + heigthExtra, offset, offset+350, new Paint(Color.BLACK));
            
            // draw Horizontal lines
            for(float i = offset; i <= intHeigthCount*gridWidth+offset; i+=gridWidth)
            {
            	//canvas.drawLine(startX, startY, stopX, stopY, paint)
            	canvas.drawLine(offset, i, intWidthCount*gridWidth+offset, i, new Paint(Color.RED));
            }
            
            // draw Vertical lines
            for(float i = offset; i <= intWidthCount*gridWidth+offset; i+=gridWidth)
            {
            	canvas.drawLine(i,offset,i, intHeigthCount*gridWidth+offset, new Paint(Color.RED));
            }
	    }

	}
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        final Panel panelView = new Panel(this);
        setContentView(panelView);
        
        
    }
}