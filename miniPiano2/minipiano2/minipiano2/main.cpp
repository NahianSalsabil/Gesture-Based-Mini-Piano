/*
 * miniPiano.cpp
 *
 * Created: 17-Jun-21 11:21:48 AM
 * Author : Adrita, Nahian, Rittik
 */ 

#ifndef F_CPU
#define F_CPU 1000000UL // 16 MHz clock speed
#endif
//#define F_CPU 8000000UL

#define get_bit(reg,bitnum) ((reg & (1<<bitnum))>>bitnum)
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTD2
#define EN eS_PORTD3

#define __DELAY_BACKWARD_COMPATIBLE__

#define SPEAKER_PORT PORTD
#define SPEAKER_DDR DDRD
#define SPEAKER_PIN 1

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "lcd.h"
volatile int cur_octave=1;
void TWI_Init (void);

void TWI_Start (void);

void TWI_Stop (void);

void TWI_Write (char data);

void TWI_Read_Nack (char* ptr);

void EEPROM_Write (char data, char address);

void EEPROM_Read (char address, char* ptr);




void TWI_Init (void)

{

	//set_bit(TWCR,6);

	TWSR=0;

	TWBR=0x07;

	TWCR|=(1<<TWEN);

}




void TWI_Start (void)

{

	TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);

	while (get_bit(TWCR,TWINT)==0)

	{

	}

}




void TWI_Stop (void)

{

	TWCR=(1<<TWSTO)|(1<<TWEN)|(1<<TWINT);

}




void TWI_Write (char data)

{

	TWDR=data;

	TWCR= (1<<TWINT)|(1<<TWEN);

	while (get_bit(TWCR,TWINT)==0)

	{

	}

}




void TWI_Read_Nack (char* ptr) // The function argument is a pointer to a memory place in the MCU to store the received data in

{

	TWCR=(1<<TWINT)|(1<<TWEN);

	while (get_bit(TWCR,TWINT)==0)

	{

	}

	*ptr=TWDR;

}




void EEPROM_Write (char data, char address)

{

	TWI_Start();

	TWI_Write(0xA8); //slave address is 1010.100 and a 0 in the 8th bit to indicate Writting.

	TWI_Write(address);

	TWI_Write(data);

	TWI_Stop();

}

void EEPROM_Read (char address, char* ptr) // the function arguments are an address in the EEPROM to read from and a pointer to a memory place in the MCU to store the read data in

{

	TWI_Start();

	TWI_Write(0xA8);

	TWI_Write(address);

	TWI_Start();

	TWI_Write(0xA9);

	TWI_Read_Nack(ptr);

	TWI_Stop();




}
void playnote1(int octave)
{
	//float frequency=524;
}
void playnote2(int octave)
{
	//float frequency=524;
}
void playnote3(int octave)
{
	//float frequency=524;
}
void playnote4(int octave)
{
	//float frequency=524;
}
void playnote5(int octave)
{
	//float frequency=524;
}
void playnote6(int octave)
{
	//float frequency=524;
}
void playnote7(int octave)
{
	//float frequency=524;
}
void playnote8(int octave)
{
	//float frequency=524;
}
void playnote9(int octave)
{
	//float frequency=524;
}
void playnote10(int octave)
{
	//float frequency=524;
}
void playnote11(int octave)
{
	//float frequency=524;
}
void playnote12(int octave)
{
	//float frequency=524;
}
void PLAYNOTE(float duration, float frequency);

int main(void)
{
	/* Replace with your application code */
	DDRA=0x00;
	DDRB=0x00;
	DDRD=0xFF;
	Lcd4_Init();
	char R;

	TWI_Init();

	
	int ir_input_A, ir_input_B;
	char str[2];
	
	while (1)
	{
		DDRA = 0x00;
		if(PINA & (1 << 1)){
			PLAYNOTE(400,880);  // Musical note 880 Hz
		}
		
		if(PINA & (1 << 7)){
			PLAYNOTE(400,1660);  // Musical note 1660 Hz
		}
		
		ir_input_A = PINA;
		ir_input_B = PINB;
		
		//PORTD = 0x00;
		str[0] = '0';
		str[1] = '0';	
		if((PINA & 1))
		{
			playnote1(cur_octave);
		}
		else if(PINA & (1<<1))
		{
			playnote2(cur_octave);
		}
		else if(PINA & (1<<2))
		{
			playnote3(cur_octave);
		}
		else if(PINA & (1<<3))
		{
			playnote4(cur_octave);
		}
		else if(PINA & (1<<4))
		{
			playnote5(cur_octave);
		}
		else if(PINA & (1<<5))
		{
			playnote6(cur_octave);
		}
		else if(PINA & (1<<6))
		{
			playnote7(cur_octave);
		}
		else if(PINA & (1<<7))
		{
			playnote8(cur_octave);
		}
		else if(PINB & 1)
		{
			playnote9(cur_octave);
		}
		else if (PINB & (1<<1))
		{
			playnote10(cur_octave);
		}
		else if (PINB & (1<<2))
		{
			playnote11(cur_octave);
		}
		else if (PINB & (1<<3))
		{
			playnote12(cur_octave);
		}
		
		if(ir_input_A != 0){
			switch(ir_input_A){
				case 1:
					str[0] = '0';
					str[1] = '1';
					//PORTD = 0x01;
					break;
				case 2:
					str[0] = '0';
					str[1] = '2';
					//PORTD = 0x01;
					break;
				case 4:
					str[0] = '0';
					str[1] = '3';
					//PORTD = 0x01;
					break;
				case 8:
					str[0] = '0';
					str[1] = '4';
					//PORTD = 0x01;
					break;
				case 16:
					str[0] = '0';
					str[1] = '5';
					//PORTD = 0x01;
					break;
				case 32:
					str[0] = '0';
					str[1] = '6';
					//PORTD = 0x01;
					break;
				case 64:
					str[0] = '0';
					str[1] = '7';
					//PORTD = 0x01;
					break;
				case 128:
					str[0] = '0';
					str[1] = '8';
					//PORTD = 0x01;
					break;
					
			}
		}
		
		else if(ir_input_B != 0){
			switch(ir_input_B){
				case 1:
					str[0] = '0';
					str[1] = '9';
					//PORTD = 0x01;
					break;
				case 2:
					str[0] = '1';
					str[1] = '0';
					//PORTD = 0x01;
					break;
				case 4:
					str[0] = '1';
					str[1] = '1';
					//PORTD = 0x01;
					break;
				case 8:
					str[0] = '1';
					str[1] = '2';
					//PORTD = 0x01;
					break;
			}
		}
		
		Lcd4_Set_Cursor(1,1);
		Lcd4_Write_String(str);
		EEPROM_Write(0xE0,0x00);

		_delay_ms(1000); // You must allow suffcent delay for the EEPROM to complete the its internal write cycle

		EEPROM_Read(0x00,&R);

		if (R==0xE0)

		{

			PORTD=0b00000001;

		}
	}
				

}
 // ---------------------------------------
 // The PLAYNOTE function must be given the
 // duration, and frequency values.
 // The duration is how long the note
 // is played for. The frequency value
 // determines the musical note.
 // ---------------------------------------
void PLAYNOTE(float duration, float frequency)
{
	// Physics variables
	long int i,cycles;
	float half_period;
	float wavelength;

	wavelength=(1/frequency)*1000;
	// Standard physics formula.
	cycles=duration/wavelength;
	// The number of cycles.
	half_period = wavelength/2;
	// The time between each toggle.

	// Data direction register Pin 7
	// is set for output.
	SPEAKER_DDR |= (1 << SPEAKER_PIN);

	for (i=0;i<cycles;i++)
	{
		// The output pin 7 is toggled
		// for the 'cycles'number of times.
		// --------------------------------

		_delay_ms(half_period);
		// Wait 1 half wavelength.
		SPEAKER_PORT |= (1 << SPEAKER_PIN);
		// Output 5 V to port Pin 7.
		_delay_ms(half_period);
		// Wait 1 half wavelength.
		SPEAKER_PORT &= ~(1 << SPEAKER_PIN);
		// 0 V at port pin 7.
	}
	return;
	// Return to main()
}
