#ifndef BSP_LED_H
#define BSP_LED_H

#include "gpio.h"



void bsp_led_on(uint16_t LED_Pin);

void bsp_led_off(uint16_t LED_Pin);

void bsp_led_toggle(uint16_t LED_Pin);

void bsp_led_blinken(uint16_t LED_Pin, uint8_t num, uint16_t delay);


#endif
