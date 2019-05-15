#include "bsp_lcd.h"
#include "stdio.h"


/****************************************************************************
* 名    称: void LCD_WriteReg(u16 LCD_Reg, u16 LCD_Value)
* 功    能：LCD写寄存器
* 入口参数：LCD_Reg: 寄存器地址
*           LCD_RegValue: 要写入的数据
* 返回参数：无
* 说    明：       
****************************************************************************/				   
void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_Value)
{	
	LCD_CMD = LCD_Reg;		 //写入要写的寄存器序号	 
	LCD_DATA = LCD_Value;  //向寄存器写入的数据	    		 
}


/****************************************************************************
* 名    称: u16 LCD_ReadReg(u16 LCD_Reg)
* 功    能：LCD读寄存器
* 入口参数：LCD_Reg:寄存器地址
* 返回参数：读到该寄存器序号里的值
* 说    明：       
****************************************************************************/	
uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{										   
	LCD_CMD=LCD_Reg;		//写入要读的寄存器序号
	HAL_Delay(1);		  
	return LCD_DATA;		//返回读到的值
}   




uint16_t lcd_id;
void bsp_lcd_init(void)
{
	MX_FSMC_Init();
	LCD_WriteReg(0x0000,0x0001);
	HAL_Delay(50);
	lcd_id = LCD_ReadReg(0x0000);    //读取LCD ID
	printf("lcd id=0x%x\r\n",lcd_id);
	
	if(lcd_id==0x9325)
	{
		printf("write lcd init data\r\n");
		LCD_WriteReg(0x00E5,0x78F0); 
		LCD_WriteReg(0x0001,0x0100); 
		LCD_WriteReg(0x0002,0x0700); 
		LCD_WriteReg(0x0003,0x1030); 
		LCD_WriteReg(0x0004,0x0000); 
		LCD_WriteReg(0x0008,0x0202);  
		LCD_WriteReg(0x0009,0x0000);
		LCD_WriteReg(0x000A,0x0000); 
		LCD_WriteReg(0x000C,0x0000); 
		LCD_WriteReg(0x000D,0x0000);
		LCD_WriteReg(0x000F,0x0000);
    //power on sequence VGHVGL
		LCD_WriteReg(0x0010,0x0000);   
		LCD_WriteReg(0x0011,0x0007);  
		LCD_WriteReg(0x0012,0x0000);  
		LCD_WriteReg(0x0013,0x0000); 
		LCD_WriteReg(0x0007,0x0000); 
		//vgh 
		LCD_WriteReg(0x0010,0x1690);   
		LCD_WriteReg(0x0011,0x0227);
		//delayms(100);
		//vregiout 
		LCD_WriteReg(0x0012,0x009D); //0x001b
		//delayms(100); 
		//vom amplitude
		LCD_WriteReg(0x0013,0x1900);
		//delayms(100); 
		//vom H
		LCD_WriteReg(0x0029,0x0025); 
		LCD_WriteReg(0x002B,0x000D); 
		//gamma
		LCD_WriteReg(0x0030,0x0007);
		LCD_WriteReg(0x0031,0x0303);
		LCD_WriteReg(0x0032,0x0003);// 0006
		LCD_WriteReg(0x0035,0x0206);
		LCD_WriteReg(0x0036,0x0008);
		LCD_WriteReg(0x0037,0x0406); 
		LCD_WriteReg(0x0038,0x0304);//0200
		LCD_WriteReg(0x0039,0x0007); 
		LCD_WriteReg(0x003C,0x0602);// 0504
		LCD_WriteReg(0x003D,0x0008); 
		//ram
		LCD_WriteReg(0x0050,0x0000); 
		LCD_WriteReg(0x0051,0x00EF);
		LCD_WriteReg(0x0052,0x0000); 
		LCD_WriteReg(0x0053,0x013F);  
		LCD_WriteReg(0x0060,0xA700); 
		LCD_WriteReg(0x0061,0x0001); 
		LCD_WriteReg(0x006A,0x0000); 
		//
		LCD_WriteReg(0x0080,0x0000); 
		LCD_WriteReg(0x0081,0x0000); 
		LCD_WriteReg(0x0082,0x0000); 
		LCD_WriteReg(0x0083,0x0000); 
		LCD_WriteReg(0x0084,0x0000); 
		LCD_WriteReg(0x0085,0x0000); 
		//
		LCD_WriteReg(0x0090,0x0010); 
		LCD_WriteReg(0x0092,0x0600); 
		
		LCD_WriteReg(0x0007,0x0133);
		LCD_WriteReg(0x00,0x0022);//
	}
	else
	{
		printf("lcd id !=0x9325!!!\r\n");
	}
	
	
	
	
	
}