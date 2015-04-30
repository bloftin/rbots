/*

Blink the LED on a AX-12 motor when sent a 
message from an android capable device
current code was tested with Google's DemoKit
example with the LED hooked to the relay

RTEAM Robotics Club
www.phys-x.org/rbots
Ben Loftin

GPL 3 or greater
*/

#include <Wire.h>
#include <Servo.h>

#include <Max3421e.h>
#include <Usb.h>
#include <AndroidAccessory.h>

byte motorID = 0x02;

int xmtEn = 31;

// AX-12 led on packet
byte ledOn01[8] = {0xff, 0xff, motorID, 0x04, 0x03, 0x19, 0x01, 0xE1};
// AX-12 led off packet
byte ledOff01[8] = {0xff, 0xff, motorID, 0x04, 0x03, 0x19, 0x00, 0xE2};

AndroidAccessory acc("RTEAM Robotics Club",
		     "AX-12 Blink",
		     "AX-12 LED Blink Arduino Board",
		     "1.0",
		     "http://www.phys-x.org/rbots/",
		     "000000001");

void setup();
void loop();

byte b1, b2, b3, b4, c;
void setup()
{
  Serial.begin(115200);
  Serial.print("\r\nStart");

  pinMode(xmtEn, OUTPUT); // output enables half duplex transmit buffer
  Serial3.begin(1000000); // sets 1 Mbps baudrate for uart
  
  //try calculating checksum
  ledOn01[7] = ~(ledOn01[2]+ledOn01[3]+ledOn01[4]+ledOn01[5]+ledOn01[6]);
  ledOff01[7] = ~(ledOff01[2]+ledOff01[3]+ledOff01[4]+ledOff01[5]+ledOff01[6]);

  acc.powerOn();
}

void loop()
{
  byte err;
  byte idle;
  static byte count = 0;
  byte msg[3];

  if (acc.isConnected()) {
    int len = acc.read(msg, sizeof(msg), 1);
    if (len > 0) {
      // assumes only one command per packet
      if (msg[0] == 0x3) {
        if (msg[1] == 0x0) {
            digitalWrite(xmtEn, HIGH); // enable transmit buffer
            if( msg[2] == HIGH)
            {
              for(int i = 0; i < 8; i++)
                Serial3.write(ledOn01[i]);
            }
            else
            {
              for(int i = 0; i < 8; i++)
                Serial3.write(ledOff01[i]);
            } 
            delayMicroseconds(25); // need this 0 delay for the transmission to complete
            digitalWrite(xmtEn, LOW); // disables transmit buffer
        }
      }
    }
				

  } 
  else {
      // reset outputs to default values on disconnect
      digitalWrite(xmtEn, HIGH); // enable transmit buffer
      for(int i = 0; i < 8; i++)
        Serial3.write(ledOff01[i]); 
      delayMicroseconds(25); // need this 0 delay for the transmission to complete
      digitalWrite(xmtEn, LOW); // disables transmit buffer
		
  }

  delay(10);
}
