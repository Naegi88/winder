//////////////////////////////////////
//									//
//			Silvan Nägeli			//
//		     29.03.2018				//
//		   RTC funktionen			//
//									//
//									//
//////////////////////////////////////




uint8_t RTCWriteByte(uint8_t u8addr, uint8_t u8data);
void RTCInit(void);
uint8_t RTCReadByte(uint8_t u8addr);
void ReadTime(void);
void WriteTime(void);
uint8_t SetTime(void);
void SetDate(void);

