/**
  ******************************************************************************
  * @file    Project/User/zwave
  * @author  NTT - BKSTEK Team
  * @version V1.0
  * @date    15-11-2019
  * @brief   Zwave functions description
	*/
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
#include "zwave.h"
#include "buttons.h"
#include "uart_command.h"
#include "Valve.h"
#include "main.h"

/* Private variables ---------------------------------------------------------*/
zwave_mode_t Zwave_mode = ZWAVE_IDLE;
extern T_CON_TYPE cmd_ready;
extern BYTE serBuf[SERBUF_MAX];

/**
  * @brief  FreeRTOS Zwave protocol communication Task
  * @param  pvParameters not used
  * @retval None
  */
void vTaskZmReceiver(void *pvParameters)
{
		  ValveHandles_t *pxValveHandles = (ValveHandles_t*) pvParameters;

	const portTickType xTicksToWait = 100 / portTICK_RATE_MS;
	portBASE_TYPE xStatus;
	Data_motor_t data_control;
	ZW_UART_COMMAND uart_cmd;
	BOOL ZW_ready =FALSE;
  
  BOOL ZW_appversion_send = FALSE;
  BOOL ZW_getconnect_send = FALSE;
	uint8_t tryCounter = 0;
	uint8_t  data_btn;
	while (1)
	{
		switch(Zwave_mode)
		{
			case ZWAVE_START:
			{
				
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
										Zwave_mode = ZWAVE_IDLE ;
							}		
					else
							{
								printf("\r\n ZWave communication OK: Sent EXCLUSION \r\n");
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
											Zwave_mode = ZWAVE_IDLE ;
							}		
					else
							{
									printf("\r\n Connect ZWave communication OK \r\n");
									Zwave_mode = ZWAVE_IDLE;
							}
							break;
			}
			case ZWAVE_IDLE:
			{
				/*Statement for sending message*/
				//printf("Starting or Handling Zwave device after RESET!");
				xStatus = xQueueReceive(pxValveHandles->xQueue,&data_btn,NULL );
				if (xStatus == pdPASS)
				{
					if (data_btn == xBTN2)
					{
						Zwave_mode = ZWAVE_CONNECT;
					}
					if (data_btn == xBTN3)
					{
						Zwave_mode = ZWAVE_EXC;

					}
					
					break;
					
				}	
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
									printf(" Zw reponse connected");
                  //ZW is ready  
                }
                else if (uart_cmd.zw_uartcommandreport.value1 == ZW_DISCONNECT)
                {
									printf(" Zw reponse not connected");

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
																		printf(" Zw reponse connected");

                  //ZW is connected
                }
                else if( uart_cmd.zw_uartcommandget.value == ZW_DISCONNECT)
                {
																		printf(" Zw reponse not connected");

                  //ZW is not connected
                }
              }

              break;
            }
            case COMMAND_SWITCHBINARY:
            {
							if (uart_cmd.zw_uartcommandset.type ==ZW_SWITCHBINARY_SET )
							{
								// send data to queue for motor task 
								data_control.motor_num 	=uart_cmd.zw_uartcommandset.value1;
								data_control.state 			= uart_cmd.zw_uartcommandset.value2;
								//uint8_t data;
								xStatus = xQueueSend( pxValveHandles->xQueueControl, &data_control, xTicksToWait );
                if( xStatus != pdPASS )
                {
                    printf("Could not send to the queue.\n" );
                }
								
							}
							

              break;
            }
						case COMMAND_ZWAVE_RESET:
            { 
							printf("Zwave reset.\n" );

              Zwave_mode = ZWAVE_IDLE;
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
   

      vTaskDelay(5000);
    }
    
}







