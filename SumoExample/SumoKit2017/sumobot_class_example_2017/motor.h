#ifndef Motor_h
#define Motor_h

#include <AFMotor.h>

//Full Motor Speed
#define MAXSPEED 255
//Half Motor Speed
#define HALFSPEED 128
//Quarter Motor Speed
#define QUARTERSPEED 64

AF_DCMotor motor_left(1);
AF_DCMotor motor_right(3);

void motor_init()
{
  // turn on motor
  motor_left.setSpeed(MAXSPEED);
  motor_right.setSpeed(MAXSPEED);
  motor_left.run(RELEASE);
  motor_right.run(RELEASE);
}

void idle_motor()
{
  motor_left.run(RELEASE);
  motor_right.run(RELEASE);
}

void forward(int left_speed, int right_speed)
{
  motor_left.setSpeed(left_speed);
  motor_right.setSpeed(right_speed);
  motor_left.run(FORWARD);
  motor_right.run(FORWARD);
}

void backward(int left_speed, int right_speed)
{
  motor_left.setSpeed(left_speed);
  motor_right.setSpeed(right_speed);
  motor_left.run(BACKWARD);
  motor_right.run(BACKWARD);
}

void rotateleft(int rotate_speed)
{
  motor_left.setSpeed(rotate_speed);
  motor_right.setSpeed(rotate_speed);
  motor_left.run(BACKWARD);
  motor_right.run(FORWARD);
}

void rotateright(int rotate_speed)
{
  motor_left.setSpeed(rotate_speed);
  motor_right.setSpeed(rotate_speed);
  motor_left.run(FORWARD);
  motor_right.run(BACKWARD);
}

#endif // Motor_h
