/*
 Object Detection for REX
 
 Go forward until front IR sensor detects something up at 10 cm and the stop
 
 */

int LeftMotorRev =  5;    // LEFT motor Reverse connected to PD5, pin 11
int LeftMotorFwd =  6;    // LEFT motor Forward connected to PD6, pin 12
int RightMotorRev= 7;     // RIGHT motor Reverse connected to PB0, pin 14
int RightMotorFwd = 8;    // RIGHT motor Forward connected to PD7, pin 13
int IRForward = 0;        // Forward middle looking sharp 120 ADC 0, pin 23
int IRLeftBack = 1;        // Back left looking sharp 120 ADC 1, pin 24

float IRFowardValue = 0.0;  // value to store IR measurement for Forward looking sharp 120
float IRLeftBackValue = 0.0; // value to store IR measurement for Back left looking sharp 120

// The setup() method runs once, when the sketch starts

void setup()   {                
  // initialize the digital pin as an output:
  pinMode(LeftMotorRev, OUTPUT);  
  pinMode(LeftMotorFwd, OUTPUT); 
  pinMode(RightMotorRev, OUTPUT); 
  pinMode(RightMotorFwd, OUTPUT);  
  digitalWrite(LeftMotorRev, LOW);
  digitalWrite(LeftMotorFwd, LOW);
  digitalWrite(RightMotorRev, LOW);
  digitalWrite(RightMotorFwd, LOW);
}

// the loop() method runs over and over again,
// as long as the Arduino has power

void loop()                     
{
  /*
  digitalWrite(LeftMotorRev, HIGH);
  delay(1000);                      // wait for a second
  digitalWrite(LeftMotorRev, LOW);
  digitalWrite(LeftMotorFwd, HIGH);
  delay(1000);                  // wait for a second
  digitalWrite(LeftMotorFwd, LOW);
  digitalWrite(RightMotorRev, HIGH);
  digitalWrite(RightMotorFwd, LOW);
  delay(1000);                  // wait for a second
  digitalWrite(RightMotorRev, LOW);
  digitalWrite(RightMotorFwd, HIGH);
  delay(1000);                  // wait for a second
  */
  
  IRLeftBackValue = read_gp2d120_range(IRLeftBack);
  
  if( (int) IRLeftBackValue > 8 && (int) IRLeftBackValue < 11  && (int) IRLeftBackValue > 0 )
  {
    digitalWrite(LeftMotorFwd, HIGH);
    digitalWrite(RightMotorFwd, HIGH);
    digitalWrite(LeftMotorRev, LOW);
    digitalWrite(RightMotorRev, LOW);
  }
  // turn left if too far from wall
  else if ( (int) IRLeftBackValue > 11 && (int) IRLeftBackValue > 0 )
  {
    digitalWrite(LeftMotorFwd, HIGH);
    digitalWrite(RightMotorFwd, HIGH);
    digitalWrite(LeftMotorRev, LOW);
    digitalWrite(RightMotorRev, LOW);
    
    IRLeftBackValue = read_gp2d120_range(IRLeftBack);
    //while( (int) IRLeftBackValue > 10 )
    //{
      digitalWrite(LeftMotorFwd, LOW);
      delay(500);
    //  IRLeftBackValue = read_gp2d120_range(IRLeftBack);
    //}
    digitalWrite(LeftMotorFwd, HIGH);

  }
  
  // turn right if too close to wall
  else if ( (int) IRLeftBackValue < 8 && (int) IRLeftBackValue > 0 )
  {
    digitalWrite(LeftMotorFwd, HIGH);
    digitalWrite(RightMotorFwd, HIGH);
    digitalWrite(LeftMotorRev, LOW);
    digitalWrite(RightMotorRev, LOW);
   // IRLeftBackValue = read_gp2d120_range(IRLeftBack);
    //while( (int) IRLeftBackValue < 9 )
    //{
      digitalWrite(RightMotorFwd, LOW);
      delay(500);
    //  IRLeftBackValue = read_gp2d120_range(IRLeftBack);
    //}
    digitalWrite(RightMotorFwd, HIGH);


  }
  
}

float read_gp2d12_range(byte pin) {
  int tmp;
  
  tmp = analogRead(pin);
  if(tmp < 3)
    return -1;  //invalid value
    
  return (6787.0 / ((float)tmp - 3.0))-4.0;
}

float read_gp2d120_range(byte pin) {
  int tmp;
  
  tmp = analogRead(pin);
  if(tmp < 3)
    return -1;  //invalid value
    
  return (2914.0 / ((float)tmp + 5.0))-1.0;
}
