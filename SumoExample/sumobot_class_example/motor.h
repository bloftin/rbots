#ifndef Motor_h
#define Motor_h

//motor control pins
int PWM_L = 9;   //PWM control for motor outputs 1 and 2 is on digital pin 10
int PWM_R = 10;   //PWM control for motor outputs 3 and 4 is on digital pin 11
int DIR_L = 5;   //direction control for motor outputs 1 and 2 is on digital pin 12
int DIR_R = 6;  //direction control for motor outputs 3 and 4 is on digital pin 13

//Full Motor Speed
#define MAXSPEED 200
//Half Motor Speed
#define HALFSPEED 100
//Quarter Motor Speed
#define QUARTERSPEED 50

void motor_init()
{
  pinMode(PWM_L, OUTPUT);  //Set control pins to be outputs
  pinMode(PWM_R, OUTPUT);
  pinMode(DIR_L, OUTPUT);
  pinMode(DIR_R, OUTPUT);
  analogWrite(PWM_L,0);
  analogWrite(PWM_R,0);
}

void idle_motor()
{
  analogWrite(PWM_L,0);
  analogWrite(PWM_R,0);
}

void forward(int left_speed, int right_speed)
{
  digitalWrite(DIR_L, HIGH);
  digitalWrite(DIR_R, HIGH);
  analogWrite(PWM_L, left_speed);
  analogWrite(PWM_R, right_speed);
  
}

void backward(int left_speed, int right_speed)
{
  digitalWrite(DIR_L, LOW);
  digitalWrite(DIR_R, LOW);
  analogWrite(PWM_L, left_speed);
  analogWrite(PWM_R, right_speed);
}

void rotateleft(int rotate_speed)
{
  digitalWrite(DIR_L, LOW);
  digitalWrite(DIR_R, HIGH);
  analogWrite(PWM_L, rotate_speed);
  analogWrite(PWM_R, rotate_speed);
}

void rotateright(int rotate_speed)
{
  digitalWrite(DIR_L, HIGH);
  digitalWrite(DIR_R, LOW);
  analogWrite(PWM_L, rotate_speed);
  analogWrite(PWM_R, rotate_speed);
}

#endif // Motor_h
