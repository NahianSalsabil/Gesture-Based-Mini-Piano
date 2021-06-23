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
	
	DDRC=0xFF;
	

	while (1)
	{
		if((PINA & 1))
		{
			//playnote1(cur_octave);
			PORTC=0b00000001;
		}
		else
		{
			PORTC=0b00000000;
		}
		
		
	}
				

}

