/*
   DynamixelS.h - Ax-12+ Half Duplex USART Comunication    
   Copyright (c) 2011 Savage Electronics. All rights reserved.
   Created by Savage on 27/01/11.
 
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
 
   This library is distributed in the hope that it will be useful,  
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
 
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 
   Modified HardwareSerial( Nicholas Zambetti & David A. Mellis ) Library 
   to listen at 1MBps USART Transmissions by Josue Gutierrez - Savage Electronics
 
 Contact: quarryman89 at Hotmail dot com 
 
 */



#ifndef Dynamixel_h
#define Dynamixel_h

	// EEPROM AREA  ///////////////////////////////////////////////////////////
#define AX_MODEL_NUMBER_L           0
#define AX_MODEL_NUMBER_H           1
#define AX_VERSION                  2
#define AX_ID                       3
#define AX_BAUD_RATE                4
#define AX_RETURN_DELAY_TIME        5
#define AX_CW_ANGLE_LIMIT_L         6
#define AX_CW_ANGLE_LIMIT_H         7
#define AX_CCW_ANGLE_LIMIT_L        8
#define AX_CCW_ANGLE_LIMIT_H        9
#define AX_SYSTEM_DATA2             10
#define AX_LIMIT_TEMPERATURE        11
#define AX_DOWN_LIMIT_VOLTAGE       12
#define AX_UP_LIMIT_VOLTAGE         13
#define AX_MAX_TORQUE_L             14
#define AX_MAX_TORQUE_H             15
#define AX_RETURN_LEVEL             16
#define AX_ALARM_LED                17
#define AX_ALARM_SHUTDOWN           18
#define AX_OPERATING_MODE           19
#define AX_DOWN_CALIBRATION_L       20
#define AX_DOWN_CALIBRATION_H       21
#define AX_UP_CALIBRATION_L         22
#define AX_UP_CALIBRATION_H         23

	// RAM AREA  //////////////////////////////////////////////////////////////
#define AX_TORQUE_ENABLE            24
#define AX_LED                      25
#define AX_CW_COMPLIANCE_MARGIN     26
#define AX_CCW_COMPLIANCE_MARGIN    27
#define AX_CW_COMPLIANCE_SLOPE      28
#define AX_CCW_COMPLIANCE_SLOPE     29
#define AX_GOAL_POSITION_L          30
#define AX_GOAL_POSITION_H          31
#define AX_GOAL_SPEED_L             32
#define AX_GOAL_SPEED_H             33
#define AX_TORQUE_LIMIT_L           34
#define AX_TORQUE_LIMIT_H           35
#define AX_PRESENT_POSITION_L       36
#define AX_PRESENT_POSITION_H       37
#define AX_PRESENT_SPEED_L          38
#define AX_PRESENT_SPEED_H          39
#define AX_PRESENT_LOAD_L           40
#define AX_PRESENT_LOAD_H           41
#define AX_PRESENT_VOLTAGE          42
#define AX_PRESENT_TEMPERATURE      43
#define AX_REGISTERED_INSTRUCTION   44
#define AX_PAUSE_TIME               45
#define AX_MOVING                   46
#define AX_LOCK                     47
#define AX_PUNCH_L                  48
#define AX_PUNCH_H                  49

    // Status Return Levels ///////////////////////////////////////////////////////////////
#define AX_RETURN_NONE              0
#define AX_RETURN_READ              1
#define AX_RETURN_ALL               2

    // Instruction Set ///////////////////////////////////////////////////////////////
#define AX_PING                     1
#define AX_READ_DATA                2
#define AX_WRITE_DATA               3
#define AX_REG_WRITE                4
#define AX_ACTION                   5
#define AX_RESET                    6
#define AX_SYNC_WRITE               131

	// Specials ///////////////////////////////////////////////////////////////
#define OFF                         0
#define ON                          1
#define AX_BYTE_READ                1
#define AX_BYTE_READ_POS            2
#define AX_ID_LENGTH                4
#define AX_BD_LENGTH                4
#define AX_TEM_LENGTH               4
#define AX_VOLT_LENGTH              4
#define AX_LED_LENGTH               4
#define AX_TORQUE_LENGTH            4
#define AX_POS_LENGTH               4
#define AX_GOAL_LENGTH              5
#define AX_GOAL_SP_LENGTH           7
#define BROADCAST_ID                254
#define AX_START                    255
#define BUFFER_SIZE					64
#define TIME_OUT                    10
#define TX_DELAY_TIME				400

#include <inttypes.h>

struct Serial_buffer;

class Ax12Class {
private:
	
    volatile uint8_t *_ubrrh;
    volatile uint8_t *_ubrrl;
    volatile uint8_t *_ucsra;
    volatile uint8_t *_ucsrb;
    volatile uint8_t *_udr;
    uint8_t _rxen;
    uint8_t _txen;
    uint8_t _rxcie;
    uint8_t _udre;
    uint8_t _u2x;
	
	unsigned char Checksum; 
	unsigned char Time_Counter;
	unsigned char Direction_Pin;
	unsigned char Incoming_Byte;               
	unsigned char Temperature_Byte;
	unsigned char Position_High_Byte;
	unsigned char Position_Low_Byte;
	unsigned char Voltage_Byte;
			
	unsigned int TChecksum;                    
	unsigned int Position_Long_Byte;
		                                       
	int Error_Byte;
	
	void usart_write(uint8_t);
	int usart_available(void);
    int usart_peek(void);
    int usart_read(void);    int read_error(void);
	
	
public:
    Ax12Class(volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
			  volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
			  volatile uint8_t *udr, uint8_t rxen, uint8_t txen,
			  uint8_t rxcie, uint8_t udre, uint8_t u2x);
	
	void begin(long,unsigned char);
	void end(void);
	
	int ping(unsigned char); 
	
	int setID(unsigned char, unsigned char);
	int setBD(unsigned char, unsigned char);
	
	int move(unsigned char, long);
	int moveSpeed(unsigned char, long, long);
	
	int readTemperature(unsigned char);
	int readVoltage(unsigned char);
	int readPosition(unsigned char);
	
	int torqueStatus(unsigned char, bool);
	int ledStatus(unsigned char, bool);
};

extern Ax12Class Ax12;

#endif
