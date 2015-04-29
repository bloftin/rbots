#define IR_CLOCK_RATE    36000L

#define pwmPin 11   // IR Carrier 


void setup()  {
  // set the data rate for the Serial port
  Serial.begin(2400);
  
  // toggle on compare, clk/1
  TCCR2A = _BV(WGM21) | _BV(COM2A0);
  TCCR2B = _BV(CS20);
  // 36kHz carrier/timer
  OCR2A = (F_CPU/(IR_CLOCK_RATE*2L)-1);
  pinMode(pwmPin, OUTPUT);
  
}

void loop() 
{
  char i;
  for(i='A'; i<='Z'; i++)
  {
    Serial.print(i);
  }
  delay(40);
}
 
