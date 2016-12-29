#include <TimerOne.h>

#define vibrationMotorPin 2
//#define TimerMS 7813                // 128 Hz
//#define TimerMS 15625               //  64 Hz 
#define TimerMS 31250               //  32 Hz
//#define TimerMS 62500               //  16 Hz
//#define TimerMS 125000              //   8 Hz
//#define TimerMS 250000              //   4 Hz
//#define TimerMS 500000              //   2 Hz
//#define TimerMS 1000000             //   1 Hz
//#define TimerMS 2000000             // 0.5 Hz

boolean buttonState[17];
int joystick[4];
int AnalogButton[2];
boolean ISRState = false;
byte Header1 = 255;
byte Header2 = 255;
byte Buttons1 = 0;
byte Buttons2 = 0;
byte LeftJoystickLRLOW = 511;  
byte LeftJoystickLRHIGH = 511;
byte LeftJoystickUDLOW = 511;
byte LeftJoystickUDHIGH = 511;

byte RightJoystickLRLOW = 511;
byte RightJoystickLRHIGH = 511;
byte RightJoystickUDLOW = 511;
byte RightJoystickUDHIGH = 511;

int inputCommand = 0;
unsigned long timer = 0;

String Buttons[17] = {"J2 Right","J1 Left","NULL","S1 Select","S2 Start","UP","LEFT","DOWN","RIGHT",
                      "1","4","2","3","RZ1","RZ2","LZ1","LZ2"};

void setup()
{
  Serial.begin(38400);  //Init the Serial baudrate
  Serial1.begin(38400); //Init the Serial1 port to enable the xbee wireless communication
  InitIO();             //Initialize the inputs/outputs and the buffers
  
  delay(50);
  Timer1.initialize(TimerMS);          // initialize timer1, and set a 100K usecond period 
  Timer1.attachInterrupt(ISRTimer1);  // attaches callback() as a timer overflow interrupt
  delay(50);
}

void loop(){
    if (ISRState){  
      DataUpdate();  // Read the buttons and the joysticks data
      DataTx();      // Print Data to XBee Serial
      printData();   // Print Data to Serial Port
      ISRState = false;
    }
//  if(Serial.available()){
//    char input = Serial.read();
//    
//    switch(input){
//      case 'v':
//        Serial.println("Vibration");
//        inputCommand = input;
//        digitalWrite(vibrationMotorPin,HIGH);
//        break;
//      
//      case 's':
//        Serial.println("Stop");
//        inputCommand = input;
//        digitalWrite(vibrationMotorPin,LOW);
//        break;
//        
//      default:
//        break;
//    }
//  }
}

/*********************************************************************************************
* Timer1 Interrupt Service Routine
*********************************************************************************************/
void ISRTimer1(){
  ISRState = true;
  //   digitalWrite(ledPin, digitalRead(ledPin) ^ 1);
}
/*********************************************************************************************
* Initialize Pin Modes
*********************************************************************************************/
void InitIO(){ 
  for(int i = 0; i < 17; i++) pinMode(i, INPUT); 
  pinMode(vibrationMotorPin,OUTPUT);
  digitalWrite(vibrationMotorPin,LOW);  // Stop shaking of the gamepad
}
/*********************************************************************************************
* Read Data from Analog and Digital Pins
*********************************************************************************************/
void DataUpdate(){  
  // buttonState[0]  = J2      Right Joystick Z-Axis (Pressed = 0, Unpressed = 1023)
  // buttonState[1]  = J1      Left Joystick Z-Axis (Pressed = 0, Unpressed = 1023)
  // buttonState[2]  = NULL    Center Button between Select/Start? Results in Lockup
  // buttonState[3]  = S1      Select  Button
  // buttonState[4]  = S2      Start Button
  // buttonState[5]  = UP      Left Button Up
  // buttonState[6]  = LEFT    Left Button Left
  // buttonState[7]  = DOWN    Left Button Down
  // buttonState[8]  = RIGHT   Left Button Right 
  // buttonState[9]  = 1       Right Button 1
  // buttonState[10] = 4       Right Button 4
  // buttonState[11] = 2       Right Button 2
  // buttonState[12] = 3       Right Button 3
  // buttonState[13] = RZ1     Right Z1 Button (Upper)
  // buttonState[14] = RZ2     Right Z2 Button (Lower)
  // buttonState[15] = LZ1     Left Z1 Button (Upper)
  // buttonState[16] = LZ2     Right Z2 Button (Lower)
  for(int i = 3; i < 17; i++)  buttonState[i] = !digitalRead(i);
  
  // joystick[0]     =         Right Joystick Up/Down    (Up = 1023,   Center = 511, Down = 0)
  // joystick[1]     =         Right Joystick Left/Right (Left = 1023, Center = 511, Right = 0)
  // joystick[2]     =         Left Joystick Up/Down     (Up = 1023,   Center = 511, Down = 0)
  // joystick[3]     =         Left Joystick Left/Right  (Left = 1023, Center = 511, Right = 0)
  for(int i = 2; i < 6; i++)  joystick[i-2] = analogRead(i);
  
// AnalogButton[0] =         Right Joystick Z-Axis (Pressed = 0, Unpressed = 1023) [Same as J2 Button[0]]
// AnalogButton[1] =         Left Joystick Z-Axis  (Pressed = 0, Unpressed = 1023) [Same as J1 Button[1]]
  AnalogButton[0] = analogRead(0);
  AnalogButton[1] = analogRead(1);
  if(AnalogButton[0] >= 511){buttonState[0] = false;} else{buttonState[0] = true;}
  if(AnalogButton[1] >= 511){buttonState[1] = false;} else{buttonState[1] = true;}
//  buttonState[0] = AnalogButton[0];
//  buttonState[1] = AnalogButton[1];
}
/*********************************************************************************************
* Transmit Data to XBee Serial
*********************************************************************************************/
void DataTx(){ 
  int  iChcksum;
  byte bChcksum;
  
  Buttons1 = buttonState[0]*1  + buttonState[1]*2  + buttonState[3]*4  + buttonState[4]*8 +
             buttonState[5]*16 + buttonState[6]*32 + buttonState[7]*64 + buttonState[8]*128;
  Buttons2 = buttonState[9]*1   + buttonState[10]*2  + buttonState[11]*4  + buttonState[12]*8 +
             buttonState[13]*16 + buttonState[14]*32 + buttonState[15]*64 + buttonState[16]*128;
             
  LeftJoystickLRLOW  = lowByte(joystick[3]);  
  LeftJoystickLRHIGH = highByte(joystick[3]);
  LeftJoystickUDLOW  = lowByte(joystick[2]);
  LeftJoystickUDHIGH = highByte(joystick[2]);
  
  RightJoystickLRLOW  = lowByte(joystick[1]);
  RightJoystickLRHIGH = highByte(joystick[1]);
  RightJoystickUDLOW  = lowByte(joystick[0]);
  RightJoystickUDHIGH = highByte(joystick[0]);
  
  iChcksum = Buttons1 + Buttons2 + 
             LeftJoystickLRLOW + LeftJoystickLRHIGH + LeftJoystickUDLOW + LeftJoystickUDHIGH + 
             RightJoystickLRLOW + RightJoystickLRHIGH + RightJoystickUDLOW + RightJoystickUDHIGH;
  bChcksum = 255-lowByte(iChcksum);
  
  Serial1.write(Header1);
  Serial1.write(Header2);
  Serial1.write(Buttons1);
  Serial1.write(Buttons2);
  Serial1.write(LeftJoystickLRLOW);
  Serial1.write(LeftJoystickLRHIGH);
  Serial1.write(LeftJoystickUDLOW);
  Serial1.write(LeftJoystickUDHIGH);
  Serial1.write(RightJoystickLRLOW);
  Serial1.write(RightJoystickLRHIGH);
  Serial1.write(RightJoystickUDLOW);
  Serial1.write(RightJoystickUDHIGH);
  Serial1.write(bChcksum);
}
/*********************************************************************************************
* Print Data to Serial Port
*********************************************************************************************/
void printData(){
  Serial.print("Button Pressed:");
//  for(int i = 0; i < 2; i++)   if(buttonState[i] < 100) Serial.print(Buttons[i]),Serial.print(",");
  for(int i = 0; i < 17; i++){
    if(buttonState[i] == 1)  Serial.print(Buttons[i]),Serial.print(",");
  }
  Serial.println("");
  for(int i = 0; i < 17; i++){  
      Serial.print(buttonState[i]);
  }
  Serial.println("");
  
  Serial.print("Analog Buttons:");
  for(int i = 0; i < 2; i++)  Serial.print(AnalogButton[i]),Serial.print(",");
  Serial.println("");
  
  Serial.print("Analog Joysticks:");
  for(int i = 0; i < 4; i++)  Serial.print(joystick[i]),Serial.print(",");
  
  Serial.println("");
  Serial.println("");
//  Serial.println(inputCommand);
}


