#ifndef Comm99_h
#define Comm99_h

// Dynamixel Tx/Rx
#define DXL_BUS_SERIAL1 1  //Dynamixel on Serial1(USART1)  <-OpenCM9.04

//// Host Tx/Rx
// Host Rx Buffer States
#define sizeRx 13
#define HOSTCOMMSTATE_HEADER1     0       // Header 1
#define HOSTCOMMSTATE_HEADER2     1       // Header 2
#define HOSTCOMMSTATE_DATA        2       // Msg Data
#define HOSTCOMMSTATE_CHKSUM      sizeRx  // Checksum
#define HOSTCOMM_HEADER1          255     // ASCII "g"
#define HOSTCOMM_HEADER2          255     // ASCII "u"
  
byte bRxHostBuffer[sizeRx];
byte bRxHost[sizeRx];
byte bRxHostIdx = 0;               // Comm Buffer Index

unsigned int  iChcksumHost;
byte bChcksumHost;

// Function Protoypes
void DynamixelPosTx();
void HostComm();
void ParseHostComm();
float JoystickReshape(int x);

/*********************************************************************************************
 * Dynamixel Transmit Function
 *********************************************************************************************/
void DynamixelPosTx() { 

  unsigned int uSpeed   = 1023;                // Dynamixel Speed Command
  word SyncPage[42]; 
  
  SyncPage[0]  = L11;
  SyncPage[1]  = uT1[0];
  SyncPage[2]  = uSpeed;
  SyncPage[3]  = L12;
  SyncPage[4]  = uT1[1];
  SyncPage[5]  = uSpeed;
  SyncPage[6]  = L13;
  SyncPage[7]  = uT1[2];
  SyncPage[8]  = uSpeed;
  
  SyncPage[9]  = L21;
  SyncPage[10] = uT2[0];
  SyncPage[11] = uSpeed;
  SyncPage[12] = L22;
  SyncPage[13] = uT2[1];
  SyncPage[14] = uSpeed;
  SyncPage[15] = L23;
  SyncPage[16] = uT2[2];
  SyncPage[17] = uSpeed;
  
  SyncPage[18] = L31;
  SyncPage[19] = uT3[0];
  SyncPage[20] = uSpeed;
  SyncPage[21] = L32;
  SyncPage[22] = uT3[1];
  SyncPage[23] = uSpeed;
  SyncPage[24] = L33;
  SyncPage[25] = uT3[2];
  SyncPage[26] = uSpeed;
  
  SyncPage[27] = L41;
  SyncPage[28] = uT4[0];
  SyncPage[29] = uSpeed;
  SyncPage[30] = L42;
  SyncPage[31] = uT4[1];
  SyncPage[32] = uSpeed;
  SyncPage[33] = L43;
  SyncPage[34] = uT4[2];
  SyncPage[35] = uSpeed;
  
 //SyncPage[36] = TURRET_AZ;
 //SyncPage[37] = Turret[0];
 //SyncPage[38] = uSpeed;
 //SyncPage[39] = TURRET_EL;
 //SyncPage[40] = Turret[1];
 //SyncPage[41] = uSpeed;
  
  Dxl.syncWrite(30,2,SyncPage,42);
}

// *********************************************************************************************
// * Host Tx/Rx Function - From DFrobot Wireless Controller
// *********************************************************************************************
void HostComm() { 
  int i;
  byte commHostMsg;
  int commHostChksum;
  
  if (Serial2.available() > 0) {
    commHostMsg = Serial2.read();
    
    if(bRxHostIdx == HOSTCOMMSTATE_HEADER1)
    { 
      if(commHostMsg == HOSTCOMM_HEADER1)
      {
        bRxHostBuffer[bRxHostIdx] = commHostMsg;
        bRxHostIdx = HOSTCOMMSTATE_HEADER2;
      }
    }
    else if(bRxHostIdx == HOSTCOMMSTATE_HEADER2)
    {
      if(commHostMsg == HOSTCOMM_HEADER2)
      {
        bRxHostBuffer[bRxHostIdx] = commHostMsg;
        bRxHostIdx = HOSTCOMMSTATE_DATA;
      }
      else
      {
         bRxHostIdx = HOSTCOMMSTATE_HEADER1;
      }
    }
    else if( (bRxHostIdx >= HOSTCOMMSTATE_DATA) && (bRxHostIdx < HOSTCOMMSTATE_CHKSUM-1))
    {
      bRxHostBuffer[bRxHostIdx] = commHostMsg;
      bRxHostIdx = bRxHostIdx + 1;
    }
    else if(bRxHostIdx == HOSTCOMMSTATE_CHKSUM-1)
    {
      bRxHostBuffer[bRxHostIdx] = commHostMsg;
      bRxHostIdx = HOSTCOMMSTATE_HEADER1;
      
      commHostChksum = 0;
      for (i = 2; i <= sizeRx - 2; i++) {
        commHostChksum = commHostChksum + (unsigned int)bRxHostBuffer[i];
      }
      commHostChksum = 255 - (commHostChksum & 0xFF);    
      if(commHostChksum == bRxHostBuffer[sizeRx-1])
      {
        for(i = 0; i < sizeRx; i++){
          bRxHost[i] =  bRxHostBuffer[i]; 
        }
        ParseHostComm();
      }
    }
    else{bRxHostIdx = HOSTCOMMSTATE_HEADER1;}
  } 
  else {
  }
}
// *********************************************************************************************
// * Host Tx/Rx Function - From DFrobot Wireless Controller
// *********************************************************************************************
void ParseHostComm() {
  
  unsigned Buttons1, Buttons2, LeftJoystickLRLOW, LeftJoystickLRHIGH, 
           LeftJoystickUDLOW, LeftJoystickUDHIGH, RightJoystickLRLOW,
           RightJoystickLRHIGH, RightJoystickUDLOW, RightJoystickUDHIGH, i;  
  
  byte buttonJ2, buttonJ1, buttonSelect, buttonStart, buttonUp, buttonLeft,
       buttonDown, buttonRight, button1, button4, button2, button3, buttonRZ1, 
       buttonRZ2, buttonLZ1, buttonLZ2;

  Buttons1 = 0;
  Buttons2 = 0;
  LeftJoystickLRLOW = 0;
  LeftJoystickLRHIGH = 0;
  LeftJoystickUDLOW = 0;
  LeftJoystickUDHIGH = 0;
  RightJoystickLRLOW = 0;
  RightJoystickLRHIGH = 0;
  RightJoystickUDLOW = 0;
  RightJoystickUDHIGH = 0;
  i = 0;  
  
      // Parse Buttons
      Buttons1          = unsigned(bRxHost[2]);
      Buttons2          = unsigned(bRxHost[3]); 
      buttonJ2     = bitRead(Buttons1, 0); // J2      Right Joystick Z-Axis (Pressed = 0, Unpressed = 1023)
      buttonJ1     = bitRead(Buttons1, 1); // J1      Left Joystick Z-Axis (Pressed = 0, Unpressed = 1023)
      buttonSelect = bitRead(Buttons1, 2); // S1      Select Button
      buttonStart  = bitRead(Buttons1, 3); // S2      Start Button
      buttonUp     = bitRead(Buttons1, 4); // UP      Left Button Up
      buttonLeft   = bitRead(Buttons1, 5); // LEFT    Left Button Left
      buttonDown   = bitRead(Buttons1, 6); // DOWN    Left Button Down
      buttonRight  = bitRead(Buttons1, 7); // RIGHT   Left Button Right

      button1      = bitRead(Buttons2, 0); // 1       Right Button 1
      button4      = bitRead(Buttons2, 1); // 4       Right Button 4
      button2      = bitRead(Buttons2, 2); // 2       Right Button 2
      button3      = bitRead(Buttons2, 3); // 3       Right Button 3
      buttonRZ1    = bitRead(Buttons2, 4); // RZ1     Right Z1 Button (Upper)
      buttonRZ2    = bitRead(Buttons2, 5); // RZ2     Right Z2 Button (Lower)
      buttonLZ1    = bitRead(Buttons2, 6); // LZ1     Left Z1 Button  (Upper)
      buttonLZ2    = bitRead(Buttons2, 7); // LZ2     Left Z2 Button  (Lower)

      // Parse Joysticks
      LeftJoystickLRLOW   = unsigned(bRxHost[4]);  // Left Joystick Left/Right  (Left = 1023, Center = 511, Right = 0)
      LeftJoystickLRHIGH  = unsigned(bRxHost[5]);  // Left Joystick Left/Right  (Left = 1023, Center = 511, Right = 0)
      LeftJoystickUDLOW   = unsigned(bRxHost[6]);  // Left Joystick Up/Down     (Up = 1023,   Center = 511, Down = 0)
      LeftJoystickUDHIGH  = unsigned(bRxHost[7]);  // Left Joystick Up/Down     (Up = 1023,   Center = 511, Down = 0)
      RightJoystickLRLOW  = unsigned(bRxHost[8]);  // Right Joystick Left/Right (Left = 1023, Center = 511, Right = 0)
      RightJoystickLRHIGH = unsigned(bRxHost[9]);  // Right Joystick Left/Right (Left = 1023, Center = 511, Right = 0)
      RightJoystickUDLOW  = unsigned(bRxHost[10]); // Right Joystick Up/Down    (Up = 1023,   Center = 511, Down = 0)
      RightJoystickUDHIGH = unsigned(bRxHost[11]); // Right Joystick Up/Down    (Up = 1023,   Center = 511, Down = 0)

      // Set Gate Interrupt Update Speed (How Fast Gate Steps are Executed)
      // Set Step Size Multiplier
      // Set Step Height Multiplier
      if (button1 == 1) {
        Timer.pause();
        Timer.setPeriod(TimerMS1);
        Timer.refresh();
        Timer.resume();        
        uStepSize  = 2;
      }
      else if (button2 == 1) {
        Timer.pause();
        Timer.setPeriod(TimerMS2);
        Timer.refresh();
        Timer.resume();
        uStepSize  = 2;
      }
      else if (button3 == 1) {
        Timer.pause();
        Timer.setPeriod(TimerMS3);
        Timer.refresh();
        Timer.resume();
        uStepSize  = 2;
      }
      if (button4 == 1) {
        Timer.pause();
        Timer.setPeriod(TimerMS4);
        Timer.refresh();
        Timer.resume();
        uStepSize  = 3;
      }

      // Step Commands in x-direction
      fdelx   = JoystickReshape(int(LeftJoystickUDLOW)  + int(LeftJoystickUDHIGH << 8)) * delxMult * float(uStepSize);

      // Step Commands in y-direction
      // Step Commands for yaw
      if (buttonRZ1 == 1) {
        fdelyaw = 0;
        fdely   = JoystickReshape(int(LeftJoystickLRLOW)  + int(LeftJoystickLRHIGH << 8)) * delyMult * float(uStepSize);;
      }
      else
      {
        fdelyaw = -JoystickReshape(int(LeftJoystickLRLOW)  + int(LeftJoystickLRHIGH << 8)) * delyawMult * float(uStepSize);
        fdely   = 0;
      }

      // Body Pitch Adjustment
      if (buttonJ2 == 1)
      {
        float RightJoystickUD = float(int(RightJoystickUDLOW)  + int(RightJoystickUDHIGH << 8) - 512) / 512;
        if ((RightJoystickUD > .25) && (fPitch < PitchMax))
        {
        fPitch = fPitch + PitchRate;
        }
        if ((RightJoystickUD < -.25) && (fPitch > PitchMin))
        {
          fPitch = fPitch - PitchRate;
        }

        // Body Yaw Adjustment
        float RightJoystickLR = float(int(RightJoystickLRLOW)  + int(RightJoystickLRHIGH << 8) - 512) / 512;
        if ((RightJoystickLR > .25) && (fYaw < YawMax))
        {
          fYaw = fYaw + YawRate;
        }
        if ((RightJoystickLR < -.25) && (fYaw > YawMin))
        {
          fYaw = fYaw - YawRate;
        }
      }
      else
      {
        // Turret Adjustment
       // fdelel = -(JoystickReshape(int(RightJoystickUDLOW)  + int(RightJoystickUDHIGH << 8)) * delelMult);
      //  fdelaz = JoystickReshape(int(RightJoystickLRLOW)  + int(RightJoystickLRHIGH << 8)) * delazMult;
      }
      
      // Body Height Adjustment
     /* if ((buttonUp == 1) && (fHeight < HeightMax)) {
        fHeight =  fHeight + HeightRate;
      }
      if ((buttonDown == 1) && ((fHeight > HeightMin))) {
        fHeight =  fHeight - HeightRate;
      }*/

      // Step Height Adjustment
      if ((buttonLZ1 == 1) && (StepHeight < StepHeightMax)) {
        StepHeight =  StepHeight + StepHeightRate;
      }
      if ((buttonLZ2 == 1) && ((StepHeight > StepHeightMin))) {
        StepHeight =  StepHeight - StepHeightRate;
      }

      // Reset to Home Conditions
      if (buttonSelect == 1)
      {
        fHeight    = 0;
        fPitch     = 0;
        fYaw       = 0;
        StepHeight = StepHeightDefault;
       // Turret[0] = 2048;
       // Turret[1] = 2048;
      }
      
     // Arm Gun Board
      if (buttonStart == 1)
      {
        digitalWrite(armPin, HIGH);
      }
      else
      {
        digitalWrite(armPin, LOW);
      }
      
      // Fire Gun Board
      if (buttonRZ2 == 1)
      {
        digitalWrite(firePin, HIGH);
      }
      else
      {
        digitalWrite(firePin, LOW);
      } 
      //Turret
       if (buttonDown == 1)
      {
        digitalWrite(downPin, HIGH);
      }
      else
      {
        digitalWrite(downPin, LOW);
      } 
       if (buttonUp == 1)
      {
        digitalWrite(upPin, HIGH);
      }
      else
      {
        digitalWrite(upPin, LOW);
      }
       if (buttonLeft == 1)
      {
        digitalWrite(leftPin, HIGH);
      }
      else
      {
        digitalWrite(leftPin, LOW);
      }
       if (buttonRight == 1)
      {
        digitalWrite(rightPin, HIGH);
      }
      else
      {
        digitalWrite(rightPin, LOW);
      }   
}  
// *********************************************************************************************
// * Reshape Analog Joystick for Deadband at Zero
// *********************************************************************************************
float JoystickReshape(int x) {
  float y;
  y = pow((float(x) - 512) / 512, 3);
  if (abs(y) <= .0025) {
    y = 0;
  }
  return y;
}

#endif // Comm99_h

