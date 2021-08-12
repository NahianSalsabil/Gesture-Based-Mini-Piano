//This cpp file is for second ATmega32 Microcontroller connected with LCD.

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
#include "lcd.h" 

int main(void)
{
	DDRD = 0xFF;
	DDRC = 0b11111111;
	DDRA = 0x00;
	DDRB = 0b11100100;

	int i;
	Lcd4_Init();
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	char fstr[]=" Frequency : x";
	Lcd4_Write_String(fstr);
	
	while(1)
	{
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
	}
}