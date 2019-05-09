#include "delay.h"
#include "tim.h"


void delay_us(uint32_t us)
{
	uint32_t differ=0xffffffff-us-5;
	/*Ϊ��ֹ���жϴ����ʱ����ɼ�������.
     ���0xfffE��ʼ��ʱ1us,�������жϴ��
    ����ʱ���������ڼ����������������0xffff��
    ��ֹͣ�����������ڴ������ֵ��������arrֵ��
    ����ʵ����ʱ(0xffff+1)us
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
