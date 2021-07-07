/*
 * miniPiano.cpp
 *
 * Created: 17-Jun-21 11:21:48 AM
 * Author : Adrita, Nahian, Rittik
 */ 

#ifndef F_CPU
#define F_CPU 1600000UL // 16 MHz clock speed
#endif

#define __DELAY_BACKWARD_COMPATIBLE__
#define SPEAKER_PORT PORTC
#define SPEAKER_DDR DDRC
#define SPEAKER_PIN 7

// My Prototype is simply called PLAYNOTE.
//void PLAYNOTE_with_duration(float duration, float frequency);
void PLAYNOTE(float frequency);
void octave_plus();
void octave_minus();
volatile unsigned char octave = 1;
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
/*void playnote1(int octave)
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
}*/

int main(void)
{
	/* Replace with your application code */
	DDRA=0x00;
	DDRB=0x00;
	//DDRC=0xFF;

	while (1)
	{
		if((PINA & 0b00000001)==0x00)
		{
			//PORTC=0b00000001;
			//PORTD=0b00000000;
			PLAYNOTE(523.2511 * octave);
		}
		else if((PINA & 0b00000010)==0x00)
		{
			//PORTC=0b00000010;
			//PORTD=0b00000000;
			PLAYNOTE(587.3295 * octave);
		}
		else if((PINA & 0b00000100)==0x00)
		{
			//PORTC=0b00000100;
			//PORTD=0b00000000;
			PLAYNOTE(659.2551 * octave);
		}
		else if((PINA & 0b00001000)==0x00)
		{
			//PORTC=0b00001000;
			//PORTD=0b00000000;
			PLAYNOTE(698.4565 * octave);
		}	
		else if((PINA & 0b00010000)==0x00)
		{
			//PORTC=0b00010000;
			//PORTD=0b00000000;
			PLAYNOTE(783.9909 * octave);
		}
		else if((PINA & 0b00100000)==0x00)
		{
			//PORTC=0b00100000;
			//PORTD=0b00000000;
			PLAYNOTE(880.0000 * octave);
		}	
		else if((PINA & 0b01000000)==0x00)
		{
			//PORTC=0b01000000;
			//PORTD=0b00000000;
			PLAYNOTE(987.7666 * octave);
		}	
		else if((PINA & 0b10000000)==0x00)
		{
			//PORTC=0b10000000;
			//PORTD=0b00000000;
			PLAYNOTE(1068 * octave);
		}		
		/*else if((PINB & 0b00000001)==0x00)
		{
			//PORTC=0b00000000;
			//PORTD=0b00000001;
			PLAYNOTE(1397 * octave);
		}
		else if((PINB & 0b00000010)==0x00)
		{
			//PORTC=0b00000000;
			//PORTD=0b00000010;
			PLAYNOTE(1480 * octave);
		}
		else if((PINB & 0b00000100)==0x00)
		{
			//PORTC=0b00000000;
			//PORTD=0b00000100;
			PLAYNOTE(1568 * octave);
		}
		else if((PINB & 0b00001000)==0x00)
		{
			//PORTC=0b00000000;
			//PORTD=0b00001000;
			PLAYNOTE(1630 * octave);
		}	*/		
		else
		{
			PORTC=0x00;
			//PORTD=0x00;
		}
		if(PINB&(1<<5))
		{
			octave_plus();
			_delay_ms(1000);
		}
		else if(PINB&(1<<6))
		{
			octave_minus();
			_delay_ms(1000);
		}
		
		
	}
			
}

void octave_plus(){
	if(octave < 8){
		octave = octave <<2;
	}	
}

void octave_minus(){
	if(octave > 1){
		octave = octave >>2;
	}
}

void PLAYNOTE( float frequency)
{
	// Physics variables
	//long int i;
	float half_period;
	float wavelength;

	wavelength=(1/frequency)*1000;
	// Standard physics formula.
	half_period = wavelength/2;
	// The time between each toggle.

	// Data direction register Pin 7
	// is set for output.
	SPEAKER_DDR |= (1 << SPEAKER_PIN);

	

	_delay_ms(half_period);
	// Wait 1 half wavelength.
	SPEAKER_PORT |= (1 << SPEAKER_PIN);
	// Output 5 V to port Pin 7.
	_delay_ms(half_period);
	// Wait 1 half wavelength.
	SPEAKER_PORT &= ~(1 << SPEAKER_PIN);
	// 0 V at port pin 7.

	return;

}

