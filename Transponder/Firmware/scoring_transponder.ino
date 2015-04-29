/*
	MWScore Transponder
	
	XBEE setup:
	ATBD = 5 (38400bps)
	ATID = 6200
	MY   = 6200 + TRANSPONDER_ID
	DL   = 6201
	CH   = c
*/ 

#include <PinChangeInt.h>

#define XBEE_BAUDRATE 38400

#define PANELMASK  240
#define PANEL1     112
#define PANEL2     176
#define PANEL3     208
#define PANEL4     224

#define MS_COOLDOWN 1000
#define MS_SIGNAL 50

#define LED_RATE 50
#define LED_PERIOD 300 // LED_RATE * 6

volatile uint8_t hit = 0;

// Arduino Pin Assignments
int hiti = 3;
int hitu = 2;

int tp1 = 4;
int tp2 = 5;
int tp3 = 6;
int tp4 = 7;

// ID Pin Assignments (Analog Pins)
int sw0 = A0;
int sw1 = A1;
int sw2 = A2;
int sw3 = A3;
int sw4 = A4;
int sw5 = A5;
int sw6 = A6;
int sw7 = A7;

void setup() {
  pinMode(hiti, OUTPUT);
  pinMode(hitu, OUTPUT);
  
  pinMode(tp1, INPUT);
  pinMode(tp2, INPUT);
  pinMode(tp3, INPUT);
  pinMode(tp4, INPUT);
  
  PCintPort::attachInterrupt(tp1, hittp1, CHANGE);
  PCintPort::attachInterrupt(tp2, hittp2, CHANGE);
  PCintPort::attachInterrupt(tp3, hittp3, CHANGE);
  PCintPort::attachInterrupt(tp4, hittp4, CHANGE);
  
  pinMode(sw0, INPUT);
  pinMode(sw1, INPUT);
  pinMode(sw2, INPUT);
  pinMode(sw3, INPUT);
  pinMode(sw4, INPUT);
  pinMode(sw5, INPUT);
  pinMode(sw6, INPUT);
  pinMode(sw7, INPUT);
  
  Serial.begin(XBEE_BAUDRATE);
}

void loop() {
  uint32_t delayms = 0;
  uint8_t panel = 0;
  uint8_t id;
  
  if(hit != 0) {		
    // determine panel that was hit
    if      ((hit & PANELMASK) == PANEL1) { delayms = MS_SIGNAL * 1; panel = 1; }
    else if ((hit & PANELMASK) == PANEL2) { delayms = MS_SIGNAL * 2; panel = 2; }
    else if ((hit & PANELMASK) == PANEL3) { delayms = MS_SIGNAL * 3; panel = 3; }
    else if ((hit & PANELMASK) == PANEL4) { delayms = MS_SIGNAL * 4; panel = 4; }
    else                                  { delayms = MS_SIGNAL * 5; panel = 5; }
    
    if(panel != 0) {			
      // hit output high
      digitalWrite(hiti, HIGH);
      
      // tx hit packet
      id = 63 - (0x3f & PINC);
      Serial.write((uint8_t) 0x55);
      Serial.write((uint8_t) id);
      Serial.write((uint8_t) (0xff - id));
      Serial.write((uint8_t) panel);
			
      // delay and reset hit output
      delay(delayms);
      digitalWrite(hiti, LOW);
			
      // blink LED board 3 times
      for (int x = 0; x < 3; x++)
      {
        digitalWrite(hitu, HIGH);
        delay(LED_RATE);
        digitalWrite(hitu, LOW);
        delay(LED_RATE);
      }
			
      // delay for the remaining cooldown period
      delay(MS_COOLDOWN - LED_PERIOD - delayms);
    }
			
    // reset variables
    hit = 0;
    delayms = 0;
    panel = 0;
  }
  
}

void hittp1() {
  hit = PANEL1;
}

void hittp2() {
  hit = PANEL2;
}

void hittp3() {
  hit = PANEL3;
}

void hittp4() {
  hit = PANEL4;
}



