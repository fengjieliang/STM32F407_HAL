#include "DS18B20.h"
#include "delay.h"
#define DS18B20_Data_Pin	GPIO_PIN_9


void DS18B20_Pin_Out_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOG_CLK_ENABLE();
	
	GPIO_InitStruct.Pin=DS18B20_Data_Pin;
	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull=GPIO_PULLUP;
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}


void DS18B20_Pin_In_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOG_CLK_ENABLE();

	GPIO_InitStruct.Pin=DS18B20_Data_Pin;
	GPIO_InitStruct.Mode=GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull=GPIO_NOPULL;
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}


void DS18B20_Init()
{
		DS18B20_Pin_Out_Init();
		HAL_GPIO_WritePin(GPIOG,DS18B20_Data_Pin,GPIO_PIN_SET);
		delay_us(50);
		HAL_GPIO_WritePin(GPIOG,DS18B20_Data_Pin,GPIO_PIN_RESET);
		delay_us(600);
		HAL_GPIO_WritePin(GPIOG,DS18B20_Data_Pin,GPIO_PIN_SET);
		delay_us(70);
	
		DS18B20_Pin_In_Init();
		delay_us(240);
		DS18B20_Pin_Out_Init();
		HAL_GPIO_WritePin(GPIOG,DS18B20_Data_Pin,GPIO_PIN_SET);
}





/**
 * @brief write 1 byte data to temperature sensor DS18B20.
 */
void DS18B20_Write(uint8_t data)
{
	DS18B20_Pin_Out_Init();	
	for (int i = 0; i < 8; i++)
	{
			HAL_GPIO_WritePin(GPIOG,DS18B20_Data_Pin,GPIO_PIN_SET);
			delay_us(10);
			HAL_GPIO_WritePin(GPIOG,DS18B20_Data_Pin,GPIO_PIN_RESET);
			if (data & 0x01)
					HAL_GPIO_WritePin(GPIOG,DS18B20_Data_Pin,GPIO_PIN_SET);
			else
					HAL_GPIO_WritePin(GPIOG,DS18B20_Data_Pin,GPIO_PIN_RESET);

			delay_us(80);
			data >>= 1;
	}
}



/**
 * @brief read 1 byte data from temperature sensor DS18B20.
 */
uint8_t DS18B20_Read()
{
    uint8_t data = 0;

    for (int i = 0; i < 8; i++)
    {
        DS18B20_Pin_Out_Init();	
        HAL_GPIO_WritePin(GPIOG,DS18B20_Data_Pin,GPIO_PIN_SET);
        delay_us(10);
        HAL_GPIO_WritePin(GPIOG,DS18B20_Data_Pin,GPIO_PIN_RESET);
        delay_us(1);
        HAL_GPIO_WritePin(GPIOG,DS18B20_Data_Pin,GPIO_PIN_SET);
        data >>= 1;
        DS18B20_Pin_In_Init();
        if (HAL_GPIO_ReadPin(GPIOG,DS18B20_Data_Pin))
            data |= 0x80;
        delay_us(60);


    }
    DS18B20_Pin_Out_Init();	
    HAL_GPIO_WritePin(GPIOG,DS18B20_Data_Pin,GPIO_PIN_SET);

    return data;
}


/**
 * @brief Function for reading temperature from sensor DS18B20.
 * @param[out] temperature.
 */
float ReadTemperature_DS18B20(void)
{
    float temp_DS18B20 = 0;
		uint8_t temp_L, temp_H;
		int16_t temp_int16;
	
    DS18B20_Init();
    DS18B20_Write(0xcc);                        //skip rom
    DS18B20_Write(0x44);                        //convert temperature
    delay_us(20);
    DS18B20_Init();
    DS18B20_Write(0xcc);                        //skip rom
    DS18B20_Write(0xbe);                        //Read Scratchpad
    temp_L = DS18B20_Read();
    temp_H = DS18B20_Read();
    temp_int16 = ((temp_H << 8) | temp_L);
    temp_DS18B20 = temp_int16 * 0.0625;            //resolution 12bit
    delay_us(100);

    return temp_DS18B20;

}





