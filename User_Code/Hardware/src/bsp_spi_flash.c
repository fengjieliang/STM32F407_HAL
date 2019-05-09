#include "bsp_spi_flash.h"
#include <stdio.h>
void W25X128_Enable(void)
{
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,SPI1_CS_Pin,GPIO_PIN_RESET);
}

void W25X128_Disable(void)
{
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,SPI1_CS_Pin,GPIO_PIN_SET);
}



void BSP_W25Qx_Init(void)
{ 
	/* Reset W25Qxxx */
	BSP_W25Qx_Reset();
	
	W25X128_WaitForWriteEnd();
	
	W25X128_Read_ID();
}



static void	BSP_W25Qx_Reset(void)
{
	uint8_t cmd[2] = {RESET_ENABLE_CMD,RESET_MEMORY_CMD};
	
	W25X128_Enable();
	/* Send the reset command */
	HAL_SPI_Transmit(&hspi1, cmd, 2, W25Qx_TIMEOUT_VALUE);	
	W25X128_Disable();

}


/**
  * @brief  Reads current status of the W25Q128FV.
  * @retval W25Q128FV memory status
  */
static uint8_t BSP_W25Qx_GetStatus(void)
{
	uint8_t cmd1[] = {READ_STATUS_REG1_CMD};
	uint8_t cmd2[] = {READ_STATUS_REG2_CMD};
	uint8_t cmd3[] = {READ_STATUS_REG3_CMD};
	uint8_t status1;
	uint8_t status2;
	uint8_t status3;
	
	W25X128_Enable();
	/* Send the read status command */
	HAL_SPI_Transmit(&hspi1, cmd1, 1, W25Qx_TIMEOUT_VALUE);	
	/* Reception of the data */
	HAL_SPI_Receive(&hspi1,&status1, 1, W25Qx_TIMEOUT_VALUE);
	W25X128_Disable();
//	printf("Status Register 1=%02X\r\n",status1);
	
	W25X128_Enable();
	/* Send the read status command */
	HAL_SPI_Transmit(&hspi1, cmd2, 1, W25Qx_TIMEOUT_VALUE);	
	/* Reception of the data */
	HAL_SPI_Receive(&hspi1,&status2, 1, W25Qx_TIMEOUT_VALUE);
	W25X128_Disable();
//	printf("Status Register 2=%02X\r\n",status2);
	
	W25X128_Enable();
	/* Send the read status command */
	HAL_SPI_Transmit(&hspi1, cmd3, 1, W25Qx_TIMEOUT_VALUE);	
	/* Reception of the data */
	HAL_SPI_Receive(&hspi1,&status3, 1, W25Qx_TIMEOUT_VALUE);
	W25X128_Disable();
//	printf("Status Register 3=%02X\r\n",status3);
	
	
	/* Check the value of the register */
  if((status1 & W25Q128FV_FSR_BUSY) != 0)
  {
    return W25Qx_BUSY;
  }
	else
	{
		return W25Qx_OK;
	}		
}

void W25X128_WaitForWriteEnd(void)
{
	while(BSP_W25Qx_GetStatus()==W25Qx_BUSY)
	{}
}


uint8_t W25X128_Read_ID(void)
{
	uint8_t ID[4];
	uint8_t cmd[4]={READ_ID_CMD,0x00};
	uint8_t DeviceID=0;
	
	W25X128_Enable();
	HAL_SPI_Transmit(&hspi1,cmd,2,W25Qx_TIMEOUT_VALUE);
	HAL_SPI_Receive(&hspi1,ID,4,W25Qx_TIMEOUT_VALUE);
	W25X128_Disable();
	printf("SPI Flash Manufacturer ID=0x%x\r\n",ID[2]);
	DeviceID=ID[3];
	printf("SPI Flash Device ID=0x%x\r\n",ID[3]);

	return DeviceID;
}


void W25X128_WriteEnable(void)
{
	uint8_t cmd[1]={WRITE_ENABLE_CMD};
	
	W25X128_Enable();
	
	HAL_SPI_Transmit(&hspi1,cmd,1,W25Qx_TIMEOUT_VALUE);
	
	W25X128_Disable();
	
	W25X128_WaitForWriteEnd();

}

void W25X128_WriteDisable(void)
{
	uint8_t cmd[1]={WRITE_DISABLE_CMD};
	
	W25X128_Enable();
	
	HAL_SPI_Transmit(&hspi1,cmd,1,W25Qx_TIMEOUT_VALUE);
	
	W25X128_Disable();
	
	W25X128_WaitForWriteEnd();
}



void W25X128_BlockErase(uint8_t BlockNum)
{
	if(BlockNum>255)
		printf("BlockNum must be between 0 and 255. Please check it.");
	
	uint32_t flashAddr=0;
	
	flashAddr=BlockNum*(0x10000);
	
	W25X128_WriteEnable();
	
	uint8_t cmd[4]={0};
	
	cmd[0]=BLOCK_ERASE_CMD;
	cmd[1]=((flashAddr & 0xFF0000) >> 16);
	cmd[2]=((flashAddr & 0xFF00) >> 8);
	cmd[3]=(flashAddr & 0xFF);
	
	
	W25X128_Enable();
	HAL_SPI_Transmit(&hspi1,cmd,4,W25Qx_TIMEOUT_VALUE);
	W25X128_Disable();
	
	W25X128_WaitForWriteEnd();
	
}


void W25X128_SectorErase(uint32_t flashAddr)
{
	
	
	uint8_t cmd[4]={0};
	
	cmd[0]=SECTOR_ERASE_CMD;
	cmd[1]=((flashAddr & 0xFF0000) >> 16);
	cmd[2]=((flashAddr & 0xFF00) >> 8);
	cmd[3]=(flashAddr & 0xFF);
	
	W25X128_WriteEnable();
	W25X128_Enable();
	HAL_SPI_Transmit(&hspi1,cmd,4,W25Qx_TIMEOUT_VALUE);
	W25X128_Disable();
	
	W25X128_WaitForWriteEnd();
	
}


void W25X128_ChipErase(void)
{
	W25X128_WriteEnable();
	
	uint8_t cmd[4]={0};
	
	cmd[0]=CHIP_ERASE_CMD;
	
	W25X128_Enable();
	HAL_SPI_Transmit(&hspi1,cmd,1,W25Qx_TIMEOUT_VALUE);
	W25X128_Disable();
	W25X128_WaitForWriteEnd();
	printf("Chip Erase finish.\r\n");
}




void W25X128_PageWrite(uint8_t *pData, uint16_t length, uint32_t page_addr)
{
	if (length > 256) {
        printf("data length must be between 0 and 256. Please check it.");
        return;
    }
	uint8_t cmd[4]={0};
	
	cmd[0]=PAGE_PROG_CMD;
	cmd[1]=((page_addr & 0xFF0000) >> 16);
	cmd[2]=((page_addr & 0xFF00) >> 8);
	cmd[3]=(page_addr & 0xFF);	
		
	W25X128_WriteEnable();	
	W25X128_Enable();
	HAL_SPI_Transmit(&hspi1,cmd,4,W25Qx_TIMEOUT_VALUE);
	HAL_SPI_Transmit(&hspi1,pData,length,W25Qx_TIMEOUT_VALUE);	
		
	W25X128_Disable();	

	W25X128_WaitForWriteEnd();	
}


void W25X128_BufferWrite(uint8_t *pData, uint16_t data_length, uint32_t write_addr)
{
		uint8_t Num_of_Page = 0;
    uint8_t Num_of_Single = 0;
    uint8_t count = 0;      //lack count data, to align page address
    uint8_t Addr = 0;
    uint8_t temp = 0;
	
		Addr    = write_addr % W25Q128FV_PAGE_SIZE;
		count   = W25Q128FV_PAGE_SIZE - Addr;

		Num_of_Page = data_length / W25Q128FV_PAGE_SIZE;

    Num_of_Single = data_length % W25Q128FV_PAGE_SIZE;

	
		if (Addr == 0) {
        //data_length<Page Size
        if (Num_of_Page == 0)
            W25X128_PageWrite(pData, data_length, write_addr);
        else {      //data_length>Page Size
            while (Num_of_Page--) {
                W25X128_PageWrite(pData, W25Q128FV_PAGE_SIZE, write_addr);
                pData += W25Q128FV_PAGE_SIZE;
                write_addr += W25Q128FV_PAGE_SIZE;
            }
            W25X128_PageWrite(pData, Num_of_Single, write_addr);

        }

    }
		else {
        if (Num_of_Page == 0) {
            if (Num_of_Single > count) {
                temp = Num_of_Single - count;
                W25X128_PageWrite(pData, count, write_addr);
                pData += count;
                write_addr += count;

                W25X128_PageWrite(pData, temp, write_addr);
            } else {
                W25X128_PageWrite(pData, data_length, write_addr);
            }
        } else {
            data_length -= count;
            Num_of_Page = data_length / W25Q128FV_PAGE_SIZE;
            Num_of_Single = data_length % W25Q128FV_PAGE_SIZE;

            W25X128_PageWrite(pData, count, write_addr);
            write_addr += count;
            pData += count;

            while (Num_of_Page--) {
                W25X128_PageWrite(pData, W25Q128FV_PAGE_SIZE, write_addr);
                write_addr += W25Q128FV_PAGE_SIZE;
                pData += W25Q128FV_PAGE_SIZE;
            }

            if (Num_of_Single != 0) {
                W25X128_PageWrite(pData, Num_of_Single, write_addr);
            }

        }

    }


	

}



void W25X128_BufferRead(uint8_t *pData, uint16_t length, uint32_t read_addr)
{
    uint8_t cmd[4];

    cmd[0] = READ_CMD;
    cmd[1] = ((read_addr & 0xFF0000) >> 16);
    cmd[2] = ((read_addr & 0xFF00) >> 8);
    cmd[3] = (read_addr & 0xFF);
		
		W25X128_WriteEnable();
		W25X128_Enable();
		HAL_SPI_Transmit(&hspi1,cmd,4,W25Qx_TIMEOUT_VALUE);
		HAL_SPI_Receive(&hspi1,pData,length,W25Qx_TIMEOUT_VALUE);
		W25X128_Disable();

    W25X128_WaitForWriteEnd();
}










void W25X128_UnlockAllBlock(void)
{
	uint8_t cmd[2];
	
	cmd[0]=WRITE_STATUS_REG1_CMD;
	cmd[1]=0x00;

	W25X128_WriteEnable();
	W25X128_Enable();
	/* Send the read ID command */
	HAL_SPI_Transmit(&hspi1, cmd, 2, W25Qx_TIMEOUT_VALUE);	
	W25X128_Disable();
	W25X128_WaitForWriteEnd();
	
	cmd[0]=WRITE_STATUS_REG2_CMD;
	cmd[1]=0x00;
	
	W25X128_WriteEnable();
	W25X128_Enable();
	/* Send the read ID command */
	HAL_SPI_Transmit(&hspi1, cmd, 2, W25Qx_TIMEOUT_VALUE);	
	W25X128_Disable();
	W25X128_WaitForWriteEnd();
	
	cmd[0]=WRITE_STATUS_REG3_CMD;
	cmd[1]=0x00;
	
	W25X128_WriteEnable();
	W25X128_Enable();
	/* Send the read ID command */
	HAL_SPI_Transmit(&hspi1, cmd, 2, W25Qx_TIMEOUT_VALUE);	
	W25X128_Disable();
	W25X128_WaitForWriteEnd();
	
	
	
}
