/////////////////////////////////////////////////////////////////////////////////////
//																				   //
//		Silvan Nägeli															   //
//																				   //
//		13.04.2018																   //
//																				   //
//		main.c / RS232															   //
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

#define F_CPU 8000000UL  
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE ((((F_CPU / 16) + (USART_BAUDRATE )) / (USART_BAUDRATE)) - 1)

void SendByte(uint8_t ByteToSend);
void SendString(char *s);

char string[30] = "";
uint8_t ReceivedByte = 0;
uint8_t x = 0;


ISR (USART_RX_vect, ISR_BLOCK)
{
	
	
}

 
int main(void)
{
	/* Backlight pin PL3, set as output, set high for 100% output */
	DDRB |= (1<<PB2);
	
	//PORTB &= ~(1<<PB2);
	 
	DDRD &= ~((1<<PD6) | (1<<PD2) | (1<<PD5)); 	//Taster 1-3
	PORTD |= ((1<<PD6) | (1<<PD2) | (1<<PD5)); 	//PUllups für Taster einschalten
	
	UCSR0B = (1 << RXEN0)  | (1 << TXEN0) | (1 << RXCIE0); // turn on the transmission and reception circuitry
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01); // Use 8-bit character sizes
	
	UBRR0H = (BAUD_PRESCALE >> 8);
	UBRR0L = BAUD_PRESCALE;
	
	sei();
	
	
	glcd_init();
	glcd_clear();
	
	
	glcd_tiny_set_font(Font5x7,5,7,32,127);
	glcd_clear_buffer();
 
	while(1) 
	{
		sprintf(string,"%c", ReceivedByte);
		glcd_draw_string_xy(0,0,string);
		
		x = 3;
		
		if(!( PIND & ( 1 << PD2 )))
		{
			x = 1;
		}
		
		
		if(!( PIND & ( 1 << PD6 )))
		{
			x = 2;
		}
		
		switch (x)
		{
			case 1:
			
			SendString("                                                                                ");
			SendString("                                       (|)                                      ");
			SendString("                                      ((|))                                     ");
			SendString("                                     (((|)))                                    ");
			SendString("                                    | ^  ^  |                                   ");
			SendString("                                    |  ?    |                                   ");
			SendString("                                    | <__>  |                                   ");
			SendString("                                     -------                                    ");
			SendString("                                       | |                                      ");
			SendString("                                                                                ");
			SendString("          III                          DDD                                      ");
			SendString("           I            i    t         D  D                                     ");
			SendString("           I   n nnn       ttttt       D   D   ooo   n nnn    eeee              ");
			SendString("           I   nn   n  ii    t         D   D  o   o  nn   n  e    e             ");
			SendString("           I   n    n   i    t         D   D  o   o  n    n  eeeeee             ");
			SendString("           I   n    n   i    t         D  D   o   o  n    n  e                  ");
			SendString("          III  n    n   i     tt       DDD     ooo   n    n   eeee              ");
				break;
				
			case 2:
				break;
				
			case 3:	
				break;
		
			case 4:
				break;
		}
		
		glcd_write();
	}//End of while
	
	return 0;
}//end of main



void SendByte(uint8_t ByteToSend)
{
	while((UCSR0A & (1 << UDRE0)) == 0){};
	UDR0 = ByteToSend;
}

void SendString(char *s)
{
	while(*s)
	{
		SendByte(*s);
		s++;
	}
}