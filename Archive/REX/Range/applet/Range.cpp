/*
  REX Systems Test
 
 IF Infrared sensor detects object less than 10 cm, it reverese motors

 */

#include "WProgram.h"
void setup();
void loop();
void establishContact();
float read_gp2d120_range(byte pin);
float read_gp2d12_range(byte pin);
int ledPin =  13;    // LED connected to digital pin 13
int enablePin2 = 2;
int enablePin3 = 3;
int enablePin4 = 4;
int enablePin5 = 5;
float IR1 = 0.0;

// The setup() method runs once, when the sketch starts

void setup()   {                
  // initialize the digital pin as an output:
  pinMode(ledPin, OUTPUT);
  pinMode(enablePin2, OUTPUT);
  pinMode(enablePin3, OUTPUT);
  pinMode(enablePin4, OUTPUT);
  pinMode(enablePin5, OUTPUT);
  Serial.begin(9600);
  establishContact();  // send a byte to establish contact until receiver responds 
}

// the loop() method runs over and over again,
// as long as the Arduino has power

void loop()                     
{
  //  IR1 is in cm
  IR1 = read_gp2d120_range(0);
  Serial.println(IR1);
  delay(400);
  digitalWrite(enablePin4, HIGH);
  digitalWrite(enablePin3, HIGH);
  if ( (int) IR1 < 10 && (int) IR1 > 0)
  {
      // go in reverse if sensor reads less than 10 cm
      digitalWrite(enablePin5, LOW);  
      digitalWrite(enablePin2, HIGH);
  }
  else
  {
      // go foward if sensor reads less than 10 cm  
      digitalWrite(enablePin5, HIGH);
      digitalWrite(enablePin2, LOW);
  }
 
  //delay(3000);    
  //Serial.println(analogRead(2));
  // wait for a second
  //digitalWrite(ledPin, LOW);    // set the LED off
  //delay(3000);                  // wait for a second
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print('A', BYTE);   // send a capital A
    delay(300);
  }
}

float read_gp2d120_range(byte pin) {
	int tmp;

	tmp = analogRead(pin);
	if (tmp < 3)
		return -1; // invalid value

	return (2914.0 /((float)tmp + 5.0)) - 1.0;
} 

float read_gp2d12_range(byte pin) {
	int tmp;

	tmp = analogRead(pin);
	if (tmp < 3)
		return -1; // invalid value

	return (6787.0 /((float)tmp - 3.0)) - 4.0;
} 


int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

