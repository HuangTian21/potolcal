#ifndef	__EEPROM_H
#define	__EEPROM_H

#include	"config.h"

extern void	DisableEEPROM(void);
extern void EEPROM_read_n(u16 EE_address,u8 *DataAddress,u16 number);
extern void EEPROM_write_n(u16 EE_address,u8 *DataAddress,u16 number);
extern void	EEPROM_SectorErase(u16 EE_address);
void EEPROM_write_Save_Other(u16 EE_address,u8 *DataAddress,u16 number);

void EEPROM_write_n_Sector(u16 EE_address,u8 *DataAddress,u16 number);
#endif