//Arduino Nano w/ ATmega 328 on COM6

//int az_enable_pin = 3;
int az_cw_pin = 4;
int az_ccw_pin = 5;
int cw_switch_pin = 6;
int ccw_switch_pin = 7;
int el_up_pin = 8;
int el_down_pin = 9;
//int el_enable_pin = 10;
int up_switch_pin = 11;
int down_switch_pin = 12;
int led_pin = 13;
int switchstate = 0;
int az_rotate_speed = 28;
int el_rotate_speed = 65;
int total = 0;
int flag = 0;

void setup()
{
//pinMode(az_enable_pin,OUTPUT);
pinMode(az_cw_pin,OUTPUT);
pinMode(az_ccw_pin,OUTPUT);
pinMode(cw_switch_pin,INPUT);
pinMode(ccw_switch_pin,INPUT);
pinMode(el_up_pin,OUTPUT);
pinMode(el_down_pin,OUTPUT);
//pinMode(el_enable_pin,OUTPUT);
pinMode(up_switch_pin,INPUT);
pinMode(down_switch_pin,INPUT);

//digitalWrite(az_enable_pin,LOW);
}
//-----------------------------------------------------
void loop()
{
read_switches();
  if(total == 1 || total == 5 || total == 9)  {
    digitalWrite(el_down_pin,HIGH);
    digitalWrite(el_up_pin,LOW);
    //analogWrite(el_enable_pin,el_rotate_speed);
  }
    if(total == 1)  {
      //digitalWrite(az_enable_pin,LOW);
    }
    if(total == 2 || total == 6 || total == 10)  {
    digitalWrite(el_up_pin,HIGH);
    digitalWrite(el_down_pin,LOW);
    //analogWrite(el_enable_pin,el_rotate_speed);
  }
    if(total == 2)  {
      //digitalWrite(az_enable_pin,LOW);
    }
    if(total == 4 || total == 5 || total == 6)  {
    digitalWrite(az_cw_pin,HIGH);
    digitalWrite(az_ccw_pin,LOW);
      if(flag == 0)  {
        //digitalWrite(az_enable_pin,HIGH);
        delay(1);
        //analogWrite(az_enable_pin,az_rotate_speed);
        flag = 1;
      }
        if(total == 4)  {
          //digitalWrite(el_enable_pin,LOW);
        }
  }
    if(total == 8 || total == 9 || total == 10)  {
    digitalWrite(az_ccw_pin,HIGH);
    digitalWrite(az_cw_pin,LOW);
      if(flag == 0)  {
        //digitalWrite(az_enable_pin,HIGH);
        delay(1);
        //analogWrite(az_enable_pin,az_rotate_speed);
        flag = 1;
      }
        if(total == 8)  {
          //digitalWrite(el_enable_pin,LOW);
        }
  }
    if(total == 0)  {
    digitalWrite(az_cw_pin,LOW);
    digitalWrite(az_ccw_pin,LOW);
    digitalWrite(el_down_pin,LOW);
    digitalWrite(el_up_pin,LOW);
    //digitalWrite(az_enable_pin,LOW);
    //digitalWrite(el_enable_pin,LOW);
    flag = 0;
  }
total = 0;
}


//------------------------------------------------------
void read_switches()
{
switchstate = digitalRead(down_switch_pin);
  if(switchstate == HIGH)  {
    total = 1;
  }
switchstate = digitalRead(up_switch_pin);
  if(switchstate == HIGH)  {
    total = total +2;
  }
switchstate = digitalRead(cw_switch_pin);
  if(switchstate == HIGH)  {
    total = total + 4;
  }
switchstate = digitalRead(ccw_switch_pin);
  if(switchstate == HIGH)  {
    total = total + 8;
  }
  if(total == 12 || total == 3)  {
    total = 0;
  }
return;
}
//------------------------------------------------------

