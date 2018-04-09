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
#include "I2C.h"

#define F_CPU 16000000UL  // 1 MHz

char string[30] = "";








int main(void)
{
	/* Backlight pin PL3, set as output, set high for 100% output */
	DDRB |= (1<<PB2);
	PORTB |= (1<<PB2);
	//PORTB &= ~(1<<PB2);
	 
	DDRD &= ~((1<<PD6) | (1<<PD2) | (1<<PD5)); 	//Taster 1-3
	PORTD |= ((1<<PD6) | (1<<PD2) | (1<<PD5)); 	//PUllups für Taster einschalten

	
	TWIInit();
	
	glcd_init();
	glcd_clear();
	glcd_write();
	
	glcd_tiny_set_font(Font5x7,5,7,32,127);
	glcd_clear_buffer();
	
	RTCInit();
	
	
	while(1) 
	{
		ReadTime();
		
		WriteTime();
		glcd_draw_string_xy(15,20,string);
		
		SetTime();

	glcd_write();
	}//End of while
	
	return 0;
}//end of main











void RTCInit(void)
{
	RTCWriteByte(0x00, 0x80);
	RTCWriteByte(0x10, 0x00);
	RTCWriteByte(0x20, 0x00);
	RTCWriteByte(0x30, 0x00);
}




void ReadTime(void)
{
	sec_e 	= 	RTCReadByte(0x00) & 0x0F;
	sec_z 	= 	(RTCReadByte(0x00) & 0x70) >> 4;
	min_e 	= 	RTCReadByte(0x01) & 0x0F;
	min_z 	= 	(RTCReadByte(0x01) & 0x70) >> 4;
	h_e 	= 	RTCReadByte(0x02) & 0x0F;
	h_z 	= 	(RTCReadByte(0x02) & 0x30) >> 4;
	Wkday 	= 	(RTCReadByte(0x03) & 0x07) ;
	
}






void WriteTime(void)
{
	sprintf(string,"%d%d:%d%d:%d%d", h_z, h_e, min_z, min_e, sec_z, sec_e);
}

void WeekDay(uint8_t Wkday)
{
	switch(Wkday)
	{
		case 1: sprintf(string,"Montag");
			break;
			
		case 2: sprintf(string,"Dinstag");
			break;
			
		case 3: sprintf(string,"Mittwoch");
			break;
			
		case 4: sprintf(string,"Donnerstag");
			break;
			
		case 5: sprintf(string,"Freitag");
			break;
			
		case 6: sprintf(string,"Samstag");
			break;
			
		case 7: sprintf(string,"Sonntag");
			break;
	}
}

uint8_t RTCReadByte(uint8_t u8addr)
	
{
	uint8_t u8data = 0;
	
	TWIStart();
	TWIWrite(RTCWrite);
	TWIWrite(u8addr);
	TWIStart();
	TWIWrite(RTCRead);
	u8data = TWIReadNACK();
	TWIStop();
	
	return u8data;
}
	
uint8_t RTCWriteByte(uint8_t u8addr, uint8_t u8data)
	
{	
	TWIStart();
	TWIWrite(RTCWrite);  
	TWIWrite(u8addr);
	TWIWrite(u8data);
	TWIStop();
	
	_delay_ms(10);
	
	return 0;
}
void SetTime(void)
{
	if(!(PIND & (1<<PD2)))
		{
			glcd_clear();
			RTCWriteByte(0x00, 0x00);
			delay_ms(500);
			while(bst == 0)////////////////////////////////////////////////////////////Stunden////////////////////////////////////////////////////////////
			{
				sprintf(string,"Stunden:   %d%d",h_z, h_e);
				glcd_draw_string_xy(0,0,string);	
				
				if(!(PIND & (1<<PD5)))
				{
					h_e++; 
					delay_ms(75);
				}
				
				if(!(PIND & (1<<PD6)))
				{
					h_e--; 
					delay_ms(75);
				}
				
				if(!(PIND & (1<<PD2)))
				bst = 1;
				
				if((h_e == 255) && (h_z  == 0))
				{
					h_z = 2; 
					h_e = 4;
				}
				
				if((h_z == 2) && (h_e == 5))
				{
					h_z = 0;
					h_e = 0;
				}

				if(h_e == 255)
				{
					h_z--;
					h_e = 9;
				}
				
				if(h_e == 10)
				{
					h_z++;
					h_e = 0;
				}
				
				glcd_write();
			}////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			bst = 0;
			delay_ms(500);
			while(bst == 0)//////////////////////////////////////////////////////Minuten////////////////////////////////////////////////////////////////
			{
				sprintf(string,"Minuten:   %d%d",min_z, min_e);
				glcd_draw_string_xy(0,8,string);
				
				if(!(PIND & (1<<PD5)))
				{
					min_e++; 
					delay_ms(75);
				}
				
				if(!(PIND & (1<<PD6)))
				{
					min_e--; 
					delay_ms(75);
				}
				
				if(!(PIND & (1<<PD2)))
				bst = 1;
				
				if((min_e == 255) && (min_z  == 0))
				{
					min_z = 5; 
					min_e = 9;
				}
				
				if(min_z == 6)
				{
					min_z = 0;
					min_e = 0;
				}

				if(min_e == 255)
				{
					min_z--;
					min_e = 9;
				}
				
				if(min_e == 10)
				{
					min_z++;
					min_e = 0;
				}
				glcd_write();
			}//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			bst = 0;
			delay_ms(500);
			while(bst == 0)//////////////////////////////////////////////////////sekunden////////////////////////////////////////////////////////////////
			{
				sprintf(string,"Sekunden:  %d%d",sec_z, sec_e);
				glcd_draw_string_xy(0,16,string);
				
				if(!(PIND & (1<<PD5)))
				{
					sec_e++; 
					delay_ms(75);
				}
				
				if(!(PIND & (1<<PD6)))
				{
					sec_e--; 
					delay_ms(75);
				}
				
				if(!(PIND & (1<<PD2)))
				bst = 1;
				
				if((sec_e == 255) && (sec_z  == 0))
				{
					sec_z = 5; 
					sec_e = 9;
				}
				
				if(sec_z == 6)
				{
					sec_z = 0;
					sec_e = 0;
				}

				if(sec_e == 255)
				{
					sec_z--;
					sec_e = 9;
				}
				
				if(sec_e == 10)
				{
					sec_z++;
					sec_e = 0;
				}
				glcd_write();
			}//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			bst = 0;
			glcd_clear();
			
			
			RTCWriteByte(0x00, 0x80 + (sec_z << 4) + sec_e);
			RTCWriteByte(0x01, (min_z << 4) + min_e);
			RTCWriteByte(0x02, (h_z << 4) + h_e);
			delay_ms(500);
		}
}

void SetDate(void)
{
	
}