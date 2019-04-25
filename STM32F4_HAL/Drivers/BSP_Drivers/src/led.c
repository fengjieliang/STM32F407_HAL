#include "led.h"

void bsp_led_on(uint16_t LED_Pin)
{
	HAL_GPIO_WritePin(GPIOF,LED_Pin,GPIO_PIN_RESET);
}


void bsp_led_off(uint16_t LED_Pin)
{
	HAL_GPIO_WritePin(GPIOF,LED_Pin,GPIO_PIN_SET);
}



void bsp_led_toggle(uint16_t LED_Pin)
{
	HAL_GPIO_TogglePin(GPIOF,LED_Pin);
}


void bsp_led_blinken(uint16_t LED_Pin, uint8_t num, uint16_t delay)
{
	for(int i=0;i<num*2;i++){
		bsp_led_toggle(LED_Pin);
		HAL_Delay(delay);
	}
}


