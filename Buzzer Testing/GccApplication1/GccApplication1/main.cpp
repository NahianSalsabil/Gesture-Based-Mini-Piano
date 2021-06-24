/*
 * GccApplication1.cpp
 *
 * Created: 6/19/2021 1:05:40 AM
 * Author : Dell
 */ 

#define F_CPU 1000000UL
#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/io.h>
#include <util/delay.h>

#define SPEAKER_PORT PORTC
#define SPEAKER_DDR DDRC
#define SPEAKER_PIN 7
// My Prototype is simply called PLAYNOTE.
void PLAYNOTE(float duration, float frequency);
void PLAYNOTE_ONE(float duration, float frequency);

int main(void)
{
	DDRA = 0x00;
	while(1){
		if(PINA & 1){
			PLAYNOTE(10,524);  // Musical note 880 Hz
		}
		else if(PINA & (1 << 1)){
			PLAYNOTE(10,880);  // Musical note 988 Hz
		}
		else if(PINA & (1 << 2)){
			PLAYNOTE(10,988);  // Musical note 988 Hz
		}
		else if(PINA & (1 << 3)){
			PLAYNOTE(10,1047);  // Musical note 1047 Hz
		}
		else if(PINA & (1 << 4)){
			PLAYNOTE(10,1109);  // Musical note 1660 Hz
		}
		else if(PINA & (1 << 5)){
			PLAYNOTE(10,1175);  // Musical note 1660 Hz
		}
		else if(PINA & (1 << 6)){
			PLAYNOTE(10,1244);  // Musical note 1660 Hz
		}
		
		else if(PINA & (1 << 7)){
			PLAYNOTE(10,1319);  // Musical note 1660 Hz
		}
		else if(PINB & 1){
			PLAYNOTE(10,1397);  // Musical note 1660 Hz
		}
		else if(PINB & (1 << 1)){
			PLAYNOTE(10,1480);  // Musical note 1660 Hz
		}
		else if(PINB & (1 << 2)){
			PLAYNOTE(10,1568);  // Musical note 1660 Hz
		}
		else if(PINB & (1 << 3)){
			PLAYNOTE(10,1048);  // Musical note 1660 Hz
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

