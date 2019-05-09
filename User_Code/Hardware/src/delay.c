#include "delay.h"
#include "tim.h"


void delay_us(uint32_t us)
{
	uint32_t differ=0xffffffff-us-5;
	/*为防止因中断打断延时，造成计数错误.
     如从0xfffE开始延时1us,但由于中断打断
    （此时计数器仍在计数），本因计数至0xffff）
    便停止计数，但由于错过计数值，并重载arr值，
    导致实际延时(0xffff+1)us
    */
	
	HAL_TIM_Base_Start(&htim2);
	
	__HAL_TIM_SetCounter(&htim2,differ);
	
	while(differ<0xffffffff-5)
    {
        differ=__HAL_TIM_GetCounter(&htim2);
    }

    HAL_TIM_Base_Stop(&htim2);
}



void delay_ms(uint16_t ms)
{
	delay_us(ms*1000);

}
