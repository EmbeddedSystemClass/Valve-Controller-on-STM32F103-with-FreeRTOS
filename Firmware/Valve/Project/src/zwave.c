
/*Standard library include*/
#include <stdio.h>
#include <string.h>

/*FreeROTS library include*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

/*STM32 library include*/
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"
#include "stm32f10x_rtc.h"

/*Valve Controller libary include*/
#include "zware.h"
#include "buttons.h"
#include "uart_command.h"

/* Private variables ---------------------------------------------------------*/
zwave_mode_t Zwave_mode = ZWAVE_IDLE;
extern xQueueHandle xQueue;
extern T_CON_TYPE cmd_ready;
extern BYTE serBuf[SERBUF_MAX];

/**
  * @brief  FreeRTOS Zwave protocol communication Task
  * @param  pvParameters not used
  * @retval None
  */
void vTaskZmReceiver(void *pvParameters)
{
	ZW_UART_COMMAND uart_cmd;
	BOOL ZW_ready =FALSE;
  
  BOOL ZW_appversion_send = FALSE;
  BOOL ZW_getconnect_send = FALSE;
	uint8_t tryCounter = 0;
	while (1)
	{
		switch(Zwave_mode)
		{
			case ZWAVE_START:
			{
				printf("Starting or Handling Zwave device");

				xSemaphoreTake(xQueue,portMAX_DELAY);
        break;
			}
      case ZWAVE_EXC:
      {
        uart_cmd.zw_uartcommandset.length = 4;
        uart_cmd.zw_uartcommandset.cmd = COMMAND_ZW_CONNECT;
        uart_cmd.zw_uartcommandset.type = ZW_SET_CONNECT;
        uart_cmd.zw_uartcommandset.value1 = ZW_DISCONNECT;
        if (Uart_send_command(uart_cmd)==0)
							{
									printf("\r\n Failed to send data to ZW \r\n");
									vTaskDelay(500);
								/*
								Reset Zwave device when cannot connect after 10 times trying to connect
								*/
									tryCounter++;
									if(tryCounter == 10) 
										Zwave_mode = ZWAVE_START ;
							}		
					else
							{
									printf("\r\n ZWave communication OK \r\n");
									Zwave_mode = ZWAVE_IDLE;
							}
        break;
      }
			case ZWAVE_CONNECT:
			{
					uart_cmd.zw_uartcommandset.length = 4;
					uart_cmd.zw_uartcommandset.cmd = COMMAND_ZW_CONNECT;
					uart_cmd.zw_uartcommandset.type = ZW_SET_CONNECT;
					uart_cmd.zw_uartcommandset.value1 =ZW_CONNECT;
					if (Uart_send_command(uart_cmd)==0)
							{
									printf("\r\n Failed to send data to ZW \r\n");
									vTaskDelay(500);
								/*
								Reset Zwave device when cannot connect after 10 times trying to connect
								*/
									tryCounter++;
									if(tryCounter == 10) 
											Zwave_mode = ZWAVE_START ;
							}		
					else
							{
									printf("\r\n ZWave communication OK \r\n");
									Zwave_mode = ZWAVE_IDLE;
							}
			}
			case ZWAVE_IDLE:
			{
				/*Statement for sending message*/
        

				/*Statement for receiving message*/
				if (cmd_ready == conFrameReceived)
				{	
					memcpy(&uart_cmd.zw_uartcommand.length,serBuf,sizeof(uart_cmd));
					cmd_ready = conIdle;
					switch (uart_cmd.zw_uartcommand.cmd)
					{
            case COMMAND_STATUS:
            {
              if (uart_cmd.zw_uartcommand.type == ZW_REPORT_STATUS )
              {
                if (uart_cmd.zw_uartcommandreport.value1 == ZW_CONNECT)
                {
                  //ZW is ready  
                }
                else if (uart_cmd.zw_uartcommandreport.value1 == ZW_DISCONNECT)
                {
                  //ZW not ready
                }                
              }
              break;
            }
            case COMMAND_ZW_CONNECT:
            {
              if(uart_cmd.zw_uartcommand.type ==ZW_REPORT_STATUS)
              {
                if(uart_cmd.zw_uartcommandget.value == ZW_CONNECT)
                {
                  //ZW is connected
                }
                else if( uart_cmd.zw_uartcommandget.value == ZW_DISCONNECT)
                {
                  //ZW is not connected
                }
              }

              break;
            }
            case COMMAND_SWITCHBINARY:
            {

              break;
            }
						case COMMAND_ZWAVE_RESET:
            { 
              Zwave_mode = ZWAVE_START;
              break;
            }
            
					}
				}
			}
     
				
			
				
		}
  }
}
/**
  * @brief  FreeRTOS Zwave Periodic Checking Task
  * @param  pvParameters not used
  * @retval None
  * 
  */
void vTaskZmPeriodic(void *pvParameters)
{
    ZW_UART_COMMAND uart_cmd;
    while (TRUE)
    {
      6
      3.+
      3

      vTaskdelay(5000);
    }
    
}







