/* Mini Sumo Code */
// Written by D. Fong
//
// Compatible with Arduino IDE 1.6.10
// Compatible with Arduino Trinket Pro 5V
// 
// Releases:
// v1.00 Initial Release to R-Team Robotics Community

#include "sumosense.h"
#include "motor.h"
#include "edgesensor.h"

#include <IRremote.h>

// ir remote pin
int irremote_pin = A0;
IRrecv irrecv(irremote_pin);
decode_results signals;

// time variable
long cycletime = 0;

enum sumostate {
  IDLESTATE,
  STARTFIVESEC,
  SEARCH,
  TRACKTEST,
  SENSORON,
  EDGEAVOIDLEFT,        // state where sumo detects edge of ring and will try to turn around
  EDGEAVOIDRIGHT };    // state where sumo detects edge of ring and will try to turn around
 
typedef enum sumostate sumo;
long searchtime = 0;     // time when sumo enters find state
long hunttime = 0;     // time when sumo enters hunt state
long edgetime = 0;

//sumo states
sumo current;
sumo nextstate;

void setup() {
  Serial.begin(9600);  // Initializes serial port
  // Waits for serial port to connect. Needed for Leonardo only
  while ( !Serial ) ;

  motor_init();
  edge_init();
  ir_prox_init();

  //enabe input from IR remote receiver
  irrecv.enableIRIn();

  // start motors in idle/brake
  idle_motor();
}

void loop() {
    cycletime = millis();
    bool edgeleft = false;
    bool edgeright = false;

    // Check if at edge
    edgeleft = left_edge();
    edgeright = right_edge();
    if (edgeleft == true && current == SEARCH) current = EDGEAVOIDLEFT;
    if (edgeright == true && current == SEARCH) current = EDGEAVOIDRIGHT;
    
    switch (current) {
        case IDLESTATE:
          //Serial.println("IDLE STATE"); 
          idle_motor();
          break;
        case STARTFIVESEC:
          //Serial.println("START 5 SEC STATE"); 
          delay(5000);
          nextstate = SEARCH;
          break;
        case SEARCH:
          if (ir_detect_front == true && ir_detect_left == false && ir_detect_right == false) {
            forward(MAXSPEED, MAXSPEED);
          }
          else if (ir_detect_front == true && ir_detect_left == true && ir_detect_right == true) {
            forward(MAXSPEED, MAXSPEED);
          }
          else if (ir_detect_front == false && ir_detect_left == true && ir_detect_right == false) {
            rotateleft(MAXSPEED);
          }
          else if (ir_detect_front == true && ir_detect_left == true && ir_detect_right == false) {
            forward(HALFSPEED, MAXSPEED);
          }
          else if (ir_detect_front == false && ir_detect_left == false && ir_detect_right == true) {
            rotateright(MAXSPEED);
          }
          else if (ir_detect_front == true && ir_detect_left == false && ir_detect_right == true) {
            forward(MAXSPEED, HALFSPEED);
          }
          else {
            forward(HALFSPEED, HALFSPEED);
          }
          nextstate = SEARCH;  
          break;
        case TRACKTEST:
          nextstate = TRACKTEST;
          if (ir_detect_front == true && ir_detect_left == false && ir_detect_right == false) {
            idle_motor();
          }
          else if (ir_detect_front == true && ir_detect_left == true && ir_detect_right == true) {
            idle_motor();
          }
          else if (ir_detect_front == false && ir_detect_left == true && ir_detect_right == false) {
            rotateleft(HALFSPEED);
          }
          else if (ir_detect_front == true && ir_detect_left == true && ir_detect_right == false) {
            rotateleft(HALFSPEED);
          }
          else if (ir_detect_front == false && ir_detect_left == false && ir_detect_right == true) {
            rotateright(HALFSPEED);
          }
          else if (ir_detect_front == true && ir_detect_left == false && ir_detect_right == true) {
            rotateright(HALFSPEED);
          }
          else {
            idle_motor();
          }
          break;
        case SENSORON:
          Serial.println(detect_ir, BIN);
          if (ir_detect_front == true && ir_detect_left == false && ir_detect_right == false) {
            Serial.println("DETECT FRONT");
          }
          else if (ir_detect_front == true && ir_detect_left == true && ir_detect_right == true) {
            Serial.println("DETECT FRONT");
          }
          else if (ir_detect_front == false && ir_detect_left == true && ir_detect_right == false) {
            Serial.println("DETECT LEFT");
          }
          else if (ir_detect_front == true && ir_detect_left == true && ir_detect_right == false) {
            Serial.println("DETECT LEFT");
          }
          else if (ir_detect_front == false && ir_detect_left == false && ir_detect_right == true) {
            Serial.println("DETECT RIGHT");
          }
          else if (ir_detect_front == true && ir_detect_left == false && ir_detect_right == true) {
            Serial.println("DETECT RIGHT");
          }
          else {
            Serial.println("DETECT NONE");
          }
          nextstate = SENSORON;
          break;
        case EDGEAVOIDLEFT:
          backward(MAXSPEED, MAXSPEED);
          delay(200);
          rotateleft(MAXSPEED);
          delay(300);
          nextstate = SEARCH;
          break;
        case EDGEAVOIDRIGHT:
          backward(MAXSPEED, MAXSPEED);
          delay(200);
          rotateright(MAXSPEED);
          delay(300);
          nextstate = SEARCH;
          break;
        default:
          nextstate = IDLESTATE;
          break;
    }
    
    if (irrecv.decode(&signals)) 
    {
      // Serial.println(signals.value, HEX);
      if (signals.value == 0xFFC23D) nextstate = STARTFIVESEC;
      if (signals.value == 0xFF6897) nextstate = IDLESTATE;
      if (signals.value == 0xFF7A85) nextstate = TRACKTEST;
      if (signals.value == 0xFF10EF) nextstate = SENSORON;
      irrecv.resume(); // get the next signal
      }

    
    if (current != IDLESTATE) {
      ir_prox_update();
    }
    
    //Serial.print("Main Loop Time : ");
    //Serial.print(millis() - cycletime);
    //Serial.println("");
    
    current = nextstate;
    
}
