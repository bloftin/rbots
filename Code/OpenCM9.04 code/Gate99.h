#ifndef Gate99_h
#define Gate99_h

//#include "Dimensions99.h"
// Uncomment Following Line for Smoother (but Slower) Gate
//#define FastGate
#define LOW_BYTE(x)     ((unsigned char)((x)&0xFF))
#define HIGH_BYTE(x)    ((unsigned char)(((x)>>8)&0xFF))

// Function Protoypes
void Gate();

// ****************************************************************
// Walking Gate Multipliers
// ****************************************************************
// The normalized walking gate vectors (defined below) are multiplied 
// by the following "base" multipliers [delxMult, delyMult, delyawMult]. 
// Change these to adjust the "base" step size.
// The step sizes are then dynamically adjusted using "uStepSize"
#define delxMult    0.3                  // inch
#define delyMult    0.3                  // inch
#define delyawMult  0.034906585039887    // rad
unsigned int uStepSize = 2;              // Gate Stepsize Multiplier 
                                         // (Speed Select Dependent)

// ****************************************************************                                         
// Body Height, Pitch, and Yaw Adjustments.
// ****************************************************************
// These are rate terms. The base "rate" is integrated each ISR cycle
// to provide a position term.

// Body Height Adjustments
#define HeightRate  0.05                  // in/cycle
#define HeightMax   0.50                  // in 
#define HeightMin  -1.00                  // in
// Height Hop is used to try to counteract the effects of shoulder joint
// compliance during stepping. Can be set to 0 if not wanted.
#define HeightHop   0.125*0                 // in

// Body Pitch
#define PitchRate   0.01                  // rad/cycle
#define PitchMax   (10*0.017453292519943) // rad
#define PitchMin  (-10*0.017453292519943) // rad

// Body Yaw
#define YawRate     0.01                  // rad/cycle
#define YawMax     (5*0.017453292519943) // rad
#define YawMin    (-5*0.017453292519943) // rad

// **************************************************************** 
// Step Height Adjustnments
// ****************************************************************
// These are rate terms. The base "rate" is integrated each ISR cycle
// to provide a position term.  default 0.125
#define StepHeightDefault 0.30
float StepHeight      = StepHeightDefault; // Base Step Height (in)
float StepHeightRate  = 0.01;              // in/cycle
float StepHeightMin   = 0.00;              // in
float StepHeightMax   = 0.50;              // in

// ****************************************************************
// Step Smoothing Filter
// **Needs Further Work in Future Releases** 
//
// fdelftz   = fStepFilt*fdelftzk1 + (1-fStepFilt)*fdelftzu;
// ****************************************************************
// This filter is used to smooth out the transition between a "moving"
// state and stationary. If not used, the transition results in a very
// "jerky" transition. Its possible at high movements rates this filter
// reults in lag which is potentially could causing problems in Inverse 
// Kinematics.
float fStepFilt       = 0.25;                 // Filter Coefficient 
                                              // (0 = No Filtering) 
float fdelftzu        = 0;                    // Filter Input(k)
float fdelftz         = 0;                    // Filter Output(k)
float fdelftzk1       = 0;                    // Filter Output(k-1)

// Initialize Gate Commands
float fdelx     = 0;                          // Gate delta x (inch)
float fdely     = 0;                          // Gate delta y (inch)
float fdelyaw   = 0;                          // Gate delta yaw (rad)
float fHeight   = 0;                          // Body delta height (inch)

// ****************************************************************
// Gate Vector Definitions
// ****************************************************************
// Walking Gate Step Location Vectors - Length 32
// Used for Smoother Gate Steps
unsigned int countmax  = 32;   // Step Multiplier Vector Size
unsigned int count = 0;        // Initialize Gate Vector Count

float delxy_vec[32] = { 0.0000,  0.2543,  0.5086,  0.7629,
                        1.0172,  1.2715,  1.5257,  1.7800,
                        2.0000,  1.7800,  1.5257,  1.2715,
                        1.0172,  0.7629,  0.5086,  0.2543,
                        0.0000, -0.2543, -0.5086, -0.7629,
                       -1.0172, -1.2715, -1.5257, -1.7800,
                       -2.0000, -1.7800, -1.5257, -1.2715,
                       -1.0172, -0.7629, -0.5086, -0.2543};

float delyaw_vec[32] = { 0.0000,  0.2543,  0.5086,  0.7629,
                         1.0172,  1.2715,  1.5257,  1.7800,
                         2.0000,  1.7800,  1.5257,  1.2715,
                         1.0172,  0.7629,  0.5086,  0.2543,
                         0.0000, -0.2543, -0.5086, -0.7629,
                        -1.0172, -1.2715, -1.5257, -1.7800,
                        -2.0000, -1.7800, -1.5257, -1.2715,
                        -1.0172, -0.7629, -0.5086, -0.2543};

float delz13_vec[32] = { 2.0000, 2.0000,  2.0000,  2.0000,
                         1.8731, 1.3749,  0.8750,  0.3751,
                         0.0000, 0.0000,  0.0000,  0.0000,
                         0.0000, 0.0000,  0.0000,  0.0000,
                         0.0000, 0.0000,  0.0000,  0.0000,
                         0.0000, 0.0000,  0.0000,  0.0000,
                         0.0000, 0.4064,  0.9063,  1.4062,
                         1.8998, 2.0000,  2.0000,  2.0000};

float delz24_vec[32] = { 0.0000,  0.0000, 0.0000,  0.0000,
                         0.0000,  0.0000, 0.0000,  0.0000,
                         0.0000,  0.4064, 0.9063,  1.4062,
                         1.8998,  2.0000, 2.0000,  2.0000,
                         2.0000,  2.0000, 2.0000,  2.0000,
                         1.8731,  1.3749, 0.8750,  0.3751,
                         0.0019,  0.0000, 0.0000,  0.0000,
                         0.0000,  0.0000, 0.0000,  0.0000};

float delzadj[32]    = { 1.0000,  0.9808, 0.9239,  0.8315,
                         0.7071,  0.5556, 0.3827,  0.1951,
                         0.0000,  0.1951, 0.3827,  0.5556,
                         0.7071,  0.8315, 0.9239,  0.9808,
                         1.0000,  0.9808, 0.9239,  0.8315,
                         0.7071,  0.5556, 0.3827,  0.1951,
                         0.0000,  0.1951, 0.3827,  0.5556,
                         0.7071,  0.8315, 0.9239,  0.9808};   
/*********************************************************************************************
 * Walking Gate
 *********************************************************************************************/
// Leg Angle Initializations
unsigned int uT1[3]   = {512, 512, 512};
unsigned int uT2[3]   = {512, 512, 512};
unsigned int uT3[3]   = {512, 512, 512};
unsigned int uT4[3]   = {512, 512, 512};

float fT11, fT12, fT13;
float fT21, fT22, fT23;
float fT31, fT32, fT33;
float fT41, fT42, fT43;
  

float fdelyawtemp;

void Gate() {
  float fdelx1, fdelx2, fdelx3, fdelx4;
  float fdely1, fdely2, fdely3, fdely4;
  float fdelz1, fdelz2, fdelz3, fdelz4;
  
  
  if ( (count != 0) && (count != countmax / 2) ) {
    fdelftzu = StepHeight;
  }
  else {
    if (fdelx == 0 && fdely == 0 && fdelyaw == 0) {
      count = count - 1;
      fdelftzu = 0.0;
    }
    else {
      fdelftzu = StepHeight;
    }
  }
  fdelftz   = fStepFilt * fdelftzk1 + (1 - fStepFilt) * fdelftzu;
  fdelftzk1 = fdelftz;

  // Determine Step Locations
  fdelx1 =  delxy_vec[count] * fdelx;
  fdelx2 = -fdelx1;
  fdelx3 =  fdelx1;
  fdelx4 = -fdelx1;

  fdely1 =  delxy_vec[count] * fdely;
  fdely2 = -fdely1;
  fdely3 =  fdely1;
  fdely4 = -fdely1;

  fdelz1 =  delz13_vec[count] * fdelftz;
  fdelz2 =  delz24_vec[count] * fdelftz;
  fdelz3 =  fdelz1;
  fdelz4 =  fdelz2;

  //******************************************************************************************
  // Calculate Inverse Kinematics
  //******************************************************************************************
  // Foot Step Locations, Leg 1 & 3
  float Xft13[4] = {XFT[0] + fdelx1, XFT[1] + fdelx2, XFT[2] + fdelx3, XFT[3] + fdelx4};
  float Yft13[4] = {YFT[0] + fdely1, YFT[1] + fdely2, YFT[2] + fdely3, YFT[3] + fdely4};
  float Zft13[4] = {ZFT[0] + fdelz1, ZFT[1] + fdelz2, ZFT[2] + fdelz3, ZFT[3] + fdelz4};
  SingleRotation(delyaw_vec[count]*fdelyaw, &Xft13[0], &Yft13[0], &Zft13[0]);

  // Foot Step Locations, Leg 2 & 4
  float Xft24[4] = {XFT[0] + fdelx1, XFT[1] + fdelx2, XFT[2] + fdelx3, XFT[3] + fdelx4};
  float Yft24[4] = {YFT[0] + fdely1, YFT[1] + fdely2, YFT[2] + fdely3, YFT[3] + fdely4};
  float Zft24[4] = {ZFT[0] + fdelz1, ZFT[1] + fdelz2, ZFT[2] + fdelz3, ZFT[3] + fdelz4};
  fdelyawtemp = -delyaw_vec[count] * fdelyaw ;
  SingleRotation(fdelyawtemp, &Xft24[0], &Yft24[0], &Zft24[0]);

  // Update Foot Locations
  float Xft[4], Yft[4], Zft[4];
  Xft[0] = Xft13[0]; 
  Xft[1] = Xft24[1]; 
  Xft[2] = Xft13[2]; 
  Xft[3] = Xft24[3];
  
  Yft[0] = Yft13[0];
  Yft[1] = Yft24[1]; 
  Yft[2] = Yft13[2]; 
  Yft[3] = Yft24[3];
  
  Zft[0] = fdelz1;   
  Zft[1] = fdelz2;   
  Zft[2] = fdelz3;   
  Zft[3] = fdelz4;

  // Shoulder Rotations
  float Xsh[4] = {XSH[0], XSH[1], XSH[2], XSH[3]};
  float Ysh[4] = {YSH[0], YSH[1], YSH[2], YSH[3]};
  float Zsh[4] = {ZSH[0], ZSH[1], ZSH[2], ZSH[3]};
  DoubleRotation(fPitch, fYaw, fHeight + delzadj[count]*HeightHop, &Xsh[0], &Ysh[0], &Zsh[0]);

  // Inverse Kinematic Angles
  InverseKinematics(Xsh[0], Ysh[0], Zsh[0], Xft[0], Yft[0], Zft[0], &fT11, &fT12, &fT13);
  uT1[0] = (unsigned int)(512 + ((fT11 * 57.295779513082323 + offsetT11) * 3.41));
  uT1[1] = (unsigned int)(512 - (fT12 * 57.295779513082323 + offsetT2 ) * 3.41);
  uT1[2] = (unsigned int)(512 - (fT13 * 57.295779513082323 + offsetT3 ) * 3.41);
  
  InverseKinematics(Xsh[1], Ysh[1], Zsh[1], Xft[1], Yft[1], Zft[1], &fT21, &fT22, &fT23);
  uT2[0] = (unsigned int)(512 + (fT21 * 57.295779513082323 + offsetT21) * 3.41);
  uT2[1] = (unsigned int)(512 - (fT22 * 57.295779513082323 + offsetT2 ) * 3.41);
  uT2[2] = (unsigned int)(512 - (fT23 * 57.295779513082323 + offsetT3 ) * 3.41);

  InverseKinematics(Xsh[2], Ysh[2], Zsh[2], Xft[2], Yft[2], Zft[2], &fT31, &fT32, &fT33);
  uT3[0] = (unsigned int)(512 + (fT31 * 57.295779513082323 + offsetT31) * 3.41);
  uT3[1] = (unsigned int)(512 - (fT32 * 57.295779513082323 + offsetT2 ) * 3.41);
  uT3[2] = (unsigned int)(512 - (fT33 * 57.295779513082323 + offsetT3 ) * 3.41);

  InverseKinematics(Xsh[3], Ysh[3], Zsh[3], Xft[3], Yft[3], Zft[3], &fT41, &fT42, &fT43);
  uT4[0] = (unsigned)(512 + (fT41 * 57.295779513082323 + offsetT41) * 3.41);
  uT4[1] = (unsigned)(512 - (fT42 * 57.295779513082323 + offsetT2 ) * 3.41);
  uT4[2] = (unsigned)(512 - (fT43 * 57.295779513082323 + offsetT3 ) * 3.41);
  
  count = count + 1;
  if (count > countmax - 1) {
    count = 0;
  }
}  

#endif /*Gate99*/                      
