//////////////////////////////////////
//									//
//			Silvan Nägeli			//
//		     29.03.2018				//
//		   RTC funktionen			//
//									//
//									//
//////////////////////////////////////

#define RTCWrite 0xDE
#define RTCRead 0xDF


#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include "RTC.h"
#include "TWI.h"

uint8_t 	sec 	= 0;
uint8_t		sec_e 	= 0;
uint8_t		sec_z	= 0;
uint8_t 	min		= 0;
uint8_t		min_e	= 0;
uint8_t 	min_z	= 0;
uint8_t		h		= 0;
uint8_t 	h_e 	= 0;
uint8_t 	h_z 	= 0;
uint8_t		Wkday	= 0;
uint8_t 	bst		= 0;


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


void RTCInit(void)
{
	RTCWriteByte(0x00, 0x80);
	RTCWriteByte(0x10, 0x00);
	RTCWriteByte(0x20, 0x00);
	RTCWriteByte(0x30, 0x00);
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


void ReadTime(void)
{
	sec_e 	= 	RTCReadByte(0x00) & 0x0F;
	sec_z 	= 	(RTCReadByte(0x00) & 0x70) >> 4;
	min_e 	= 	RTCReadByte(0x01) & 0x0F;
	min_z 	= 	(RTCReadByte(0x01) & 0x70) >> 4;
	h_e 	= 	RTCReadByte(0x02) & 0x0F;
	h_z 	= 	(RTCReadByte(0x02) & 0x30) >> 4;
	Wkday 	= 	(RTCReadByte(0x03) & 0x07);
	
}


void WriteTime(void)
{
 	h = (h_z * 10) + h_e;
	min = (min_z * 10) + min_e;
	sec = (sec_z * 10) + sec_e;
}
	

uint8_t SetTime(void)
{
			while(bst == 0)////////////////////////////////////////////////////////////Stunden////////////////////////////////////////////////////////////
			{	
				glcd_clear();
				RTCWriteByte(0x00, 0x00);
				delay_ms(500);	
				
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
				
				h = (h_z * 10) + h_e;
				return h;
				
			}////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			
			while(bst == 1)//////////////////////////////////////////////////////Minuten////////////////////////////////////////////////////////////////
			{
				delay_ms(500);
				
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
				bst = 2;
				
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
				
				min = (min_z * 10) + min_e;
				return min;
			}//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			
			while(bst == 2)//////////////////////////////////////////////////////sekunden////////////////////////////////////////////////////////////////
			{
				delay_ms(500);
				
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
				bst = 3;
				
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
				sec = (sec_z * 10) + sec_e;
				return sec;
			}//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			
			glcd_clear();
			
			
			RTCWriteByte(0x00, 0x80 + (sec_z << 4) + sec_e);
			RTCWriteByte(0x01, (min_z << 4) + min_e);
			RTCWriteByte(0x02, (h_z << 4) + h_e);
			delay_ms(500);
			
			bst = 4;
			return 0;
		}

void SetDate(void)
{
	
}