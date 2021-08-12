// This cpp file is for the main ATmega32 Microcontroller.

//IR: portA all pins, portB 0-3
//speaker: portC 7
//octave++: portB 5
//octave--: portB 6
//play/stop: portD 2->INT0
//playprev: portD 0
//playnext: portD 1
//start/stop recording portD 3 -> INT1


#ifndef F_CPU
#define F_CPU 1600000UL // 16 MHz clock speed
#endif



#define note1 523.2511
#define note2 587.3295
#define note3 659.2551
#define note4 698.4565
#define note5 783.9909
#define note6 880.0000
#define note7 987.7666
#define note8 1068
#define note9 1397
#define note10 1480
#define note11 1568
#define note12 1630    
     
#define tot_playlist_size 10
#define __DELAY_BACKWARD_COMPATIBLE__
#define SPEAKER_PORT PORTC
#define SPEAKER_DDR DDRC
#define SPEAKER_PIN 7


#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "24c64.h"
#include "24c64.c"

volatile unsigned char octave = 1;
volatile unsigned char mode = 0x00; //mode 0 for normal piano play, mode 1 for record play
volatile int curr_playing = 0;
volatile int current_tune_index = 0;
volatile int tot_saved_tunes = 0;
volatile int end_address_of_tune;
volatile int recording = 0; //recording off
volatile int playlist[tot_playlist_size]; // array to keep each tunes end address, address starts from 0
volatile int low = 0; 
volatile int up = 0;
volatile int not_failed=1;

ISR(INT0_vect)
{
	mode=~mode; //invert mode
}

ISR(INT1_vect){
	if(recording==1){
		if(not_failed==1)
		{
			playlist[current_tune_index] = octave;
			playlist[current_tune_index+1] = end_address_of_tune;
			current_tune_index = current_tune_index + 2;
			tot_saved_tunes++;
		}
		recording = 0;   //recording mode off
	}
	else if(recording==0){
		recording = 1;   //recording mode on
		not_failed=1;
	}
}

void octave_plus(){
	if(octave < 4){
		octave = octave <<1;
	}
}

void octave_minus(){
	if(octave > 1){
		octave = octave >>1;
	}
}

void play_prev(){
	if(curr_playing>0){
		curr_playing--;
	}
	else
	{
		curr_playing=tot_saved_tunes-1;//assuming there is atleast one saved tune
	}
}

void play_next(){
	if(curr_playing<(tot_saved_tunes-1)){
		curr_playing++;
	}
	else
	{
		curr_playing=0;
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

int main(void)
{
	mode=0b00000000; //initialize to normal playing mode
	DDRA=0x00;
	DDRB=0x04;
	DDRD=0b11110000;
	PORTD &= 0b00001111;
	int previous_note = 0x00;
	int current_note = 0x00;
	unsigned int duration_count = 0;
	unsigned int upper_part = 0;
	unsigned int lower_part = 0;
	unsigned int address = 0;
	current_tune_index = 0;
	
	int i=0;
	for(i=0;i<tot_playlist_size;i++)
	{
		playlist[i]=0;
	}
	
	GICR |= (1<<INT0); //interrupt
	GICR |= (1<<INT1);
	MCUCR = MCUCR | 0b00000011; //set MCUCR for falling edge
	MCUCR = MCUCR | 0b00001100; //set MCUCR for falling edge
	
	sei(); //enable interrupt
	
	mode=0b00000000; //initialize to normal playing mode
	recording=0;
	curr_playing = 0;
	current_tune_index = 0;
	tot_saved_tunes = 0;
	not_failed=1;
	
	EEOpen();

	while (1)
	{
		if(!mode) //normal play
		{
			PORTB |= 0b00000100;
			PORTD|=0b00010000;
			if(recording)
			{
				PORTD |= 0b00100000;
			}
			else
			{
				PORTD &= 0b11011111;
			}
			if((PINA & 0b00000001)==0x00)
			{
				PLAYNOTE(note1 * octave);
				if(recording){
					current_note = 0b00000001;
					if(previous_note != current_note){
						upper_part = (duration_count & 0b1111111100000000) >> 8;
						lower_part = duration_count & 0b0000000011111111;
						
						//write previous_note
						if(!EEWriteByte(address,previous_note))
						{
							not_failed=0;
						}
						//write duration count
						if(!EEWriteByte(address+1,lower_part))
						{
							not_failed=0;
						}
						if(!EEWriteByte(address+2,upper_part))
						{
							not_failed=0;
						}
 						low = lower_part;
 						up = upper_part;
						address = address + 3;
						end_address_of_tune = address - 1;
					
						previous_note = current_note;
						duration_count = 1;
					}
					else{
						duration_count = duration_count + 1;
					}
				}
			}
			else if((PINA & 0b00000010)==0x00)
			{
				PLAYNOTE(note2 * octave);
				if(recording){
					current_note = 0b00000010;
					if(previous_note != current_note){
						lower_part = duration_count & 0x00FF;
						upper_part = (duration_count & 0xFF00) >> 8;
						
						//write previous_note
						if(!EEWriteByte(address,previous_note))
						{
							not_failed=0;
						}
						//write count
						if(!EEWriteByte(address+1,lower_part))
						{
							not_failed=0;
						}
						if(!EEWriteByte(address+2,upper_part))
						{
							not_failed=0;
						}
						address = address + 3;
						end_address_of_tune = address - 1;
					
						previous_note = current_note;
						duration_count = 1;
					}
					else{
						duration_count = duration_count + 1;
					}
				}
				
			}
			else if((PINA & 0b00000100)==0x00)
			{
				if(recording){
					current_note = 0b00000011;
					if(previous_note != current_note){
						lower_part = duration_count & 0x00FF;
						upper_part = (duration_count & 0xFF00) >> 8;
						//write previous_note
						
						if(!EEWriteByte(address,previous_note))
						{
							not_failed=0;
						}
						//write count
						if(!EEWriteByte(address+1,lower_part))
						{
							not_failed=0;
						}
						if(!EEWriteByte(address+2,upper_part))
						{
							not_failed=0;
						}
						_delay_ms(100);
						address = address + 3;
						end_address_of_tune = address - 1;
						previous_note = current_note;
						duration_count = 1;
					}
					else{
						duration_count++;
					}
				}
				PLAYNOTE(note3 * octave);
			}
			else if((PINA & 0b00001000)==0x00)
			{
				if(recording){
					current_note = 0b00000100;
					if(previous_note != current_note){
						lower_part = duration_count & 0x00FF;
						upper_part = (duration_count & 0xFF00) >> 8;
						//write previous_note
						if(!EEWriteByte(address,previous_note))
						{
							not_failed=0;
						}
						//write count
						if(!EEWriteByte(address+1,lower_part))
						{
							not_failed=0;
						}
						if(!EEWriteByte(address+2,upper_part))
						{
							not_failed=0;
						}
						_delay_ms(100);
						address = address + 3;
						end_address_of_tune = address - 1;
						previous_note = current_note;
						duration_count = 1;
					}
					else{
						duration_count++;
					}
				}
				PLAYNOTE(note4 * octave);
			}	
			else if((PINA & 0b00010000)==0x00)
			{
				if(recording){
					current_note = 0b00000101;
					if(previous_note != current_note){
						lower_part = duration_count & 0x00FF;
						upper_part = (duration_count & 0xFF00) >> 8;
						//write previous_note
						if(!EEWriteByte(address,previous_note))
						{
							not_failed=0;
						}
						//write count
						if(!EEWriteByte(address+1,lower_part))
						{
							not_failed=0;
						}
						if(!EEWriteByte(address+2,upper_part))
						{
							not_failed=0;
						}
						_delay_ms(100);
						address = address + 3;
						end_address_of_tune = address - 1;
						previous_note = current_note;
						duration_count = 1;
					}
					else{
						duration_count++;
					}
				}
				PLAYNOTE(note5 * octave);
			}
			else if((PINA & 0b00100000)==0x00)
			{
				if(recording){
					current_note = 0b00000110;
					if(previous_note != current_note){
						lower_part = duration_count & 0x00FF;
						upper_part = (duration_count & 0xFF00) >> 8;
						//write previous_note
						if(!EEWriteByte(address,previous_note))
						{
							not_failed=0;
						}
						//write count
						if(!EEWriteByte(address+1,lower_part))
						{
							not_failed=0;
						}
						if(!EEWriteByte(address+2,upper_part))
						{
							not_failed=0;
						}
						_delay_ms(100);
						address = address + 3;
						end_address_of_tune = address - 1;
						previous_note = current_note;
						duration_count = 1;
					}
					else{
						duration_count++;
					}
				}
				PLAYNOTE(note6 * octave);
			}	
			else if((PINA & 0b01000000)==0x00)
			{
				if(recording){
					current_note = 0b00000111;
					if(previous_note != current_note){
						lower_part = duration_count & 0x00FF;
						upper_part = (duration_count & 0xFF00) >> 8;
						//write previous_note
						if(!EEWriteByte(address,previous_note))
						{
							not_failed=0;
						}
						//write count
						if(!EEWriteByte(address+1,lower_part))
						{
							not_failed=0;
						}
						if(!EEWriteByte(address+2,upper_part))
						{
							not_failed=0;
						}
						_delay_ms(100);
						address = address + 3;
						end_address_of_tune = address - 1;
						previous_note = current_note;
						duration_count = 1;
					}
					else{
						duration_count++;
					}
				}
				PLAYNOTE(note7 * octave);
			}	
			else if((PINA & 0b10000000)==0x00)
			{
				if(recording){
					current_note = 0b00001000;
					if(previous_note != current_note){
						lower_part = duration_count & 0x00FF;
						upper_part = (duration_count & 0xFF00) >> 8;
						//write previous_note
						if(!EEWriteByte(address,previous_note))
						{
							not_failed=0;
						}
						//write count
						if(!EEWriteByte(address+1,lower_part))
						{
							not_failed=0;
						}
						if(!EEWriteByte(address+2,upper_part))
						{
							not_failed=0;
						}
						_delay_ms(100);
						address = address + 3;
						end_address_of_tune = address - 1;
						previous_note = current_note;
						duration_count = 1;
					}
					else{
						duration_count++;
					}
				}
				PLAYNOTE(note8 * octave);
			}		
			else if((PINB & 0b00000001)==0x00)
			{
				if(recording){
					current_note = 0b00001001;
					if(previous_note != current_note){
						lower_part = duration_count & 0x00FF;
						upper_part = (duration_count & 0xFF00) >> 8;
						//write previous_note
						if(!EEWriteByte(address,previous_note))
						{
							not_failed=0;
						}
						//write count
						if(!EEWriteByte(address+1,lower_part))
						{
							not_failed=0;
						}
						if(!EEWriteByte(address+2,upper_part))
						{
							not_failed=0;
						}
						_delay_ms(100);
						address = address + 3;
						end_address_of_tune = address - 1;
						previous_note = current_note;
						duration_count = 1;
					}
					else{
						duration_count++;
					}
				}
				PLAYNOTE(note9 * octave);
			}
			else if((PINB & 0b00000010)==0x00)
			{
				if(recording){
					current_note = 0b00001010;
					if(previous_note != current_note){
						lower_part = duration_count & 0x00FF;
						upper_part = (duration_count & 0xFF00) >> 8;
						//write previous_note
						if(!EEWriteByte(address,previous_note))
						{
							not_failed=0;
						}
						//write count
						if(!EEWriteByte(address+1,lower_part))
						{
							not_failed=0;
						}
						if(!EEWriteByte(address+2,upper_part))
						{
							not_failed=0;
						}
						_delay_ms(100);
						address = address + 3;
						end_address_of_tune = address - 1;
						previous_note = current_note;
						duration_count = 1;
					}
					else{
						duration_count++;
					}
				}
				PLAYNOTE(note10 * octave);
			}
			else if((PINB & 0b00001000)==0x00)
			{
				if(recording){
					current_note = 0b00001011;
					if(previous_note != current_note){
						lower_part = duration_count & 0x00FF;
						upper_part = (duration_count & 0xFF00) >> 8;
						//write previous_note
						if(!EEWriteByte(address,previous_note))
						{
							not_failed=0;
						}
						//write count
						if(!EEWriteByte(address+1,lower_part))
						{
							not_failed=0;
						}
						if(!EEWriteByte(address+2,upper_part))
						{
							not_failed=0;
						}
						_delay_ms(100);
						address = address + 3;
						end_address_of_tune = address - 1;
						previous_note = current_note;
						duration_count = 1;
					}
					else{
						duration_count++;
					}
				}
				PLAYNOTE(note11 * octave);
			}
			else if((PINB & 0b00010000)==0x00)
			{
				if(recording){
					current_note = 0b00001100;
					if(previous_note != current_note){
						lower_part = duration_count & 0x00FF;
						upper_part = (duration_count & 0xFF00) >> 8;
						//write previous_note
						if(!EEWriteByte(address,previous_note))
						{
							not_failed=0;
						}
						//write count
						if(!EEWriteByte(address+1,lower_part))
						{
							not_failed=0;
						}
						if(!EEWriteByte(address+2,upper_part))
						{
							not_failed=0;
						}
						_delay_ms(100);
						address = address + 3;
						end_address_of_tune = address - 1;
						previous_note = current_note;
						duration_count = 1;
					}
					else{
						duration_count++;
					}
				}
				PLAYNOTE(note12 * octave);
			}
			else
			{
				if(recording){
					current_note = 0b00001101;
					if(previous_note != current_note){
						upper_part = (duration_count & 0b1111111100000000) >> 8;
						lower_part = duration_count & 0b0000000011111111;
						//write previous note
						if(!EEWriteByte(address,previous_note))
						{
							not_failed=0;
							PORTD|= 0b01000000;
							_delay_ms(300);
						}
						//write duration count
						if(!EEWriteByte(address+1,lower_part))
						{
							not_failed=0;
						}
						if(!EEWriteByte(address+2,upper_part))
						{
							not_failed=0;
						}
 						low = lower_part;
 						up = upper_part;
						address = address + 3;
						end_address_of_tune = address - 1;
						PORTD&=0b10111111;
						previous_note = current_note;
						duration_count = 1;
					}
					else{
						duration_count = duration_count + 1;
					}
				}
			}
			//octave plus
			if(PINB & (1<<5))
			{
				octave_plus();
				_delay_ms(1000);
			}
			//octave minus
			else if(PINB & (1<<6))
			{
				octave_minus();
				_delay_ms(1000);
			}
			
			//play previous
			if(PIND & 1)
			{
				play_prev();
				_delay_ms(1000);
			}
			//play next
			else if(PIND & (1<<1))
			{
				play_next();
				_delay_ms(1000);
			}
		}
		else //record play
		{
			PORTB |= 0b00000100;
			PORTD&=0b11101111;
			if(tot_saved_tunes != 0) // atleast 1 saved tune
			{
				int start; //starting of saved tune
				if(curr_playing==0)
				{
					start=0;
				}
				else
				{
					start=playlist[((curr_playing-1)*2)+1]+1;
				}
				int endd=playlist[(curr_playing*2)+1];//ending of saved tune

				int play_octave=playlist[(curr_playing*2)];
				
				int eep_note=0; // note to play (numbering 1-13)
				uint16_t iter_note=0; // how many iterations of that note will be played
				uint8_t play_note_number;
				uint8_t play_lower_part = 0;
				uint8_t play_upper_part = 0;
				//read eeprom from address start to end
				int iter_read;
				for(iter_read = start ; iter_read <= endd ; iter_read+=3)
				{
					if(EEReadByte(iter_read))
					{
						PORTD|= 0b10000000;
						_delay_ms(300);
						PORTD&=0b01111111;
					}
					else
					{
						PORTD&=0b01111111;
					}
					//read from eeprom
					play_note_number = EEReadByte(iter_read);
					if(play_note_number >= 1 && play_note_number <= 13){
						
						PORTB &= 0b11111011;
						
					}
					else{
						PORTB |= 0b00000100;
					}
					play_lower_part = EEReadByte(iter_read+1);
					play_upper_part = EEReadByte(iter_read+2);
					//decode the saved info
					iter_note=play_upper_part;
					iter_note=iter_note<<8;
					iter_note = play_lower_part | iter_note;

					//set eepnote
					eep_note = play_note_number;
					
					uint16_t iter;
					if(eep_note==1)
					{
						iter=0;
						for(iter=0;iter<iter_note;iter++)
						{
							PLAYNOTE(note1*play_octave);
						}
					}
					else if (eep_note==2)
					{
						iter=0;
						for(iter=0;iter<iter_note;iter++)
						{
							PLAYNOTE(note2*play_octave);
						}
					}
					else if (eep_note==3)
					{
						iter=0;
						for(iter=0;iter<iter_note;iter++)
						{
							PLAYNOTE(note3*play_octave);
						}
					}
					else if (eep_note==4)
					{
						iter=0;
						for(iter=0;iter<iter_note;iter++)
						{
							PLAYNOTE(note4*play_octave);
						}
					}
					else if (eep_note==5)
					{
						iter=0;
						for(iter=0;iter<iter_note;iter++)
						{
							PLAYNOTE(note5*play_octave);
						}
					}
					else if (eep_note==6)
					{
						iter=0;
						for(iter=0;iter<iter_note;iter++)
						{
							PLAYNOTE(note6*play_octave);
						}
					}
					else if (eep_note==7)
					{
						iter=0;
						for(iter=0;iter<iter_note;iter++)
						{
							PLAYNOTE(note7*play_octave);
						}
					}
					else if (eep_note==8)
					{
						iter=0;
						for(iter=0;iter<iter_note;iter++)
						{
							PLAYNOTE(note8*play_octave);
						}
					}
					else if (eep_note==9)
					{
						iter=0;
						for(iter=0;iter<iter_note;iter++)
						{
							PLAYNOTE(note9*play_octave);
						}
					}
					else if (eep_note==10)
					{
						iter=0;
						for(iter=0;iter<iter_note;iter++)
						{
							PLAYNOTE(note10*play_octave);
						}
					}
					else if (eep_note==11)
					{
						iter=0;
						for(iter=0;iter<iter_note;iter++)
						{
							PLAYNOTE(note11*play_octave);
						}
					}
					else if (eep_note==12)
					{
						iter=0;
						for(iter=0;iter<iter_note;iter++)
						{
							PLAYNOTE(note12*play_octave);
						}
					}
					else if (eep_note==13)
					{
						PORTC = 0x00;
					}	
				}
			}
			//toggle mode to normal play after playing
			_delay_ms(1000);
			mode=~mode;
			
		}
		
	}
			
}


