/*

Change ID from default 1 to 0
   
RTEAM Robotics Club
Ben Loftin

*/

// change the id to zero
byte idChange01[8] = {0xff, 0xff, 0xfe, 0x04, 0x03, 0x03, 0x01, 0xF4};

// led on packet
byte ledOn00[8] = {0xff, 0xff, 0x01, 0x04, 0x03, 0x19, 0x01, 0xE1};

int xmtEn = 2;

int first = 0;

void setup()
{
  pinMode(xmtEn, OUTPUT);  // output enables half duplex transmit buffer 
  Serial.begin(1000000);    // sets 1 Mbps baudrate for uart
  
  //try calculating checksum
  ledOn00[7] = ~(ledOn00[2]+ledOn00[3]+ledOn00[4]+ledOn00[5]+ledOn00[6]);
  idChange01[7] = ~(idChange01[2]+idChange01[3]+idChange01[4]+idChange01[5]+idChange01[6]);
   
  
}

void loop()
{
  
   if (first == 0) {
     //change the id from one to zero
    digitalWrite(xmtEn, HIGH); //Enables transmit buffer
      for (int i=0; i<8; i++)
        Serial.write(idChange01[i]); //Sends motor enable packet
    delay(0);                  //Need this 0 delay for the transmission to complete
    digitalWrite(xmtEn, LOW);  //Disables transmit buffer, saw it on the scope
   }
   
  //turn on led
  digitalWrite(xmtEn, HIGH); //Enables transmit buffer
    for (int i=0; i<8; i++)
      Serial.write(ledOn00[i]); //Sends motor enable packet
  delay(0);                  //Need this 0 delay for the transmission to complete
  digitalWrite(xmtEn, LOW);  //Disables transmit buffer, saw it on the scope
  delay(5);                  //5ms follows the pot smoothly, 100ms is jumpy, emperical
    
    
   delay(1000);

}
