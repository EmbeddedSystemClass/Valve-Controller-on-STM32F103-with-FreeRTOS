#include "hw_config.h"
#include "stm32f10x_usart.h"



extern void STM32F1_HW_Init(void)
{
	UARTs_Init();
	
}



static void UARTs_Init(void) {
USART_InitTypeDef USART_InitStructure;
  /* 
		**ZWAVE UART CONFIG USING UART5**
	 USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
				- Uart Recive interrupt enabled
    */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(ZM_UART, &USART_InitStructure);
	  /* Enable ZM_UART Receive interrupts */
		USART_ITConfig(ZM_UART, USART_IT_RXNE, ENABLE);
		/* Enable ZM_UART Receive interrupts */
		USART_Cmd(ZM_UART, ENABLE);
		
}