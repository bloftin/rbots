/* Mech99 Control Program*/
// Written by D. Croft
//
// Compatible with OpenCM v1.0.2
// 
// Releases:
// v2.00 Initial Release to R-Team Robotics Community

#include <math.h>
#include <fastmath.h>
#include <float.h>

// Timer Interrupt Period (usec)
#define TimerMS4 16667                         // 60 Hz  16 mS
#define TimerMS3 20000                         // 50 Hz  20 mS
#define TimerMS2 25000                         // 40 Hz  25 mS
#define TimerMS1 40000                         // 25 Hz  40 mS

#define DXL_BUS_SERIAL1 1  //Dynamixel on Serial1(USART1)  <-OpenCM9.04

HardwareTimer Timer(4);

boolean ISRState = false;                      // ISR Interrupt Flag

// Misc Defines.
#define armPin 14
#define firePin 15
#define downPin 13
#define upPin 12
#define leftPin 11
#define rightPin 10
#define baudXbee 57600

Dynamixel Dxl(DXL_BUS_SERIAL1); //Dynamixel on Serial1(USART1)

// Custom Robot99 Header Files
#include "Dimensions99.h"
#include "Kinematics99.h"
#include "Gate99.h" 
//#include "Turret.h"
#include "Comm99.h"

void setup() {
//  // Set up the built-in LED pin as an output:
   //pinMode(BOARD_LED_PIN, OUTPUT); // Used for Debugging
   pinMode(armPin, OUTPUT);
   pinMode(firePin, OUTPUT);
   pinMode(downPin, OUTPUT);
   pinMode(upPin, OUTPUT);
   pinMode(leftPin, OUTPUT);
   pinMode(rightPin, OUTPUT);
   
   // XBEE setup on uart 2
   Serial2.begin(baudXbee);
   //Serial3.begin(115200);   // Used for Debugging
      
   //Initialize dynamixel bus as 1Mbps baud rate
   Dxl.begin(3);
   DynamixelInit();
   
   // Pause the timer while we're configuring it
   Timer.pause();
   Timer.setPeriod(TimerMS2);         // initialize timer1, and set a 100K usecond period
   Timer.setMode(TIMER_CH4, TIMER_OUTPUT_COMPARE);
   Timer.setCompare(TIMER_CH4, 1);  // Interrupt 1 count after each update
   Timer.attachInterrupt(TIMER_CH4, ISRTimer1);
   Timer.refresh();
   Timer.resume();
   
}

void loop() {
  // Service and Comm Commands from DFrobot Wireless Controller
  HostComm();
  
  // Service the ISR
  if (ISRState) {
    //digitalWrite(BOARD_LED_PIN, HIGH);
    // Determine Gate (Step Positions)
    Gate();
    
    //Update Turret Position
   // UpdateTurret();

    // Send Position Commands to Dynamixels
    DynamixelPosTx();
    
    // Resetr ISR Flag
    ISRState = false;
    //digitalWrite(BOARD_LED_PIN, LOW);
  }

}

/*********************************************************************************************
 * Timer Interrupt Service Routine
 *********************************************************************************************/
void ISRTimer1() {
  ISRState = true;
}
