package com.rbots.hellotts;

import java.util.Locale;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.speech.tts.*;
import android.speech.tts.TextToSpeech.OnInitListener;

public class HelloTTS extends Activity implements TextToSpeech.OnInitListener{
	
	
	private final int CHECK_TTS_DATA = 55;
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        Intent checkIntent = new Intent();
        checkIntent.setAction(TextToSpeech.Engine.ACTION_CHECK_TTS_DATA);
        startActivityForResult(checkIntent, CHECK_TTS_DATA);
    }
    private TextToSpeech mTts;
    protected void onActivityResult(
            int requestCode, int resultCode, Intent data) {
        if (requestCode == CHECK_TTS_DATA) {
            if (resultCode == TextToSpeech.Engine.CHECK_VOICE_DATA_PASS) {
                // success, create the TTS instance
                mTts = new TextToSpeech(this, this);
            } else {
                // missing data, install it
                Intent installIntent = new Intent();
                installIntent.setAction(
                    TextToSpeech.Engine.ACTION_INSTALL_TTS_DATA);
                startActivity(installIntent);
            }
        }
        
    }
    
    public void onInit(int version) {
  	
		if(mTts.isLanguageAvailable(Locale.US) >= TextToSpeech.LANG_AVAILABLE){
			mTts.setLanguage(Locale.US);
		}
    	String myText1 = "Sheri!";
    	String myText2 = "This is not the droid your looking for.";
    	mTts.speak(myText1, TextToSpeech.QUEUE_FLUSH, null);
    	mTts.speak(myText2, TextToSpeech.QUEUE_ADD, null);


    }
    protected void OnDestroy(){
    	super.onDestroy();
    	mTts.shutdown();
    }


}