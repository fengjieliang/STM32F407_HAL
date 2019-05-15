#ifndef _BSP_LCD_H
#define _BSP_LCD_H

#include "stm32f4xx_hal.h"
#include "fsmc.h"
#include "stdint.h"

#define  CMD_BASE       ((uint32_t)(0x6C000000 | 0x0000007E))
#define  DATA_BASE      ((uint32_t)(0x6C000000 | 0x00000080))

#define LCD_CMD       ( * (uint16_t *) CMD_BASE )
#define LCD_DATA      ( * (uint16_t *) DATA_BASE)


void bsp_lcd_init(void);






#endif
