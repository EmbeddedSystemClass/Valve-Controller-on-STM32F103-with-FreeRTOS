/**
  ******************************************************************************
  * @file    Project/User/main.c 
  * @author  NTT - BKSTEK Team
  * @version V1.0
  * @date    15-11-2019
  * @brief   Main program body
	*/
	

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"
#include "stm32f10x_rtc.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#include "eeprom.h"
#include "hw_config.h"
#include "valve.h"
#include "buttons.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
enum
{
  VALVE_START=0,
  VALVE_ZW_CONNECT,
  VALVE_ON,
  VALVE_OFF,
  VALVE_FACTORYRESET,
  VALVE_IDE,
}VALVE_MODE;
/* Private define ------------------------------------------------------------*/
#define xBTN1 1
#define xBTN2 2
#define xBTN3 3
/*--------------- Tasks Priority -------------*/     

/* Private macro -------------------------------------------------------------*/
#define uxPriority 1
/* Private variables ---------------------------------------------------------*/

char data;
  
BOOL ZW_ready =FALSE;
uint8_t Valve_mode = VALVE_IDE;
extern T_CON_TYPE cmd_ready;
extern T_CON_TYPE cmd_ready;
extern BYTE serBuf[SERBUF_MAX];

/* Declare a variable of type xQueueHandle.  This is used to store the queue
that is accessed by all three tasks. */

xQueueHandle xQueue;
xSemaphoreHandle serialPortMutex;
xSemaphoreHandle zwPortMutex;

//motor_t MOTOR_INSTANT[MAX_MOTOR]; 
/* Private function prototypes -----------------------------------------------*/


// FreeRTOS tasks
static void vTaskLED(void *pvParameters);

static void vTaskUART(void *pvParameters);
static void vTaskZWAVE(void *pvParameters);

/* Main function -------------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void) {
    
		SystemInit();
    /* Setup STM32F103 clock, PLL and Flash configuration) */
		
		
    /* Initialize STM32 peripherals (clocks, GPIO, NVIC) resources */
		STM32F1_HW_Init();
    printf("\r\n**STM32F103 HW init successful** \r\n")	;	
		Motor_Init();
		EEPROM_Init();
		printf("\r\n**EEPROM init successful** \r\n")	;	
    /* Free RTOS *//////
//			GPIO_ResetBits(ZM_UART_TxGPIO,ZM_UART_TxPin);
//			while (1)
//			{
//				for (int i = 0;i <1000000;i++);
//				  //USART_SendData(DBG_UART, 'q');
//				USART_SendData(ZM_UART, 'q');
//				
//			}
		serialPortMutex = xSemaphoreCreateMutex();
		zwPortMutex		= xSemaphoreCreateMutex();
//    xTaskCreate(vMainTestTask, "TEST", configMINIMAL_STACK_SIZE*2, NULL, mainLED_TASK_PRIORITY + 1, NULL);

		xQueue = xQueueCreate( 3, sizeof( Data_t ) );
    xTaskCreate(vTaskLED,"Task LED",configMINIMAL_STACK_SIZE,NULL,uxPriority,NULL);
		//xTaskCreate(vTaskUART,"Task UART", configMINIMAL_STACK_SIZE,NULL,1,NULL);
		xTaskCreate(vTaskZWAVE,"Task ZWAVE", configMINIMAL_STACK_SIZE,NULL,uxPriority,NULL);
    xTaskCreate(vTaskButton2,"Task BTN2", configMINIMAL_STACK_SIZE,NULL,uxPriority,NULL);
    xTaskCreate(vTaskButton3,"Task BTN3", configMINIMAL_STACK_SIZE,NULL,uxPriority,NULL);
		xTaskCreate(vTaskButton1,"Task BTN1", configMINIMAL_STACK_SIZE,NULL,uxPriority,NULL);
	//	xTaskCreate(vTaskControlMotor,"Task Control Motor", configMINIMAL_STACK_SIZE, NULL, 1, 	NULL);
    /* Start scheduler */
		vTaskStartScheduler();
    /* We should never get here as control is now taken by the scheduler */
	
    for ( ; ; );
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  FreeRTOS RGB LED Task
  * @param  pvParameters not used
  * @retval None
  */

static void vTaskLED(void *pvParameters)
{
	while(1)
	{
		GPIO_SetBits(LED1_GPIO_Port,LED1_Pin);
		vTaskDelay(1000);
		GPIO_ResetBits(LED1_GPIO_Port,LED1_Pin);
		vTaskDelay(1000);
//GPIO_SetBits(LED1_GPIO_Port,LED2_Pin);
//	vTaskDelay(1000);
//	GPIO_ResetBits(LED1_GPIO_Port,LED2_Pin);
//		vTaskDelay(1000);
	}
}

/**
  * @brief  FreeRTOS UART Test Task
  * @param  pvParameters not used
  * @retval None
  */
static void vTaskUART(void *pvParameters)
{
	uint32_t THH = 0, TMM = 0, TSS = 0;
		char data= 0xff;
	uint32_t TimeVar;
	while (1)
	{
		//EEPROM_Write(0x22,'a');
		//data = EEPROM_Read(0x22);
		//USART_SendData(DBG_UART,data);
		
		xSemaphoreTake( serialPortMutex, portMAX_DELAY);

		//printf("DBG_UART periodic task \r\n");
		TimeVar= RTC_GetCounter();
		  /* Compute  hours */
  THH = TimeVar / 3600;
  /* Compute minutes */
  TMM = (TimeVar % 3600) / 60;
  /* Compute seconds */
  TSS = (TimeVar % 3600) % 60;

		printf("Time: %0.2d:%0.2d:%0.2d\r", THH, TMM, TSS);
		xSemaphoreGive(serialPortMutex);
		vTaskDelay(5000);
		
	}
}
/**
  * @brief  FreeRTOS Zwave protocol communication Task
  * @param  pvParameters not used
  * @retval None
  */
static void vTaskZWAVE(void *pvParameters)
{
	ZW_UART_COMMAND uart_cmd;
	
			
	while (1)
	{
			uart_cmd.zw_uartcommandset.length = 4;
			uart_cmd.zw_uartcommandset.cmd = COMMAND_ZW_CONNECT;
     		 uart_cmd.zw_uartcommandset.type = ZW_SET_CONNECT;
			uart_cmd.zw_uartcommandset.value1 =ZW_CONNECT;
  			if (Uart_send_command(uart_cmd)==0)
			{
					//printf("\r\n Failed to send command \r\n")	;	
				GPIO_SetBits(LED1_GPIO_Port,LED2_Pin);
				
			}		
			else
			{
				GPIO_ResetBits(LED1_GPIO_Port,LED2_Pin);
					//printf("\r\n Send command OK\r\n");
			}
		
			vTaskDelay(1000);
	}
}
/**
  * @brief  FreeRTOS Zwave protocol communication Task
  * @param  pvParameters not used
  * @retval None
  */
static void vTaskPeriodic(void *pvParameters)
{
	ZW_UART_COMMAND uart_cmd;
	
	while(1)
	{
		vTaskPrioritySet(vTaskPeriodic,uxPriority+1 );
		xSemaphoreTake(zwPortMutex, portMAX_DELAY);
		uart_cmd.zw_uartcommandset.length = 3;
		uart_cmd.zw_uartcommandset.cmd = COMMAND_STATUS ;
      	uart_cmd.zw_uartcommandset.type = ZW_GET_STATUS ;
		  if (Uart_send_command(uart_cmd)==0)
			{
				xSemaphoreTake(serialPortMutex,portMAX_DELAY);
						printf("\r\n Send command OK\r\n");
				xSemaphoreGive(serialPortMutex);
				
			}		
			else
			{
				xSemaphoreTake(serialPortMutex,portMAX_DELAY);
					printf("\r\n Failed to send command\r\n");
				xSemaphoreGive(serialPortMutex);
			}
//			if(cmd_ready==conFrameReceived)
//			{
//				memcpy(&uart_cmd.zw_uartcommand.length,serBuf,sizeof(uart_cmd));
//				cmd_ready = conIdle;
				
			
			
			xSemaphoreTake(zwPortMutex, portMAX_DELAY);
		// uart_cmd.zw_uartcommandset.value1 =ZW_CONNECT;
			vTaskPrioritySet(vTaskPeriodic,uxPriority );
			
			vTaskDelay(5000);
	}
}

void vApplicationIdleHook( void ) {
}

void vApplicationTickHook( void ) {

}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  printf("Wrong parameters value: file %s on line %d\r\n", file, line);
  /* Infinite loop */
  while (1)
  {}
}
#endif

void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	printf("FreeRTOS freeHeapsize = %d\n",xPortGetFreeHeapSize());
	for( ;; );
}

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(DBG_UART, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(DBG_UART, USART_FLAG_TC) == RESET)
  {}

  return ch;
}
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
