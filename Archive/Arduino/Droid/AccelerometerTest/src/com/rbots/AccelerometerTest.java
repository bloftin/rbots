package com.rbots;

import java.util.List;

import android.app.Activity;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener ;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;


public class AccelerometerTest extends Activity implements SensorEventListener  {
    private static final String MY_DEBUG_TAG = "Error Accel Main view";
    // sensor manager used to control the accelerometer sensor.
    SensorManager mSensorManager;
    
    Sensor AccSensor;
    Sensor OrientSensor;
    
    // accelerometer sensor values.
    private float mAccelX = 0;
    private float mAccelY = 0;
    private float mAccelZ = 0;
    
    private float mOrientX = 0;
    private float mOrientY = 0;
    private float mOrientZ = 0;
    
    List<Sensor> listSensors;
    
    TextView tv;
    
	/** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        /* Create a new TextView to display the parsing result later. */
        tv = new TextView(this); 
        
        mSensorManager = ( SensorManager ) getSystemService(SENSOR_SERVICE);
        listSensors = mSensorManager.getSensorList(Sensor.TYPE_ALL);
        
        /* Display the TextView. */
        this.setContentView(tv);

    }
    
    @Override 
    public void onSensorChanged(SensorEvent event) {
        synchronized (this) {

            if (event.sensor.getType() == Sensor.TYPE_ORIENTATION ) {
            	mOrientX = event.values[0];
            	mOrientY = event.values[1];
            	mOrientZ = event.values[2];
            	
            	
            }
            if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
            	mAccelX = event.values[0];
            	mAccelY = event.values[1];
            	mAccelZ = event.values[2];
            }  
            
            tv.setText("Accel X: " + mAccelX + "\n" +
     			   "Accel Y: " + mAccelY + "\n" +
     			   "Accel Z: " + mAccelZ + "\n" +
            	   "Orientation X: " + mOrientX + "\n" +
     			   "Orientation Y: " + mOrientY + "\n" +
     			   "Orientation Z: " + mOrientZ + "\n");
            
            for (Sensor s : listSensors) {
            	tv.append("Type: " + s.getType() + ", Name: " + s.getName() + ", Vendor: " + s.getVendor() + "\n");
            }

            
        }
    }
    @Override 
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }
    @Override
    protected void onResume() {
        super.onResume();
      // register this class as a listener for the orientation and accelerometer sensors
        
       // possible sensors for droid is accel/orientation, magnetometer, proximity, ambient light
        mSensorManager.registerListener(this, 
        		mSensorManager.getDefaultSensor(Sensor.TYPE_ORIENTATION), 
        				SensorManager.SENSOR_DELAY_GAME);
        mSensorManager.registerListener(this, 
        		mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), 
        			    SensorManager.SENSOR_DELAY_GAME);
    }
    
    @Override
    protected void onStop() {
        // unregister listener
    	mSensorManager.unregisterListener(this);
        super.onStop();
    }

//    private final SensorListener mSensorAccelerometer = new SensorListener() {
//
//        // method called whenever new sensor values are reported.
//        public void onSensorChanged(int sensor, float[] values) {
//            // grab the values required to respond to user movement.
//            mAccelX = values[0];
//            mAccelY = values[1];
//            mAccelZ = values[2];
//        }
//
//        // reports when the accuracy of sensor has change
//        // SENSOR_STATUS_ACCURACY_HIGH = 3
//        // SENSOR_STATUS_ACCURACY_LOW = 1
//        // SENSOR_STATUS_ACCURACY_MEDIUM = 2
//        // SENSOR_STATUS_UNRELIABLE = 0 //calibration required.
//        public void onAccuracyChanged(int sensor, int accuracy) {
//            // currently not used
//        }
//    };

}