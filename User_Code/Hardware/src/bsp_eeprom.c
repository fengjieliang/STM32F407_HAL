#include "bsp_eeprom.h"
#include <stdio.h>
#include <string.h>
#define ADDR_24LCxx_Write 0xA0
#define ADDR_24LCxx_Read 0xA1
#define BufferSize 256
uint8_t WriteBuffer[BufferSize],ReadBuffer[BufferSize];


void eeprom_write(uint8_t *pData, uint16_t size,uint16_t MemAddress)
{
	
	if(size>8)
	{
		uint8_t write_cycle=size/8;
//		printf("write_cycle=%d\r\n",write_cycle);
		for(int i=0;i<write_cycle;i++)
		{
			if(HAL_I2C_Mem_Write(&hi2c1,ADDR_24LCxx_Write,MemAddress+i*8,I2C_MEMADD_SIZE_8BIT,&pData[i*8],8,0xffff)!=HAL_OK)
			{printf("I2C write error\r\n");}
//			else
//			{
//				for(int j=0;j<8;j++)
//				{printf("pData[%d]=0x%x\r\n",i*8+j,pData[i*8+j]);}
//			}
			HAL_Delay(50);
		}
		//写入剩余的数据(不足8bytes)
		if(HAL_I2C_Mem_Write(&hi2c1,ADDR_24LCxx_Write,MemAddress+write_cycle*8,I2C_MEMADD_SIZE_8BIT,&pData[write_cycle*8],size%8,0xffff)!=HAL_OK)
				printf("I2C write error\r\n");
		
	
	}
	
	else
	{
		if(HAL_I2C_Mem_Write(&hi2c1,ADDR_24LCxx_Write,MemAddress,I2C_MEMADD_SIZE_8BIT,pData,size,0xffff)!=HAL_OK)
			printf("I2C write error\r\n");
	}
}




void eeprom_read(uint8_t *pData, uint16_t size,uint16_t MemAddress)
{
	if(HAL_I2C_Mem_Read(&hi2c1,ADDR_24LCxx_Read,MemAddress,I2C_MEMADD_SIZE_8BIT,pData,size,0xffff)==HAL_OK)
		printf("I2C read ok\r\n");
	else
		printf("I2C read error\r\n");
	

}



void eeprom_test()
{
	for(int i=0;i<256;i++)
	{
		WriteBuffer[i]=i;
//		printf("WriteBuffer[%d]=0x%x\r\n",i,WriteBuffer[i]);
	}
	eeprom_write(WriteBuffer,100,0);
	HAL_Delay(100);
	eeprom_read(ReadBuffer,100,0);
	for(int i=0;i<100;i++)
	{
		printf("ReadBuffer[%d]=0x%x\r\n",i,ReadBuffer[i]);
	}
	
	if(strcmp((char *)WriteBuffer,(char *)ReadBuffer)==0)
	{printf("eeprom test passed");}
	else
	{printf("eeprom test not passed");}
	

}