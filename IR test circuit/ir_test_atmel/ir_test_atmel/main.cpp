/*
 * miniPiano.cpp
 *
 * Created: 17-Jun-21 11:21:48 AM
 * Author : Adrita, Nahian, Rittik
 */ 

#ifndef F_CPU
#define F_CPU 1600000UL // 16 MHz clock speed
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
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
	DDRB=0x00;
	DDRC=0xFF;
	DDRD=0xFF;

	while (1)
	{
		if((PINA & 1))
		{
			//playnote1(cur_octave);
			PORTC=0b00000001;
			PORTD=0b00000000;
		}
		else if(PINA & (1<<1))
		{
			//playnote2(cur_octave);
			PORTC=0b00000010;
			PORTD=0b00000000;
		}
		else if(PINA & (1<<2))
		{
			//playnote3(cur_octave);
			PORTC=0b00000100;
			PORTD=0b00000000;
		}
		else if(PINA & (1<<3))
		{
			//playnote4(cur_octave);
			PORTC=0b00001000;
			PORTD=0b00000000;
		}
		else if(PINA & (1<<4))
		{
			//playnote5(cur_octave);
			PORTC=0b00010000;
			PORTD=0b00000000;
		}
		else if(PINA & (1<<5))
		{
			//playnote6(cur_octave);
			PORTC=0b00100000;
			PORTD=0b00000000;
		}
		else if(PINA & (1<<6))
		{
			//playnote7(cur_octave);
			PORTC=0b01000000;
			PORTD=0b00000000;
		}
		else if(PINA & (1<<7))
		{
			//playnote8(cur_octave);
			PORTC=0b10000000;
			PORTD=0b00000000;
		}
		else if(PINB & 1)
		{
			//playnote9(cur_octave);
			PORTD=0b00000001;
			PORTC=0b00000000;
		}
		else if (PINB & (1<<1))
		{
			//playnote10(cur_octave);
			PORTD=0b00000010;
			PORTC=0b00000000;
		}
		else if (PINB & (1<<2))
		{
			//playnote11(cur_octave);
			PORTD=0b00000100;
			PORTC=0b00000000;
		}
		else if (PINB & (1<<3))
		{
			//playnote12(cur_octave);
			PORTD=0b00001000;
			PORTC=0b00000000;
		}
		else
		{
			PORTC=0b00000000;
			PORTD=0b00000000;
		}
		
		
	}
				

}
