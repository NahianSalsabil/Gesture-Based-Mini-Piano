/*
 * miniPiano.cpp
 *
 * Created: 17-Jun-21 11:21:48 AM
 * Author : Adrita, Nahian, Rittik
 */ 

//IR: portA all pins, portB 0-3
//speaker: portC 7
//octave++: portB 5
//octave--: portB 6
//play/stop: portD 2->INT0
//playprev: portD 0
//playnext: portD 1



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

#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC5
#define EN eS_PORTC6      



#define tot_playlist_size 10
#define __DELAY_BACKWARD_COMPATIBLE__
#define SPEAKER_PORT PORTC
#define SPEAKER_DDR DDRC
#define SPEAKER_PIN 7


#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "lcd.h"
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


ISR(INT0_vect)
{
	mode=~mode; //invert mode
}

ISR(INT2_vect){
	if(recording){
		playlist[current_tune_index] = octave;
		playlist[current_tune_index+1] = end_address_of_tune;
		current_tune_index = current_tune_index + 2;
		tot_saved_tunes++;
		recording = 0;   //recording mode off
	}
	else recording = 1;   //recording mode on
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
	/* Replace with your application code */
	mode=0b00000000; //init to normal playing mode
	DDRA=0x00;
	DDRB=0x00;
	DDRD=0b11111000;
	DDRC = 0xFF;
	
	Lcd4_Init();
	int previous_note = 0x00;
	int current_note = 0xFF;
	int duration_count = 0x0000;
	int upper_part;
	int lower_part;
	unsigned int address = 0;
	
	char outstr[20];
	
	
	int i=0;
	for(i=0;i<tot_playlist_size;i++)
	{
		playlist[i]=0;
	}
	
	GICR |= (1<<INT0); //interrupt
	GICR |= (1<<INT2);
	MCUCR = MCUCR | 0b00000011; //set MCUCR for falling edge
	MCUCSR |= 0b10111111;
	sei(); //enable interrupt
	
	mode=0b00000000; //init to normal playing mode
	
	EEOpen();

	while (1)
	{
		
		if(!mode) //normal play
		{
			if((PINA & 0b00000001)==0x00)
			{
				//PORTC=0b00000001;
				//PORTD=0b00000000;
				current_note = 0b00000001;
				if(previous_note != current_note){
					
					if(recording){
						lower_part = duration_count & 0x00FF;
						upper_part = (duration_count & 0xFF00) >> 8;
						//eeprom write
						//write previous_note
						//write count
						EEWriteByte(address,1);
						EEWriteByte(address+1,lower_part);
						EEWriteByte(address+2,upper_part);
						address = address + 2;
						end_address_of_tune = address;
						
					}
					
					/*Lcd4_Set_Cursor(1,1);
					dtostrf(lower_part,4,0,outstr);
					Lcd4_Write_String(outstr);
					Lcd4_Set_Cursor(1,6);
					dtostrf(upper_part,4,0,outstr);
					Lcd4_Write_String(outstr);
					Lcd4_Set_Cursor(2,1);
					dtostrf(duration_count,8,0,outstr);
					Lcd4_Write_String(outstr);
					_delay_ms(2000);*/
					
					previous_note = current_note;
					duration_count = 1;
				}
				else{
					Lcd4_Clear();
					duration_count = duration_count + 1;
				}
				PLAYNOTE(note1 * octave);
			}
			else if((PINA & 0b00000010)==0x00)
			{
				//PORTC=0b00000010;
				//PORTD=0b00000000;
				current_note = 0b00000010;
				if(previous_note != current_note){
					if(recording){
						lower_part = duration_count & 0x00FF;
						upper_part = (duration_count & 0xFF00) >> 8;
						//eeprom write
						//write previous_note
						//write count
						EEWriteByte(address,1);
						EEWriteByte(address+1,lower_part);
						EEWriteByte(address+2,upper_part);
						address = address + 2;
						end_address_of_tune = address;
						
					}
					
					previous_note = current_note;
					duration_count = 1;
				}
				else{
					duration_count = duration_count + 1;
				}
				PLAYNOTE(note2 * octave);
			}
			else if((PINA & 0b00000100)==0x00)
			{
				//PORTC=0b00000100;
				//PORTD=0b00000000;
				current_note = 0b00000011;
				if(previous_note != current_note){
					if(recording){
						lower_part = duration_count & 0x00FF;
						upper_part = (duration_count & 0xFF00) >> 8;
						//eeprom write
						//write previous_note
						//write count
						EEWriteByte(address,1);
						EEWriteByte(address+1,lower_part);
						EEWriteByte(address+2,upper_part);
						address = address + 2;
						end_address_of_tune = address;
					}
					
					previous_note = current_note;
					duration_count = 1;
				}
				else{
					duration_count++;
				}
				PLAYNOTE(note3 * octave);
			}
			else if((PINA & 0b00001000)==0x00)
			{
				//PORTC=0b00001000;
				//PORTD=0b00000000;
				current_note = 0b00000100;
				if(previous_note != current_note){
					if(recording){
						lower_part = duration_count & 0x00FF;
						upper_part = (duration_count & 0xFF00) >> 8;
						//eeprom write
						//write previous_note
						//write count
						EEWriteByte(address,1);
						EEWriteByte(address+1,lower_part);
						EEWriteByte(address+2,upper_part);
						address = address + 2;
						end_address_of_tune = address;
					}
					
					previous_note = current_note;
					duration_count = 1;
				}
				else{
					duration_count++;
				}
				PLAYNOTE(note4 * octave);
			}	
			else if((PINA & 0b00010000)==0x00)
			{
				//PORTC=0b00010000;
				//PORTD=0b00000000;
				current_note = 0b00000101;
				if(previous_note != current_note){
					if(recording){
						lower_part = duration_count & 0x00FF;
						upper_part = (duration_count & 0xFF00) >> 8;
						//eeprom write
						//write previous_note
						//write count
						EEWriteByte(address,1);
						EEWriteByte(address+1,lower_part);
						EEWriteByte(address+2,upper_part);
						address = address + 2;
						end_address_of_tune = address;
					}
					
					previous_note = current_note;
					duration_count = 1;
				}
				else{
					duration_count++;
				}
				PLAYNOTE(note5 * octave);
			}
			else if((PINA & 0b00100000)==0x00)
			{
				//PORTC=0b00100000;
				//PORTD=0b00000000;
				current_note = 0b00000110;
				if(previous_note != current_note){
					if(recording){
						lower_part = duration_count & 0x00FF;
						upper_part = (duration_count & 0xFF00) >> 8;
						//eeprom write
						//write previous_note
						//write count
						EEWriteByte(address,1);
						EEWriteByte(address+1,lower_part);
						EEWriteByte(address+2,upper_part);
						address = address + 2;
						end_address_of_tune = address;
					}
					
					previous_note = current_note;
					duration_count = 1;
				}
				else{
					duration_count++;
				}
				PLAYNOTE(note6 * octave);
			}	
			else if((PINA & 0b01000000)==0x00)
			{
				//PORTC=0b01000000;
				//PORTD=0b00000000;
				current_note = 0b00000111;
				if(previous_note != current_note){
					if(recording){
						lower_part = duration_count & 0x00FF;
						upper_part = (duration_count & 0xFF00) >> 8;
						//eeprom write
						//write previous_note
						//write count
						EEWriteByte(address,1);
						EEWriteByte(address+1,lower_part);
						EEWriteByte(address+2,upper_part);
						address = address + 2;
						end_address_of_tune = address;
					}
					
					previous_note = current_note;
					duration_count = 1;
				}
				else{
					duration_count++;
				}
				PLAYNOTE(note7 * octave);
			}	
			else if((PINA & 0b10000000)==0x00)
			{
				//PORTC=0b10000000;
				//PORTD=0b00000000;
				current_note = 0b00001000;
				if(previous_note != current_note){
					if(recording){
						lower_part = duration_count & 0x00FF;
						upper_part = (duration_count & 0xFF00) >> 8;
						//eeprom write
						//write previous_note
						//write count
						EEWriteByte(address,1);
						EEWriteByte(address+1,lower_part);
						EEWriteByte(address+2,upper_part);
						address = address + 2;
						end_address_of_tune = address;
					}
					
					previous_note = current_note;
					duration_count = 1;
				}
				else{
					duration_count++;
				}
				PLAYNOTE(note8 * octave);
			}		
			else if((PINB & 0b00000001)==0x00)
			{
				//PORTC=0b00000000;
				//PORTD=0b00000001;
				current_note = 0b00001001;
				if(previous_note != current_note){
					if(recording){
						lower_part = duration_count & 0x00FF;
						upper_part = (duration_count & 0xFF00) >> 8;
						//eeprom write
						//write previous_note
						//write count
						EEWriteByte(address,1);
						EEWriteByte(address+1,lower_part);
						EEWriteByte(address+2,upper_part);
						address = address + 2;
						end_address_of_tune = address;
					}
					
					previous_note = current_note;
					duration_count = 1;
				}
				else{
					duration_count++;
				}
				PLAYNOTE(note9 * octave);
			}
			else if((PINB & 0b00000010)==0x00)
			{
				//PORTC=0b00000000;
				//PORTD=0b00000010;
				current_note = 0b00001010;
				if(previous_note != current_note){
					if(recording){
						lower_part = duration_count & 0x00FF;
						upper_part = (duration_count & 0xFF00) >> 8;
						//eeprom write
						//write previous_note
						//write count
						EEWriteByte(address,1);
						EEWriteByte(address+1,lower_part);
						EEWriteByte(address+2,upper_part);
						address = address + 2;
						end_address_of_tune = address;
					}
					
					previous_note = current_note;
					duration_count = 1;
				}
				else{
					duration_count++;
				}
				PLAYNOTE(note10 * octave);
			}
			else if((PINB & 0b00000100)==0x00)
			{
				//PORTC=0b00000000;
				//PORTD=0b00000100;
				current_note = 0b00001011;
				if(previous_note != current_note){
					if(recording){
						lower_part = duration_count & 0x00FF;
						upper_part = (duration_count & 0xFF00) >> 8;
						//eeprom write
						//write previous_note
						//write count
						EEWriteByte(address,1);
						EEWriteByte(address+1,lower_part);
						EEWriteByte(address+2,upper_part);
						address = address + 2;
						end_address_of_tune = address;
					}
					
					previous_note = current_note;
					duration_count = 1;
				}
				else{
					duration_count++;
				}
				PLAYNOTE(note11 * octave);
			}
			else if((PINB & 0b00001000)==0x00)
			{
				//PORTC=0b00000000;
				//PORTD=0b00001000;
				current_note = 0b00001100;
				if(previous_note != current_note){
					if(recording){
						lower_part = duration_count & 0x00FF;
						upper_part = (duration_count & 0xFF00) >> 8;
						//eeprom write
						//write previous_note
						//write count
						EEWriteByte(address,1);
						EEWriteByte(address+1,lower_part);
						EEWriteByte(address+2,upper_part);
						address = address + 2;
						end_address_of_tune = address;
					}
					
					previous_note = current_note;
					duration_count = 1;
				}
				else{
					duration_count++;
				}
				PLAYNOTE(note12 * octave);
			}			
			else
			{
				PORTC=0x00;
				
				current_note = 0b00001101;
				if(previous_note != current_note){
					if(recording){
						lower_part = duration_count & 0x00FF;
						upper_part = (duration_count & 0xFF00) >> 8;
						//eeprom write
						//write previous_note
						//write count
					}
					Lcd4_Set_Cursor(1,1);
					dtostrf(lower_part,4,0,outstr);
					Lcd4_Write_String(outstr);
					Lcd4_Set_Cursor(1,6);
					dtostrf(upper_part,4,0,outstr);
					Lcd4_Write_String(outstr);
					Lcd4_Set_Cursor(2,1);
					dtostrf(duration_count,8,0,outstr);
					Lcd4_Write_String(outstr);
					_delay_ms(2000);
					previous_note = current_note;
					duration_count = 1;
				}
				else{
					Lcd4_Clear();
					duration_count = duration_count + 1;
				}
				//PORTD=0x00;
			}
		
		
			if(PINB & (1<<5))
			{
				octave_plus();
				_delay_ms(1000);
			}
			else if(PINB & (1<<6))
			{
				octave_minus();
				_delay_ms(1000);
			}
			
		
			
			//play prev and next
			if(PIND & 1)
			{
				play_prev();
				_delay_ms(1000);
			}
			else if(PIND & (1<<1))
			{
				play_next();
				_delay_ms(1000);
			}
			
			
		}
		else //record play
		{
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
				int iter_note=0; // how many iterations of that note will be played
				
				//read eeprom from address start to end
				int iter_read;
				for(iter_read = start ; iter_read <= endd ; iter_read++)
				{
					//read from eeprom
					//decode the saved info
					//set eepnote
					//set iteration number
				}
				
				int iter;
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
					iter=0;
					for(iter=0;iter<iter_note;iter++)
					{
						//silence
						PORTC=0x00;
					}
				}
				
				//toggle mode to normal play after playing
				if(iter_read==endd)
				{
					mode=~mode;
				}	
				
			}
			
		}
		
	}
			
}



