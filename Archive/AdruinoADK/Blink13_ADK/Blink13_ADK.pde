#include <Wire.h>
#include <Servo.h>

#include <Max3421e.h>
#include <Usb.h>
#include <AndroidAccessory.h>

#define  LED13         13

AndroidAccessory acc("RTEAM Robotics Club",
		     "Hello World Blink",
		     "Hellow World Blink Arduino Board",
		     "1.0",
		     "http://www.phys-x.org/rbots/",
		     "0000000012345678");

void setup();
void loop();

byte b1, b2, b3, b4, c;
void setup()
{
  Serial.begin(115200);
  Serial.print("\r\nStart");

  pinMode(LED13, OUTPUT);
  digitalWrite(LED13, LOW);

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
      if (msg[0] == 0x2) {
        if (msg[1] == 0x0) {
          if (msg[2] == 0x0) {
            digitalWrite(LED13, LOW);
          }
          else if ( msg[2] == 0x1) {
            digitalWrite(LED13, HIGH);
          }
        }
      }
    }
				

  } 
  else {
    // reset outputs to default values on disconnect
    digitalWrite(LED13, LOW);
		
  }

  delay(10);
}
