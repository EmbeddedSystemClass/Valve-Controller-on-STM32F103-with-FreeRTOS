#ifndef HW_CONFIG__H_
#define HW_CONFIG__H_


/****************************************************************************/
/*                      PUBLIC TYPES and DEFINITIONS                       */
/****************************************************************************/


/****************************************************************************/
/*                      PRIVATE TYPES and DEFINITIONS                       */
/****************************************************************************/
	// Define ZWAVE communication
	
	#define ZM_UART                   UART5
  #define ZM_UART_GPIO              GPIOA
  #define ZM_UART_CLK               RCC_APB2Periph_USART5
  #define ZM_UART_GPIO_CLK          RCC_APB2Periph_GPIOA
  #define ZM_UART_RxPin             GPIO_Pin_2
  #define ZM_UART_TxPin             GPIO_Pin_12
  #define ZM_UARTy_IRQn             UART5_IRQn
  #define ZM_UART_IRQHandler        UART5_IRQHandler

/****************************************************************************/
/*                      PUBLIC FUNCTION PROTOTYPES                    */
/****************************************************************************/
extern void STM32F1_HW_Init(void);

/****************************************************************************/
/*                      PRIVATE FUNCTION PROTOTYPE   S                    */
/****************************************************************************/
static void UARTs_Init(void) ;

#endif