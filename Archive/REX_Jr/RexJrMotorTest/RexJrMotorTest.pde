/*
Initial Test of motor polarity - Fwd/Rev/Left/Right
 
 */

// delay beteween movements (milliseconds)
#define MOTORDELAY 2000

int motorRightFwd   =  7;    // right motor foward 
int motorRightRev   =  8;    // right motor reverse 
int motorLeftFwd    = 12;    // left motor forward
int motorLeftRev    = 13;    // left motor referse
int motorRightSpeed = 10;    // enable pin for right motor - pwm speed control
int motorLeftSpeed  = 11;    // enable pin for left motor - pwm speed control

// The setup() method runs once, when the sketch starts

void setup()   {             
  
  // initialize the digital pins as an output and set motor speeds to zero
  analogWrite(motorRightSpeed, 0); 
  analogWrite(motorLeftSpeed, 0); 
  pinMode(motorRightFwd, OUTPUT);     
  pinMode(motorRightRev, OUTPUT);
  pinMode(motorLeftFwd, OUTPUT);
  pinMode(motorLeftRev, OUTPUT);
  
 
}

// the loop() method runs over and over again,
// as long as the Arduino has power

void loop()                     
{
  // delay 2 seconds
  delay(MOTORDELAY);
  
  // Foward
  digitalWrite(motorRightRev, LOW);
  digitalWrite(motorRightFwd, HIGH);  
  digitalWrite(motorLeftRev, LOW);
  digitalWrite(motorLeftFwd, HIGH);  
  analogWrite(motorRightSpeed, 255/4);  // quarter speed
  analogWrite(motorLeftSpeed, 255/4);  // quarter speed
  
  // delay 2 seconds
  delay(MOTORDELAY);
  
  // stop - brake
  digitalWrite(motorRightRev, LOW);
  digitalWrite(motorRightFwd, LOW);  
  digitalWrite(motorLeftFwd, LOW);  
  digitalWrite(motorLeftRev, LOW);
  analogWrite(motorRightSpeed, 0);
  analogWrite(motorLeftSpeed, 0);  
  
  // delay 2 seconds
  delay(MOTORDELAY);
  
  // Reverse
  digitalWrite(motorRightFwd, LOW);  
  digitalWrite(motorRightRev, HIGH);
  digitalWrite(motorLeftFwd, LOW); 
  digitalWrite(motorLeftRev, HIGH);
  analogWrite(motorRightSpeed, 255/4);  // quarter speed
  analogWrite(motorLeftSpeed, 255/4);  // quarter speed
  
  // delay 2 seconds
  delay(MOTORDELAY);
   
  // stop - brake
  digitalWrite(motorRightRev, LOW);
  digitalWrite(motorRightFwd, LOW);  
  digitalWrite(motorLeftFwd, LOW);  
  digitalWrite(motorLeftRev, LOW);
  analogWrite(motorRightSpeed, 0);
  analogWrite(motorLeftSpeed, 0);  
  
  // delay 2 seconds
  delay(MOTORDELAY);
  
  //spin left -  right fwd / left rev
  digitalWrite(motorRightRev, LOW);
  digitalWrite(motorRightFwd, HIGH);  
  digitalWrite(motorLeftFwd, LOW);  
  digitalWrite(motorLeftRev, HIGH);
  analogWrite(motorRightSpeed, 255/4);  // quarter speed
  analogWrite(motorLeftSpeed, 255/4);  // quarter speed
  
  // delay 2 seconds
  delay(MOTORDELAY);
  
  // stop - brake
  digitalWrite(motorRightRev, LOW);
  digitalWrite(motorRightFwd, LOW);  
  digitalWrite(motorLeftFwd, LOW);  
  digitalWrite(motorLeftRev, LOW);
  analogWrite(motorRightSpeed, 0);
  analogWrite(motorLeftSpeed, 0);  
  
  // delay 2 seconds
  delay(MOTORDELAY);
  
  //spin right -  right fwd / left rev
  digitalWrite(motorRightFwd, LOW);
  digitalWrite(motorRightRev, HIGH);
  digitalWrite(motorLeftRev, LOW);
  digitalWrite(motorLeftFwd, HIGH);  
  analogWrite(motorRightSpeed, 255/4);  // quarter speed
  analogWrite(motorLeftSpeed, 255/4);  // quarter speed
  
  // delay 2 seconds
  delay(MOTORDELAY);
  
  // stop - brake
  digitalWrite(motorRightRev, LOW);
  digitalWrite(motorRightFwd, LOW);  
  digitalWrite(motorLeftFwd, LOW);  
  digitalWrite(motorLeftRev, LOW);
  analogWrite(motorRightSpeed, 0);
  analogWrite(motorLeftSpeed, 0);  
  
  // delay 2 seconds
  delay(MOTORDELAY);
  
  //Right motor forward
  digitalWrite(motorRightRev, LOW);
  digitalWrite(motorRightFwd, HIGH);   
  analogWrite(motorRightSpeed, 255/2);  
  
  // delay 2 seconds
  delay(MOTORDELAY);
  
  // stop the right motor
  analogWrite(motorRightSpeed, 0); 
  
  // delay 2 seconds
  delay(MOTORDELAY);
  
  // Right motor reverse
  digitalWrite(motorRightRev, HIGH);
  digitalWrite(motorRightFwd, LOW);   
  analogWrite(motorRightSpeed, 255/2);  
  
  // delay 2 seconds
  delay(MOTORDELAY);
  
  // stop the right motor
  analogWrite(motorRightSpeed, 0); 
  
  // delay 2 seconds
  delay(MOTORDELAY);
  
  //left motor forward
  digitalWrite(motorLeftRev, LOW);
  digitalWrite(motorLeftFwd, HIGH);   
  analogWrite(motorLeftSpeed, 255/2);  
  
  // delay 2 seconds
  delay(MOTORDELAY);
  
  // stop the left motor
  analogWrite(motorLeftSpeed, 0); 
  
  // delay 2 seconds
  delay(MOTORDELAY);
  
  //left motor forward
  digitalWrite(motorLeftRev, HIGH);
  digitalWrite(motorLeftFwd, LOW);   
  analogWrite(motorLeftSpeed, 255/2);  
  
  // delay 2 seconds
  delay(MOTORDELAY);
  
   // stop the left motor
  analogWrite(motorLeftSpeed, 0); 


}
