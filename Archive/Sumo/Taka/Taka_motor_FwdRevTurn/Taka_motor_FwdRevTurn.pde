/*

Motor ID 1: Left motor
Motor ID 2: Right motor

Setting continuous motion for the AX-12 motors is done by setting 
both values for the CW Angle Limit an the CCW Angle Limit to zero
and then setting the goal speed

0x06:  CW Angle Limit (L)
0x07:  CW Angle Limit (H)
0x08:  CCW Angle Limit (L)
0x09:  CCW Angle Limit (H)

Set continuous mode address through 06-09

0x00 0x00 0x00 0x00

Turn Dir = 0 =>  CCW
Turn Dir = 1 => CW

Goal Speed Setting bits:

0-9: Speed Value
10:  Turn Direction
11-15: 0


Full Speed CCW

High         Low
0000111 11111111
   0x01     0xFF

Full Speed CW

0000011 11111111
   0x03     0xFF
   
RTEAM Robotics Club
GPL 3 or greater
Ben Loftin

*/

// led on packet left motor
byte ledOn01[8] = {0xff, 0xff, 0x01, 0x04, 0x03, 0x19, 0x01, 0x00};
// led off packet left motor
byte ledOff01[8] = {0xff, 0xff, 0x01, 0x04, 0x03, 0x19, 0x00, 0x00};

// led on packet right motor
byte ledOn02[8] = {0xff, 0xff, 0x02, 0x04, 0x03, 0x19, 0x01, 0x00};
// led off packet right motor
byte ledOff02[8] = {0xff, 0xff, 0x02, 0x04, 0x03, 0x19, 0x00, 0x00};

//Torque Enable left Motor
byte tqEn01[8] = {0xff,0xff,0x01,0x04,0x03,0x18,0x01,0x00};

//Torque Enable Right Motor
byte tqEn02[8] = {0xff,0xff,0x02,0x04,0x03,0x18,0x01,0x00};

//Continuous (Endless Turn) Mode on left motor
byte continuous01[11] = {0xff,0xff,0x01,0x07,0x03,0x06,0x00,0x00,0x00,0x00,0x00};

//Continuous (Endless Turn) Mode on right motor
byte continuous02[11] = {0xff,0xff,0x02,0x07,0x03,0x06,0x00,0x00,0x00,0x00,0x00};

//  CW full speed left motor
byte rev01FullSpeed[9] = {0xff,0xff,0x01,0x05,0x03,0x20,0xFF,0x03,0x00};
//  CW full speed right motor
byte fwd02FullSpeed[9] = {0xff,0xff,0x02,0x05,0x03,0x20,0xFF,0x03,0x00};

//  CCW full speed left motor
byte fwd01FullSpeed[9] = {0xff,0xff,0x01,0x05,0x03,0x20,0xFF,0x07,0x00};
//  CCW full speed right motor
byte rev02FullSpeed[9] = {0xff,0xff,0x02,0x05,0x03,0x20,0xFF,0x07,0x00};

// the digital pin that sets the enable for half duplex tx
int xmtEn = 2;

void setup()
{
  pinMode(xmtEn, OUTPUT);  // output enables half duplex transmit buffer 
  Serial.begin(1000000);    // sets 1 Mbps baudrate for uart
  
  //calculate checksum
  ledOn01[7] = ~(ledOn01[2]+ledOn01[3]+ledOn01[4]+ledOn01[5]+ledOn01[6]);
  ledOff01[7] = ~(ledOff01[2]+ledOff01[3]+ledOff01[4]+ledOff01[5]+ledOff01[6]);
  ledOn02[7] = ~(ledOn02[2]+ledOn02[3]+ledOn02[4]+ledOn02[5]+ledOn02[6]);
  ledOff02[7] = ~(ledOff02[2]+ledOff02[3]+ledOff02[4]+ledOff02[5]+ledOff02[6]);
  
  tqEn01[7] =  ~(tqEn01[2]+tqEn01[3]+tqEn01[4]+tqEn01[5]+tqEn01[6]);
  tqEn02[7] =  ~(tqEn02[2]+tqEn02[3]+tqEn02[4]+tqEn02[5]+tqEn02[6]);
  
  continuous01[10] = ~(continuous01[2] + continuous01[3] + continuous01[4] + 
                                continuous01[5] + continuous01[6] + continuous01[7] +
                                continuous01[8] + continuous01[9]);

  continuous02[10] = ~(continuous02[2] + continuous02[3] + continuous02[4] + 
                                continuous02[5] + continuous02[6] + continuous02[7] +
                                continuous02[8] + continuous02[9]); 
                                
  rev01FullSpeed[8] = ~( rev01FullSpeed[2] +  rev01FullSpeed[3] +  rev01FullSpeed[4] +  rev01FullSpeed[5] +
                             rev01FullSpeed[6] +  rev01FullSpeed[7]);
  rev02FullSpeed[8] = ~( rev02FullSpeed[2] +  rev02FullSpeed[3] +  rev02FullSpeed[4] +  rev02FullSpeed[5] +
                             rev02FullSpeed[6] +  rev02FullSpeed[7]);
  fwd01FullSpeed[8] = ~( fwd01FullSpeed[2] +  fwd01FullSpeed[3] +  fwd01FullSpeed[4] +  fwd01FullSpeed[5] +
                             fwd01FullSpeed[6] +  fwd01FullSpeed[7]);                           
  fwd02FullSpeed[8] = ~( fwd02FullSpeed[2] +  fwd02FullSpeed[3] +  fwd02FullSpeed[4] +  fwd02FullSpeed[5] +
                             fwd02FullSpeed[6] +  fwd02FullSpeed[7]);   
                             
  //Enable motor 1
  digitalWrite(xmtEn, HIGH); //Enables transmit buffer
    for (int i=0; i<8; i++)
      Serial.write(tqEn01[i]); //Sends motor enable packet
  delay(0);                  //Need this 0 delay for the transmission to complete
  digitalWrite(xmtEn, LOW);  //Disables transmit buffer, saw it on the scope
  delay(5);                  //5ms follows the pot smoothly, 100ms is jumpy, emperical
  
  //Enable motor 2
  digitalWrite(xmtEn, HIGH); //Enables transmit buffer
    for (int i=0; i<8; i++)
      Serial.write(tqEn02[i]); //Sends motor enable packet
  delay(0);                  //Need this 0 delay for the transmission to complete
  digitalWrite(xmtEn, LOW);  //Disables transmit buffer, saw it on the scope
  delay(5);                  //5ms follows the pot smoothly, 100ms is jumpy, emperical
  
  // set continous rotation motor 1
  digitalWrite(xmtEn, HIGH);  // enable transmit buffer
  for(int i = 0; i < 11; i++)
    Serial.write(continuous01[i]);
  delay(0);                   // need this 0 delay for the transmission to complete
  digitalWrite(xmtEn, LOW);   // disables transmit buffer
  delay(5);                  //5ms follows the pot smoothly, 100ms is jumpy, emperical
  
  // set continous rotation motor 2
  digitalWrite(xmtEn, HIGH);  // enable transmit buffer
  for(int i = 0; i < 11; i++)
    Serial.write(continuous02[i]);
  delay(0);                   // need this 0 delay for the transmission to complete
  digitalWrite(xmtEn, LOW);   // disables transmit buffer
  delay(5);                  //5ms follows the pot smoothly, 100ms is jumpy, emperical
  
}

void loop()
{
  // turn on led motor 1
  digitalWrite(xmtEn, HIGH);  // enable transmit buffer
  for(int i = 0; i < 8; i++)
    Serial.write(ledOn01[i]);
  delay(0);                   // need this 0 delay for the transmission to complete
  digitalWrite(xmtEn, LOW);   // disables transmit buffer
  
  delay(5);
  
  // turn on led motor 2
  digitalWrite(xmtEn, HIGH);  // enable transmit buffer
  for(int i = 0; i < 8; i++)
    Serial.write(ledOn02[i]);
  delay(0);                   // need this 0 delay for the transmission to complete
  digitalWrite(xmtEn, LOW);   // disables transmit buffer
  delay(5);                  //5ms follows the pot smoothly, 100ms is jumpy, emperical

  forward();
  delay(3000);
  reverse();
  delay(3000);
  turnRight();
  delay(3000);
  turnLeft();
  delay(3000);

}

void forward()
{
  
   digitalWrite(xmtEn, HIGH); //Enables transmit buffer
   for (int i=0; i<9; i++)
     Serial.write(rev01FullSpeed[i]);    //Sends variable position command to servo motor
   delay(0);                  //Need this 0 delay for the transmission to complete, saw it on the scope
   digitalWrite(xmtEn, LOW);  //Disables transmit buffer
   
   delay(5);
   
   digitalWrite(xmtEn, HIGH); //Enables transmit buffer
   for (int i=0; i<9; i++)
     Serial.write(rev02FullSpeed[i]);    //Sends variable position command to servo motor
   delay(0);                  //Need this 0 delay for the transmission to complete, saw it on the scope
   digitalWrite(xmtEn, LOW);  //Disables transmit buffer
   delay(5);                  //5ms follows the pot smoothly, 100ms is jumpy, emperical
  
  
}

void reverse()
{
  
   digitalWrite(xmtEn, HIGH); //Enables transmit buffer
   for (int i=0; i<9; i++)
     Serial.write(fwd01FullSpeed[i]);    //Sends variable position command to servo motor
   delay(0);                  //Need this 0 delay for the transmission to complete, saw it on the scope
   digitalWrite(xmtEn, LOW);  //Disables transmit buffer
   
   delay(5);
   
   digitalWrite(xmtEn, HIGH); //Enables transmit buffer
   for (int i=0; i<9; i++)
     Serial.write(fwd02FullSpeed[i]);    //Sends variable position command to servo motor
   delay(0);                  //Need this 0 delay for the transmission to complete, saw it on the scope
   digitalWrite(xmtEn, LOW);  //Disables transmit buffer
   delay(5);                  //5ms follows the pot smoothly, 100ms is jumpy, emperical
  
  
}

void turnRight()
{
  
   digitalWrite(xmtEn, HIGH); //Enables transmit buffer
   for (int i=0; i<9; i++)
     Serial.write(rev01FullSpeed[i]);    //Sends variable position command to servo motor
   delay(0);                  //Need this 0 delay for the transmission to complete, saw it on the scope
   digitalWrite(xmtEn, LOW);  //Disables transmit buffer
   
   delay(5);
   
   digitalWrite(xmtEn, HIGH); //Enables transmit buffer
   for (int i=0; i<9; i++)
     Serial.write(fwd02FullSpeed[i]);    //Sends variable position command to servo motor
   delay(0);                  //Need this 0 delay for the transmission to complete, saw it on the scope
   digitalWrite(xmtEn, LOW);  //Disables transmit buffer
   delay(5);                  //5ms follows the pot smoothly, 100ms is jumpy, emperical
  
  
}

void turnLeft()
{
  
   digitalWrite(xmtEn, HIGH); //Enables transmit buffer
   for (int i=0; i<9; i++)
     Serial.write(fwd01FullSpeed[i]);    //Sends variable position command to servo motor
   delay(0);                  //Need this 0 delay for the transmission to complete, saw it on the scope
   digitalWrite(xmtEn, LOW);  //Disables transmit buffer
   
   delay(5);
   
   digitalWrite(xmtEn, HIGH); //Enables transmit buffer
   for (int i=0; i<9; i++)
     Serial.write(rev02FullSpeed[i]);    //Sends variable position command to servo motor
   delay(0);                  //Need this 0 delay for the transmission to complete, saw it on the scope
   digitalWrite(xmtEn, LOW);  //Disables transmit buffer
   delay(5);                  //5ms follows the pot smoothly, 100ms is jumpy, emperical
  
  
}
