#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include "lcd.h" //Can be download from the bottom of this article

// volatile unsigned char octave = 1;
// 
// void octave_plus(){
// 	if(octave < 4){
// 		octave = octave <<1;
// 	}
// }
// 
// void octave_minus(){
// 	if(octave > 1){
// 		octave = octave >>1;
// 	}
// }

int main(void)
{
	DDRD = 0xFF;
	//DDRC = 0b11110011;
	DDRC = 0b11111111;
	DDRA = 0x00;
	DDRB = 0b11100100;
	
	//char outstr[2];
	
	int i;
	Lcd4_Init();
// 	Lcd4_Clear();
// 	Lcd4_Set_Cursor(1,0);
// 	char hstr[]="   MINI PIANO   ";
// 	Lcd4_Write_String(hstr);
// 	
// 	_delay_ms(500);
	
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	char fstr[]=" Frequency : x";
	Lcd4_Write_String(fstr);
	
// 	Lcd4_Set_Cursor(2,0);
// 	char ostr[]=" Octave : x";
// 	Lcd4_Write_String(ostr);
	
	while(1)
	{
// 		Lcd4_Set_Cursor(2,10);
// 		//dtostrf(octave,1,0,outstr);
// 		//Lcd4_Write_String(outstr);
// 		if(octave==1)
// 		{
// 			char ostr11[]="1    ";
// 			Lcd4_Write_String(ostr11);
// 		}
// 		else if (octave==2)
// 		{
// 			char ostr22[]="2    ";
// 			Lcd4_Write_String(ostr22);
// 		}
// 		else if (octave==4)
// 		{
// 			char ostr44[]="4    ";
// 			Lcd4_Write_String(ostr44);
// 		}
// 		else
// 		{
// 			char ostrxx[]="x    ";
// 			Lcd4_Write_String(ostrxx);
// 		}

		if((PINA & 0b00000001)==0x00)
		{
			Lcd4_Set_Cursor(1,13);
			char fstr1[]="C  ";
			Lcd4_Write_String(fstr1);
		}
		else if((PINA & 0b00000010)==0x00)
		{
			Lcd4_Set_Cursor(1,13);
			char fstr2[]="C# ";
			Lcd4_Write_String(fstr2);
		}
		else if((PINA & 0b00000100)==0x00)
		{
			Lcd4_Set_Cursor(1,13);
			char fstr3[]="D  ";
			Lcd4_Write_String(fstr3);
		}
		else if((PINA & 0b00001000)==0x00)
		{
			Lcd4_Set_Cursor(1,13);
			char fstr4[]="D# ";
			Lcd4_Write_String(fstr4);
		}
		else if((PINA & 0b00010000)==0x00)
		{
			Lcd4_Set_Cursor(1,13);
			char fstr5[]="E  ";
			Lcd4_Write_String(fstr5);	
		}
		else if((PINA & 0b00100000)==0x00)
		{
			Lcd4_Set_Cursor(1,13);
			char fstr6[]="F  ";
			Lcd4_Write_String(fstr6);
		}
		else if((PINA & 0b01000000)==0x00)
		{
			Lcd4_Set_Cursor(1,13);
			char fstr7[]="F# ";
			Lcd4_Write_String(fstr7);
		}
		else if((PINA & 0b10000000)==0x00)
		{
			Lcd4_Set_Cursor(1,13);
			char fstr8[]="G ";
			Lcd4_Write_String(fstr8);	
		}
		else if((PINB & 0b00000001)==0x00)
		{
			Lcd4_Set_Cursor(1,13);
			char fstr9[]="G# ";
			Lcd4_Write_String(fstr9);
		}
		else if((PINB & 0b00000010)==0x00)
		{
			Lcd4_Set_Cursor(1,13);
			char fstr10[]="A ";
			Lcd4_Write_String(fstr10);	
		}
		else if((PINB & 0b00001000)==0x00)
		{
			Lcd4_Set_Cursor(1,13);
			char fstr11[]="A# ";
			Lcd4_Write_String(fstr11);	
		}
		else if((PINB & 0b00010000)==0x00)
		{
			Lcd4_Set_Cursor(1,13);
			char fstr12[]="B  ";
			Lcd4_Write_String(fstr12);
		}
		else
		{
			Lcd4_Set_Cursor(1,13);
			char fstr13[]="x  ";
			Lcd4_Write_String(fstr13);
		}
		
// 		if(PINC & (1<<2))
// 		{
// 			octave_plus();
// 			_delay_ms(1000);
// 		}
// 		else if(PINC & (1<<3))
// 		{
// 			octave_minus();
// 			_delay_ms(1000);
// 		}
		
		
	}
}