/*
   DynamixelS.cpp - Ax-12+ Half Duplex USART Comunication
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

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "wiring.h"
#include "wiring_private.h"

#include "Dynamixel.h"

#define RX_BUFFER_SIZE 128

struct Serial_buffer {
	volatile unsigned char buffer[RX_BUFFER_SIZE];
	volatile unsigned char head;
	volatile unsigned char tail;
};

Serial_buffer _rx_buffer = { { 0 },0,0 };

ISR(USART1_RX_vect)            
{
//#if defined(__AVR_ATmega8__)
//	_rx_buffer.buffer[(_rx_buffer.head++)] = UDR;	
//#else
	_rx_buffer.buffer[(_rx_buffer.head++)] = UDR1;
//#endif
}

// Constructors ////////////////////////////////////////////////////////////////

Ax12Class::Ax12Class(volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
					 volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
					 volatile uint8_t *udr, uint8_t rxen, uint8_t txen, 
					 uint8_t rxcie, uint8_t udre, uint8_t u2x)
{
	_ubrrh = ubrrh;
	_ubrrl = ubrrl;
	_ucsra = ucsra;
	_ucsrb = ucsrb;
	_udr = udr;
	_rxen = rxen;
	_txen = txen;
	_rxcie = rxcie;
	_udre = udre;
	_u2x = u2x;
}

// Private Methods //////////////////////////////////////////////////////////////

int Ax12Class::read_error(void)
{
	Time_Counter = 0;
	while(usart_available() < 5 & Time_Counter < TIME_OUT){  // Wait for Data
		Time_Counter++;
		delay(1);
		if( usart_peek() != 255 ){
		usart_read();
		}
	}
	
	while (usart_available() > 0){
		Incoming_Byte = usart_read();
		if ( Incoming_Byte == 255 & usart_peek() == 255 ){
			usart_read();                                    // Start Bytes
			usart_read();                                    // Ax-12 ID
			usart_read();                                    // Length
			Error_Byte = usart_read();                       // Error
				return (Error_Byte);
		}
	}
	return (-1);											 // No Ax Response
}

int Ax12Class::usart_available(void)
{
	return (_rx_buffer.head - _rx_buffer.tail);
}

int Ax12Class::usart_peek(void)
{
	if ((_rx_buffer.head - _rx_buffer.tail) == 0) {
		return -1;
	} else {
		return _rx_buffer.buffer[_rx_buffer.tail];
	}
}

int Ax12Class::usart_read(void)
{
	if ((_rx_buffer.head - _rx_buffer.tail) == 0) {
		return -1;
	} else {
		unsigned char c = _rx_buffer.buffer[_rx_buffer.tail];
		_rx_buffer.tail++;
		if (_rx_buffer.tail == _rx_buffer.head)
			_rx_buffer.head = _rx_buffer.tail =  0;
		return c;
	}
}

void Ax12Class::usart_write(uint8_t c)
{
	while (!((*_ucsra) & (1 << _udre)));
	*_udr = c;
}



// Public Methods //////////////////////////////////////////////////////////////

void Ax12Class::begin(long baud, unsigned char D_Pin)
{
	uint16_t baud_setting;
	
	pinMode(D_Pin,OUTPUT);
	Direction_Pin = D_Pin;
	
	*_ucsra = 0;
	baud_setting = (F_CPU / 8 / baud - 1) / 2;
	*_ubrrh = baud_setting >> 8;
	*_ubrrl = baud_setting;
	
	sbi(*_ucsrb, _rxen);
	sbi(*_ucsrb, _txen);
	sbi(*_ucsrb, _rxcie);
	//sbi(*_ucsra, _u2x);

}	

void Ax12Class::end()
{
	cbi(*_ucsrb, _rxen);
	cbi(*_ucsrb, _txen);
	cbi(*_ucsrb, _rxcie);  
	//cbi(*_ucsra, _u2x);
}	

int Ax12Class::ping(unsigned char ID)
{
    TChecksum = (ID + AX_READ_DATA + AX_PING);  
    while ( TChecksum >= 255){
		TChecksum -= 255;
    }
    Checksum = 255 - TChecksum;          
	
	digitalWrite(Direction_Pin,HIGH);
	usart_write(AX_START);                     
	usart_write(AX_START);
	usart_write(ID);
	usart_write(AX_READ_DATA);
	usart_write(AX_PING);    
	usart_write(Checksum);
	delayMicroseconds(TX_DELAY_TIME);
	digitalWrite(Direction_Pin,LOW);
    
    return (read_error());              
}

int Ax12Class::setID(unsigned char ID, unsigned char New_ID)
{    
    TChecksum = (ID + AX_ID_LENGTH + AX_WRITE_DATA + AX_ID + New_ID);  
    while ( TChecksum >= 255){
		TChecksum -= 255;
    }
    Checksum = 255 - TChecksum;         
	
    digitalWrite(Direction_Pin,HIGH);     // Set Tx Mode
    usart_write(AX_START);                // Send Instructions over Serial
    usart_write(AX_START);
    usart_write(ID);
	usart_write(AX_ID_LENGTH);
    usart_write(AX_WRITE_DATA);
    usart_write(AX_ID);
    usart_write(New_ID);
    usart_write(Checksum);
	delayMicroseconds(TX_DELAY_TIME);
	digitalWrite(Direction_Pin,LOW);      // Set Rx Mode
    
    return (read_error());                // Return the read error
}

int Ax12Class::setBD(unsigned char ID, unsigned char Baud_Rate)
{    
    TChecksum = (ID + AX_BD_LENGTH + AX_WRITE_DATA + AX_BAUD_RATE + Baud_Rate); 
    while ( TChecksum >= 255){
		TChecksum -= 255;
    }
    Checksum = 255 - TChecksum;         
	
    digitalWrite(Direction_Pin,HIGH);      // Set Tx Mode
    usart_write(AX_START);                 // Send Instructions over Serial
    usart_write(AX_START);
    usart_write(ID);
	usart_write(AX_BD_LENGTH);
    usart_write(AX_WRITE_DATA);
    usart_write(AX_BAUD_RATE);
    usart_write(Baud_Rate);
    usart_write(Checksum);
    delayMicroseconds(TX_DELAY_TIME);
    digitalWrite(Direction_Pin,LOW);      // Set Rx Mode
    
    return (read_error());                // Return the read error
}

int Ax12Class::move(unsigned char ID, long Position)
{
    char Position_H,Position_L;
    Position_H = Position >> 8;           // 16 bits - 2 x 8 bits variables
    Position_L = Position;
    
    TChecksum = (ID + AX_GOAL_LENGTH + AX_WRITE_DATA + AX_GOAL_POSITION_L + Position_L + Position_H);
    while ( TChecksum >= 255){            
		TChecksum -= 255;     
    }
    Checksum = 255 - TChecksum;
    
	digitalWrite(Direction_Pin,HIGH);      // Set Tx Mode
    usart_write(AX_START);                 // Send Instructions over Serial
    usart_write(AX_START);
    usart_write(ID);
    usart_write(AX_GOAL_LENGTH);
    usart_write(AX_WRITE_DATA);
    usart_write(AX_GOAL_POSITION_L);
    usart_write(Position_L);
    usart_write(Position_H);
    usart_write(Checksum);
	delayMicroseconds(TX_DELAY_TIME);
    digitalWrite(Direction_Pin,LOW);       // Set Rx Mode
	
    return (read_error());                 // Return the read error
}

int Ax12Class::moveSpeed(unsigned char ID, long Position, long Speed)
{
    char Position_H,Position_L,Speed_H,Speed_L;
    Position_H = Position >> 8;    
    Position_L = Position;                // 16 bits - 2 x 8 bits variables
    Speed_H = Speed >> 8;
    Speed_L = Speed;                      // 16 bits - 2 x 8 bits variables
    
    TChecksum = (ID + AX_GOAL_SP_LENGTH + AX_WRITE_DATA + AX_GOAL_POSITION_L);
    TChecksum += (Position_L + Position_H + Speed_L + Speed_H);
    while ( TChecksum >= 255){            
		TChecksum -= 255;     
    }
    Checksum = 255 - TChecksum;
	
    digitalWrite(Direction_Pin,HIGH);     // Set Tx Mode
    usart_write(AX_START);                // Send Instructions over Serial
    usart_write(AX_START);
    usart_write(ID);
    usart_write(AX_GOAL_SP_LENGTH);
    usart_write(AX_WRITE_DATA);
    usart_write(AX_GOAL_POSITION_L);
    usart_write(Position_L);
    usart_write(Position_H);
    usart_write(Speed_L);
    usart_write(Speed_H);
    usart_write(Checksum);
    delayMicroseconds(TX_DELAY_TIME);
    digitalWrite(Direction_Pin,LOW);     // Set Rx Mode
    
    return (read_error());               // Return the read error
}

int Ax12Class::torqueStatus( unsigned char ID, bool Status)
{
    TChecksum = (ID + AX_TORQUE_LENGTH + AX_WRITE_DATA + AX_TORQUE_ENABLE + Status);
    while ( TChecksum >= 255){
		TChecksum -= 255;     
    }
    Checksum = 255 - TChecksum;
    
    digitalWrite(Direction_Pin,HIGH);   // Set Tx Mode
    usart_write(AX_START);              // Send Instructions over Serial
    usart_write(AX_START);
    usart_write(ID);
    usart_write(AX_TORQUE_LENGTH);
    usart_write(AX_WRITE_DATA);
    usart_write(AX_TORQUE_ENABLE);
    usart_write(Status);
    usart_write(Checksum);
    delayMicroseconds(TX_DELAY_TIME);
    digitalWrite(Direction_Pin,LOW);    // Set Rx Mode
    
    return (read_error());              // Return the read error
}

int Ax12Class::ledStatus(unsigned char ID, bool Status)
{    
    TChecksum = (ID + AX_LED_LENGTH + AX_WRITE_DATA + AX_LED + Status);
    while ( TChecksum >= 255){
		TChecksum -= 255;     
    }
    Checksum = 255 - TChecksum;
    
    digitalWrite(Direction_Pin,HIGH);   // Set Tx Mode
    usart_write(AX_START);              // Send Instructions over Serial
    usart_write(AX_START);
    usart_write(ID);
    usart_write(AX_LED_LENGTH);
    usart_write(AX_WRITE_DATA);
    usart_write(AX_LED);
    usart_write(Status);
    usart_write(Checksum);
    delayMicroseconds(TX_DELAY_TIME);
    digitalWrite(Direction_Pin,LOW);    // Set Rx Mode
    
    return (read_error());              // Return the read error
}

int Ax12Class::readTemperature(unsigned char ID)
{	
    TChecksum = (ID + AX_TEM_LENGTH  + AX_READ_DATA + AX_PRESENT_TEMPERATURE + AX_BYTE_READ);
    while ( TChecksum >= 255){
		TChecksum -= 255;     
    }
    Checksum = 255 - TChecksum;
    
    digitalWrite(Direction_Pin,HIGH); 
    usart_write(AX_START);
    usart_write(AX_START);
    usart_write(ID);
    usart_write(AX_TEM_LENGTH);
    usart_write(AX_READ_DATA);
    usart_write(AX_PRESENT_TEMPERATURE);
    usart_write(AX_BYTE_READ);
    usart_write(Checksum);
    delayMicroseconds(TX_DELAY_TIME);
    digitalWrite(Direction_Pin,LOW);     // Set Rx Mode
	
    Temperature_Byte = 0;
    Time_Counter = 0;
    while(usart_available() < 6 & Time_Counter < TIME_OUT){
		Time_Counter++;
		delay(1);
		if( usart_peek() != 255 ){
			usart_read();
		}   
    }
	
    while (usart_available() > 0){
		Incoming_Byte = usart_read();
		if ( Incoming_Byte == 255 & usart_peek() == 255 ){
			usart_read();                            // Start Bytes
			usart_read();                            // Ax-12 ID
			usart_read();                            // Length
			if( (Error_Byte = usart_read()) != 0 )   // Error
				return (Error_Byte*(-1));
			Temperature_Byte = usart_read();         // Temperature
		}
    }
	return (Temperature_Byte);               // Returns the read temperature
}

int Ax12Class::readPosition(unsigned char ID)
{	
    TChecksum = (ID + AX_POS_LENGTH  + AX_READ_DATA + AX_PRESENT_POSITION_L + AX_BYTE_READ_POS);
    while ( TChecksum >= 255){
		TChecksum -= 255;     
    }
    Checksum = 255 - TChecksum;
    
    digitalWrite(Direction_Pin,HIGH); 
    usart_write(AX_START);
    usart_write(AX_START);
    usart_write(ID);
    usart_write(AX_POS_LENGTH);
    usart_write(AX_READ_DATA);
    usart_write(AX_PRESENT_POSITION_L);
    usart_write(AX_BYTE_READ_POS);
    usart_write(Checksum);
    delayMicroseconds(TX_DELAY_TIME);
    digitalWrite(Direction_Pin,LOW);     // Set Rx Mode
	
    Position_Long_Byte = 0;
	Time_Counter = 0;
    while(usart_available() < 7 & Time_Counter < TIME_OUT){
		Time_Counter++;
		delay(1);
		if( usart_peek() != 255 ){
			usart_read();
		}   
    }
	
    while (usart_available() > 0){
		Incoming_Byte = usart_read();
		if ( Incoming_Byte == 255 & usart_peek() == 255 ){
			usart_read();                            // Start Bytes
			usart_read();                            // Ax-12 ID
			usart_read();                            // Length
			if( (Error_Byte = usart_read()) != 0 )   // Error
				return (Error_Byte*(-1));
    
			Position_Low_Byte = usart_read();            // Position Bytes
			Position_High_Byte = usart_read();
			Position_Long_Byte = Position_High_Byte << 8; 
			Position_Long_Byte = Position_Long_Byte + Position_Low_Byte;
		}
    }
	return (Position_Long_Byte);     // Returns the read position
}

int Ax12Class::readVoltage(unsigned char ID)
{    
    TChecksum = (ID + AX_VOLT_LENGTH  + AX_READ_DATA + AX_PRESENT_VOLTAGE + AX_BYTE_READ);
    while ( TChecksum >= 255){
		TChecksum -= 255;     
    }
    Checksum = 255 - TChecksum;
    
    digitalWrite(Direction_Pin,HIGH); 
    usart_write(AX_START);
    usart_write(AX_START);
    usart_write(ID);
    usart_write(AX_VOLT_LENGTH);
    usart_write(AX_READ_DATA);
    usart_write(AX_PRESENT_VOLTAGE);
    usart_write(AX_BYTE_READ);
    usart_write(Checksum);
	delayMicroseconds(TX_DELAY_TIME);
    digitalWrite(Direction_Pin,LOW);     // Set Rx Mode 
	
    Voltage_Byte = 0;
	Time_Counter = 0;
    while(usart_available() < 6 & Time_Counter < TIME_OUT){
		Time_Counter++;
		delay(1);
		if( usart_peek() != 255 ){
			usart_read();
		}   
    }
	
    while (usart_available() > 0){
		Incoming_Byte = usart_read();
		if ( Incoming_Byte == 255 & usart_peek() == 255 ){
			usart_read();                            // Start Bytes
			usart_read();                            // Ax-12 ID
			usart_read();                            // Length
			if( (Error_Byte = usart_read()) != 0 )   // Error
				return (Error_Byte*(-1));
			Voltage_Byte = usart_read();             // Voltage
		}
    }
	return (Voltage_Byte);               // Returns the read Voltage
}


// Preinstantiate Objects //////////////////////////////////////////////////////

//#if defined(__AVR_ATmega8__)
//Ax12Class Ax12(&UBRRH, &UBRRL, &UCSRA, &UCSRB, &UDR, RXEN, TXEN, RXCIE, UDRE);
//#else
Ax12Class Ax12(&UBRR1H, &UBRR1L, &UCSR1A, &UCSR1B, &UDR1, RXEN1, TXEN1, RXCIE1, UDRE1, U2X1);
//#endif



















































