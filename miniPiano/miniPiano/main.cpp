/*
 * miniPiano.cpp
 *
 * Created: 17-Jun-21 11:21:48 AM
 * Author : Lenovo
 */ 

#include <avr/io.h>


int main(void)
{
	/* Replace with your application code */
	DDRA=0x00;
	DDRD=0xFF;
	while (1)
	{
		PORTD=PINA;
	}
}



