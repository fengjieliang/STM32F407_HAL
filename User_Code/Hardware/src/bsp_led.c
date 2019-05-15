#include "bsp_led.h"



void bsp_led_on(uint32_t pin)
{
	HAL_GPIO_WritePin(GPIOF,pin,GPIO_PIN_RESET);
}



void bsp_led_off(uint32_t pin)
{
	HAL_GPIO_WritePin(GPIOF,pin,GPIO_PIN_SET);
}




void bsp_led_toggle(uint32_t pin)
{
	HAL_GPIO_TogglePin(GPIOF,pin);
}



