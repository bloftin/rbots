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

// time variable
long cycletime = 0;

enum sumostate {
  IDLESTATE,
  STARTFIVESEC,
  SEARCH,
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
  ir_prox_init();

  // start motors in idle/brake
  idle_motor();
}

void loop() {
    cycletime = millis();
    bool edgeleft = false;
    bool edgeright = false;

    // Check if at edge
    update_edge();
    edgeleft = left_edge();
    edgeright = right_edge();
    if (edgeleft == true && current == SEARCH) current = EDGEAVOIDLEFT;
    if (edgeright == true && current == SEARCH) current = EDGEAVOIDRIGHT;
    
    switch (current) {
        case IDLESTATE:
          //Serial.println("IDLE STATE"); 
          //idle_motor();
          nextstate = STARTFIVESEC;
          break;
        case STARTFIVESEC:
          //Serial.println("START 5 SEC STATE"); 
          delay(3400);
          nextstate = SEARCH;
          break;
        case SEARCH:
          if (ir_detect_left == true && ir_detect_right == true) {
            forward(MAXSPEED, MAXSPEED);
          }
          else if (ir_detect_left == true && ir_detect_right == false) {
            rotateleft(MAXSPEED);
          }
          else if (ir_detect_left == false && ir_detect_right == true) {
            rotateright(MAXSPEED);
          }
          else {
            forward(MAXSPEED, MAXSPEED);
          }
          nextstate = SEARCH;  
          break;
        case EDGEAVOIDLEFT:
          backward(MAXSPEED, MAXSPEED);
          delay(200);
          rotateright(MAXSPEED);
          delay(500);
          nextstate = SEARCH;
          break;
        case EDGEAVOIDRIGHT:
          backward(MAXSPEED, MAXSPEED);
          delay(200);
          rotateleft(MAXSPEED);
          delay(500);
          nextstate = SEARCH;
          break;
        default:
          nextstate = IDLESTATE;
          break;
    }
    
    if (current != IDLESTATE) {
      ir_prox_update();
    }
    
    //Serial.print("Main Loop Time : ");
    //Serial.print(millis() - cycletime);
    //Serial.println("");
    
    current = nextstate;
    
}
