//////////////////////////////////////
//									//
//			Silvan Nägeli			//
//		     29.03.2018				//
//		  eeprom funktionen			//
//									//
//									//
//////////////////////////////////////




uint8_t EEWriteByte ( uint16_t u16addr, uint8_t u8data );
uint8_t EEReadByte ( uint16_t u16addr );
void EEWriteWord (uint16_t speiste, uint32_t var32);
uint32_t EEReadWord(uint16_t speiste);

/*
uint8_t EEWritePage(uint8_t page, uint8_t u8data)
uint8_t EEReadPage(uint8_t page, uint8_t u8data)*/