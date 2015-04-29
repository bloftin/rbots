/*
  For Loop Iteration
 
 Demonstrates the use of a for() loop. 
 Lights multiple LEDs in sequence, then in reverse
 with the speed controlled by the potentiometer reading
 
 
 created 2006
 by David A. Mellis
 modified 5 Jul 2009
 by Tom Igoe 
 modified 20 Jul 2010
 by Ben Loftin - removed 1 led pin for starter kit
 modified 31 Jul 2010
 by Ion Bealer - connected pot to the cylon eyes

This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/ForLoop
 */

int timer = 50;           // The higher the number, the slower the timing.

int potPin = 2;   //select the input pin for the potentiometer
int val = 0;      // variable to store the value coming from the sensor


void setup() {
  // use a for loop to initialize each pin as an output:
  for (int thisPin = 9; thisPin < 13; thisPin++)  {
    pinMode(thisPin, OUTPUT);      
  }
}

void loop() {
  val = analogRead(potPin);   //read the value from the sensor
  timer = val/10;
  // loop from the lowest pin to the highest:
  for (int thisPin = 9; thisPin < 12; thisPin++) { 
    // turn the pin on:
    digitalWrite(thisPin, HIGH);   
    delay(timer);                  
    // turn the pin off:
    digitalWrite(thisPin, LOW);    
  }

  // loop from the highest pin to the lowest:
  for (int thisPin = 12; thisPin >= 9; thisPin--) { 
    // turn the pin on:
    digitalWrite(thisPin, HIGH);
    delay(timer);
    // turn the pin off:
    digitalWrite(thisPin, LOW);
  }
}
