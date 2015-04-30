package com.rbots.ra;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

import android.bluetooth.BluetoothServerSocket;
import android.content.Context;


public class BluetoothHelloActivity extends Activity {
	private static final UUID SerialPortServiceClass_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
	
	// Message types sent from the BluetoothChatService Handler
    public static final int MESSAGE_READ = 2;
	
	private BluetoothAdapter mBtAdapter;
	private ConnectedThread mConnectedThread;
	private BluetoothDevice mDevice;
	//private byte mTestByte = 0;
	private byte [] mTestByte = {33};
	
	private int mBluetoothState = 0;
	
	BluetoothSocket blueSocket = null; 
	
	 @Override
     public boolean onTouchEvent(MotionEvent e) {
	    	
	    	if( e.getAction()  == MotionEvent.ACTION_UP)
	    	{
	    		if(mBluetoothState == 1)
	   		 	{
	    			if(mTestByte[0] == 126)
	    			{
	    				mTestByte[0] = 10; // line feed
	    				mConnectedThread.write(mTestByte);
	    				mTestByte[0] = 33;
	    			}
	    			mTestByte[0] ++;//mTestByte[0] + 0x01;
	                mConnectedThread.write(mTestByte);
	   		 	}
		    	
	    	}
         return true;
     }
	
	@Override
	public boolean onKeyDown(int keycode, KeyEvent event ) {
	 if(keycode == KeyEvent.KEYCODE_MENU){
		 
		 if(mBluetoothState == 1)
		 {
			 mBluetoothState = 0;
			 // close bluetooth connection on menu button press
			 try {
				 blueSocket.close();
	         } catch (IOException e2) {
	      	   //tv.append("Bluetooth device close during connect failed" + e2);
	           //finish();
	         }
		 }
		 else if(mBluetoothState == 0)
		 {
			 try {
                 //next try connections
				 blueSocket = mDevice.createRfcommSocketToServiceRecord(SerialPortServiceClass_UUID);
                 blueSocket.connect();
                 //tv.append("Bluetooth device connection success");
                 
                 // Start the thread to manage the connection and perform transmissions
                 mConnectedThread = new ConnectedThread(blueSocket);
                 mConnectedThread.start();

                 mBluetoothState = 1;
               
                 
                 byte [] btWrite = {'a'};
                 mConnectedThread.write(btWrite);

             } catch (IOException e) {
          	   //tv.append("Bluetooth device connect failed" + e);
             }
		 
		 }
		 
	 }
	 return super.onKeyDown(keycode,event);  
	}
	
	 private final Handler mHandler = new Handler() {
	        @Override
	        public void handleMessage(Message msg) {
	            String address = null;
	            switch (msg.what) {
		            case MESSAGE_READ:
		                byte[] readBuf = (byte[]) msg.obj;
		                // construct a string from the valid bytes in the buffer
		                String readMessage = new String(readBuf, 0, msg.arg1);
		                //mConversationArrayAdapter.add(mConnectedDeviceName+":  " + readMessage);
		                break;
	           
	            }
	        }
	    };
	    
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        TextView tv = new TextView(this);
        
        // Get the local Bluetooth adapter
        mBtAdapter = BluetoothAdapter.getDefaultAdapter();
        // Get a set of currently paired devices
        Set<BluetoothDevice> pairedDevices = mBtAdapter.getBondedDevices();
        
        // If there are paired devices, add each one to the ArrayAdapter
        if (pairedDevices.size() > 0) {
            for (BluetoothDevice device : pairedDevices) {
               tv.append("Device Name: " + device.getName() + "\n" + "Address: " + device.getAddress());
               
               // If the name is what we expect to be RA host computer, then connect
               // and establish serial connection
               if (device.getName().equals("RA-PC"))
               {
            	   try {
            		   mDevice = device;
            		   blueSocket = mDevice.createRfcommSocketToServiceRecord(SerialPortServiceClass_UUID);
                       
                       //next try connections
                       blueSocket.connect();
                       tv.append("Bluetooth device connection success");
                       
                       // Start the thread to manage the connection and perform transmissions
                       mConnectedThread = new ConnectedThread(blueSocket);
                       mConnectedThread.start();

                       mBluetoothState = 1;
                       

                   } catch (IOException e) {
                	   tv.append("Bluetooth device connect failed" + e);
                   }
               }
            }
        } else {
	           tv.append("No Paired Devices\n");
        }
        
        // do random bs
        for(int i=0;i<1000000;i++)
        {
           int j =1;	
        }
        
        byte [] btWrite = {'a'};
        mConnectedThread.write(btWrite);
        // close connection
//        try {
//     	   blueSocket.close();
//        } catch (IOException e3) {
//     	   tv.append("Bluetooth device close during exit failed" + e3);
//        }        
//              
        setContentView(tv);
    }
    
    private class ConnectedThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;
     
        public ConnectedThread(BluetoothSocket socket) {
            mmSocket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;
     
            // Get the input and output streams, using temp objects because
            // member streams are final
            try {
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException e) { }
     
            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }
     
        public void run() {
            byte[] buffer = new byte[1024];  // buffer store for the stream
            int bytes; // bytes returned from read()
     
            // Keep listening to the InputStream until an exception occurs
            while (true) {
                try {
                    // Read from the InputStream
                    bytes = mmInStream.read(buffer);
                    
					// Send the obtained bytes to the UI Activity
                    mHandler.obtainMessage(MESSAGE_READ, bytes, -1, buffer)
                            .sendToTarget();
                } catch (IOException e) {
                    break;
                }
            }
        }
     
        /* Call this from the main Activity to send data to the remote device */
        public void write(byte[] bytes) {
            try {
                mmOutStream.write(bytes);
            } catch (IOException e) { }
        }
     
        /* Call this from the main Activity to shutdown the connection */
        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) { }
        }
    }
}