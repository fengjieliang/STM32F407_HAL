#ifndef BSP_LED_H
#define BSP_LED_H

#include "stm32f4xx_hal.h"
#include "gpio.h"



void bsp_led_on(uint32_t pin);
void bsp_led_off(uint32_t pin);
void bsp_led_toggle(uint32_t pin);






#endif
