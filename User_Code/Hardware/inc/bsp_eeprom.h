#ifndef BSP_EEPROM_H
#define BSP_EEPROM_H

#include "i2c.h"
void eeprom_write(uint8_t *pData, uint16_t size,uint16_t MemAddress);

void eeprom_read(uint8_t *pData, uint16_t size,uint16_t MemAddress);


void eeprom_test();













#endif
