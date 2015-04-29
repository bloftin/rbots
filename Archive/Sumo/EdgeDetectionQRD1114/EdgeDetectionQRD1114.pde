/*

Reads the QRD1114 Sensor into analog pin 0, if it detects a white line it toggles digital pin 9 high.  The threshold of 100 for white was chosen through trial.  The QRD1114 needs to be 4-6 mm next to the surface.
 
RTEAM Robotics Club
Ben Loftin 9/18/2010

GPL 3 or greater
 
 */

// Analog input pin to read QRD1114
const int analogInPin = 0;
// Digital output pin that the LED is attached to turn high
const int OutPin = 9; 
// set default sensor value large - not white line
int sensorValue = 10000;  
// set threshold for white line detection
int whiteThreshold = 100;  

void setup() {
  // initialize the digital pin as an output:
  pinMode(OutPin, OUTPUT);
  
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);              
  

  if(sensorValue < whiteThreshold) {
    // change the analog out value:
    digitalWrite(OutPin, HIGH);   
  }    
  else
  {
    digitalWrite(OutPin, LOW);
  }

  // print the results to the serial monitor:
  Serial.print("sensor = " );                       
  Serial.println(sensorValue);        

  // wait 10 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(10);                     
}
