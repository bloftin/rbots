#ifndef SumoSense_h
#define SumoSense_h

//IR Proximity Sensor Pin Definition
int IR_PROX_L = A1;    //Left Angle Proximity IR Detect
int IR_PROX_F = A2;     //Front Left Proximity IR Detect
int IR_PROX_R = A3;     //Front Right Proximity IR Detect
int IR_PROX_L_EN = 11; //Left Angle Proximity IR Enable
int IR_PROX_F_EN = 12;  //Front Left Proximity IR Enable
int IR_PROX_R_EN = 13;  //Front Right Proximity IR Enable

#define IR_ENABLE_DELAY 1

int detect_ir = 1;

bool ir_detect_front = false;
bool ir_detect_left = false;
bool ir_detect_right = false;

void ir_prox_init()
{
  //Init Pins
  pinMode(IR_PROX_R, INPUT);
  pinMode(IR_PROX_F, INPUT);
  pinMode(IR_PROX_L, INPUT);
  pinMode(IR_PROX_R_EN, OUTPUT);
  pinMode(IR_PROX_F_EN, OUTPUT);
  pinMode(IR_PROX_L_EN, OUTPUT);

  //Set Pins to Default States
  digitalWrite(IR_PROX_L_EN, LOW);
  digitalWrite(IR_PROX_F_EN, LOW);
  digitalWrite(IR_PROX_R_EN, LOW);
}

void ir_prox_update()
{
  detect_ir = 0;

  ir_detect_front = false;
  ir_detect_left = false;
  ir_detect_right = false;

  digitalWrite(IR_PROX_L_EN, HIGH);
  delay(IR_ENABLE_DELAY);
  if(digitalRead(IR_PROX_L) == LOW) {
    detect_ir = detect_ir + 1;
    ir_detect_left = true;
  }
  digitalWrite(IR_PROX_L_EN, LOW);

  digitalWrite(IR_PROX_F_EN, HIGH);
  delay(IR_ENABLE_DELAY);
  if(digitalRead(IR_PROX_F) == LOW){
    detect_ir = detect_ir + 2;
    ir_detect_front = true;
  }
  digitalWrite(IR_PROX_F_EN, LOW);

  digitalWrite(IR_PROX_R_EN, HIGH);
  delay(IR_ENABLE_DELAY);
  if(digitalRead(IR_PROX_R) == LOW) {
    detect_ir = detect_ir + 4;
    ir_detect_right = true;
  }
  digitalWrite(IR_PROX_R_EN, LOW);
 
}


#endif // SumoSense_h
