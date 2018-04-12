////////////////////////////////////////////////////////////////////////////////////
//																				   //
//		Silvan Nägeli															   //
//																				   //
//		06.04.2018																   //
//																				   //
//		main.c / winder													   //
//																				   //
//		github: https://github.com/Naegi88										   //
//																				   //
/////////////////////////////////////////////////////////////////////////////////////

#include <avr/io.h>
#include "glcd/glcd.h"
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "glcd/fonts/Liberation_Sans15x21_Numbers.h"
#include "glcd/fonts/font5x7.h"
#include <avr/pgmspace.h>

#include "I2C/RTC.h"
#include "I2C/EEPROM.h"
#include "I2C/TWI.h"


#define F_CPU 16000000UL  // 1 MHz

char string[30] = "";
 
 
int main(void)
{
	/* Backlight pin PL3, set as output, set high for 100% output */
	DDRB |= (1<<PB2);
	//PORTB |= (1<<PB2);
	PORTB &= ~(1<<PB2);
	 
	DDRD &= ~((1<<PD6) | (1<<PD2) | (1<<PD5)); 	//Taster 1-3
	PORTD |= ((1<<PD6) | (1<<PD2) | (1<<PD5)); 	//PUllups für Taster einschalten

	
	TWIInit();
	glcd_init();
	glcd_clear();
	glcd_write();
	
	glcd_tiny_set_font(Font5x7,5,7,32,127);
	glcd_clear_buffer();
	
	RTCInit();
	
	sprintf(string,"Hello World");
	glcd_draw_string_xy(10,18,string);
	
	while(1) 
	{
		glcd_write();
	}//End of while
	
	return 0;
}//end of main
