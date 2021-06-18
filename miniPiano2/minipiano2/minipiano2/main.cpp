/*
 * miniPiano.cpp
 *
 * Created: 17-Jun-21 11:21:48 AM
 * Author : Adrita, Nahian, Rittik
 */ 

#ifndef F_CPU
#define F_CPU 1000000UL // 16 MHz clock speed
#endif
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTD2
#define EN eS_PORTD3

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "lcd.h"
volatile int cur_octave=1;
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

int main(void)
{
	/* Replace with your application code */
	DDRA=0x00;
	DDRB=0x0F;
	DDRD=0xFF;
	Lcd4_Init();
	
	int ir_input_A, ir_input_B;
	char str[2];
	
	while (1)
	{
		ir_input_A = PINA;
		ir_input_B = PINB;
		
		PORTD = 0x00;
		str[0] = '0';
		str[1] = '0';	
		if((PINA & 1))
		{
			playnote1(cur_octave);
			PORTD=0b00000001;
		}
		else if(PINA & (1<<1))
		{
			playnote2(cur_octave);
			PORTD=0b00000011;
		}
		else if(PINA & (1<<2))
		{
			playnote3(cur_octave);
			PORTD=0b00000001;
		}
		else if(PINA & (1<<3))
		{
			playnote4(cur_octave);
			PORTD=0b00000011;
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
					PORTD = 0x01;
					break;
				case 2:
					str[0] = '0';
					str[1] = '2';
					PORTD = 0x01;
					break;
				case 4:
					str[0] = '0';
					str[1] = '3';
					PORTD = 0x01;
					break;
				case 8:
					str[0] = '0';
					str[1] = '4';
					PORTD = 0x01;
					break;
				case 16:
					str[0] = '0';
					str[1] = '5';
					PORTD = 0x01;
					break;
				case 32:
					str[0] = '0';
					str[1] = '6';
					PORTD = 0x01;
					break;
				case 64:
					str[0] = '0';
					str[1] = '7';
					PORTD = 0x01;
					break;
				case 128:
					str[0] = '0';
					str[1] = '8';
					PORTD = 0x01;
					break;
					
			}
		}
		
		else if(ir_input_B != 0){
			switch(ir_input_B){
				case 1:
					str[0] = '0';
					str[1] = '9';
					PORTD = 0x01;
					break;
				case 2:
					str[0] = '1';
					str[1] = '0';
					PORTD = 0x01;
					break;
				case 4:
					str[0] = '1';
					str[1] = '1';
					PORTD = 0x01;
					break;
				case 8:
					str[0] = '1';
					str[1] = '2';
					PORTD = 0x01;
					break;
			}
		}
		
		Lcd4_Set_Cursor(1,1);
		Lcd4_Write_String(str);
	}
				

}
