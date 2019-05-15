#include "bsp_uart.h"
#include <string.h>

#ifndef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
//�����ض���printf
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
    return ch;
}




uint16_t USART_RX_Length = 0;   //���ڽ��յĳ���
uint8_t USART_RXBuffer[256];    //���ڽ��յ����ݱ�������
uint8_t USART_RXSingleBuffer;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);
    /* NOTE: This function Should not be modified, when the callback is needed,
             the HAL_UART_TxCpltCallback could be implemented in the user file
     */

    if (huart->Instance == huart1.Instance)
    {
        if (USART_RX_Length >= 255) //����ж�
        {
            USART_RX_Length = 0;
            memset(USART_RXBuffer, 0x00, sizeof(USART_RXBuffer));
            //printf("�������(����256)\r\n");
        }
        else
        {
            if (USART_RX_Length == 0)
            {
                memset(USART_RXBuffer, 0, 256);
            }

            USART_RXBuffer[USART_RX_Length++] = USART_RXSingleBuffer;   //��������ת��

            if ((USART_RXBuffer[USART_RX_Length - 1] == 0x0A)
                    && (USART_RXBuffer[USART_RX_Length - 2] == 0x0D)) //�жϽ���λ
            {
                USART_RX_Length = 0;
                printf("receive: %s", USART_RXBuffer);
            }
        }
        HAL_UART_Receive_IT(&huart1, (uint8_t *)&USART_RXSingleBuffer, 1); //�ٿ��������ж�

    }

}

void start_uart_receive(void)
{
	HAL_UART_Receive_IT(&huart1, &USART_RXSingleBuffer, 1);
}