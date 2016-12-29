#ifndef Dimensions99_h
#define Dimensions99_h

void DynamixelInit();

// Leg Definitions: #define LegLocation Dynamixel#
#define L11 1      // Leg 1, Joint 1 ID 
#define L12 2      // Leg 1, Joint 2 ID
#define L13 3      // Leg 1, Joint 3 ID
#define L21 4      // Leg 2, Joint 1 ID
#define L22 5      // Leg 2, Joint 2 ID
#define L23 6      // Leg 2, Joint 3 ID
#define L31 7      // Leg 3, Joint 1 ID
#define L32 8      // Leg 3, Joint 2 ID
#define L33 9      // Leg 3, Joint 3 ID
#define L41 10     // Leg 4, Joint 1 ID
#define L42 11     // Leg 4, Joint 2 ID
#define L43 12     // Leg 4, Joint 3 ID


// Comment/Uncomment Body Configuration to s
#define ConfigBody1

// Select Configuration
#ifdef ConfigBody1
  // Body Dimensions
  #define L0  1.07044                 // Shoulder Fixed Length        (in)
  #define L1  2.60275                 // Upper Leg (Femur) Length     (in)
  #define L2  4.11417                 // Lower Leg (Tibia) Length     (in)
  
  #define Phi 1.00830                 // Shoulder Fixed Angle         (rad)
  #define RSH 4.250                 // Shoulder Joint Radius        (in)
  #define HSH 3.7795                // Shoulder Joint Height        (in)
  #define LFT 3.1102                // Foot Radius - Joint Radius   (in) 

  // Offsets used in calculation of Dynamixel angle commands  
  #define offsetT11  -45.0000        // Theta1 Offset - Leg1         (deg)             
  #define offsetT21 -135.0000        // Theta1 Offset - Leg2         (deg)
  #define offsetT31  135.0000        // Theta1 Offset - Leg3         (deg)
  #define offsetT41   45.0000        // Theta1 Offset - Leg4         (deg)
  
  #define offsetT2   12.6699         // Theta2 Offest - All Legs     (deg) 
  #define offsetT3   77.3302         // Theta3 Offset - All Legs     (deg)  
#endif /*ConfigBody*/

// Shoulder "Home" x,y,z Locations [Body Center] (in)
float XSH[4] = {.7071 * RSH, -.7071 * RSH, -.7071 * RSH,  .7071 * RSH};
float YSH[4] = {.7071 * RSH,  .7071 * RSH, -.7071 * RSH, -.7071 * RSH};
float ZSH[4] = {HSH,  HSH,  HSH,  HSH};
  
// Foot "Home" x,y,z Locations [Body Center] (in)
float XFT[4] = {.7071 * (RSH + LFT), -.7071 * (RSH + LFT), -.7071 * (RSH + LFT),  .7071 * (RSH + LFT)};
float YFT[4] = {.7071 * (RSH + LFT),  .7071 * (RSH + LFT), -.7071 * (RSH + LFT), -.7071 * (RSH + LFT)};
float ZFT[4] = {0, 0, 0, 0};

#endif /*Dimensions99_h*/

void DynamixelInit(){
   unsigned int Joint1CWlim = 410;
   unsigned int Joint1CCWlim = 614;
   unsigned int Joint2CWlim = 208;
   unsigned int Joint2CCWlim = 614;
   unsigned int Joint3CWlim = 359;
   unsigned int Joint3CCWlim = 614;
   
// Leg Travel Limits
   // Joint 1 CW angle Limit
   Dxl.cwAngleLimit(L11, Joint1CWlim);
   Dxl.cwAngleLimit(L21, Joint1CWlim); 
   Dxl.cwAngleLimit(L31, Joint1CWlim);   
   Dxl.cwAngleLimit(L41, Joint1CWlim);
   // Joint 1 CCW angle Limit
   Dxl.ccwAngleLimit(L11, Joint1CCWlim);  
   Dxl.ccwAngleLimit(L21, Joint1CCWlim);   
   Dxl.ccwAngleLimit(L31, Joint1CCWlim);
   Dxl.ccwAngleLimit(L41, Joint1CCWlim); 
   // Joint 2 CW Limit
   Dxl.cwAngleLimit(L12, Joint2CWlim);
   Dxl.cwAngleLimit(L22, Joint2CWlim);  
   Dxl.cwAngleLimit(L32, Joint2CWlim);
   Dxl.cwAngleLimit(L42, Joint2CWlim);
   // Joint 2 CCW Limit
   Dxl.ccwAngleLimit(L12, Joint2CCWlim);  
   Dxl.ccwAngleLimit(L22, Joint2CCWlim); 
   Dxl.ccwAngleLimit(L32, Joint2CCWlim);
   Dxl.ccwAngleLimit(L42, Joint2CCWlim); 
   // Joint 3 CW Limit  
   Dxl.cwAngleLimit(L13, Joint3CWlim);
   Dxl.cwAngleLimit(L23, Joint3CWlim);
   Dxl.cwAngleLimit(L33, Joint3CWlim);
   Dxl.cwAngleLimit(L43, Joint3CWlim);
   // Joint 3 CCW Limit
   Dxl.ccwAngleLimit(L13, Joint3CCWlim);
   Dxl.ccwAngleLimit(L23, Joint3CCWlim);    
   Dxl.ccwAngleLimit(L33, Joint3CCWlim);    
   Dxl.ccwAngleLimit(L43, Joint3CCWlim);  
   
  unsigned int Speed   = 512;
  word SyncPage[12]; 
  
  SyncPage[0]  = 51;
  SyncPage[1]  = 512;
  SyncPage[2]  = Speed;
  
  SyncPage[3]  = 52;
  SyncPage[4]  = 512;
  SyncPage[5]  = Speed;
  
  SyncPage[6]  = 61;
  SyncPage[7]  = 512;
  SyncPage[8]  = Speed;
  
  SyncPage[9]  = 62;
  SyncPage[10] = 512;
  SyncPage[11] = Speed;
  
  Dxl.syncWrite(30,2,SyncPage,12);
}
