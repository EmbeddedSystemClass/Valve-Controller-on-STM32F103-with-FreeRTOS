#include "buttons.h"
#include "hw_config.h"
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "queue.h"
#include "task.h"
#include "main.h"
#include "semphr.h"

extern xQueueHandle xQueue;
extern xSemaphoreHandle serialPortMutex;
sw_t switchState(int SWnumber)
{
    switch    (SWnumber)
        {
            case 1:
                if (GPIO_ReadInputDataBit(BTN_1_GPIO_Port,BTN_1_Pin)==0) return PRESSED;
                else return RELEASED;

            case 2:
               if (GPIO_ReadInputDataBit(BTN_2_GPIO_Port,BTN_2_Pin)==0) return PRESSED;
                else return RELEASED;
						case 3:
						if (GPIO_ReadInputDataBit(BTN_3_GPIO_Port,BTN_3_Pin)==0) return PRESSED;
                else return RELEASED;

        }
}

void vTaskButton1(void *pvParameters)
{
	vTaskDelay(10);
portBASE_TYPE xStatus;
     const portTickType xTicksToWait = 100 / portTICK_RATE_MS;
    sw_t    preVal, currentVal;
    Data_t  data;

    data.taskSource = xBTN2;
    preVal = switchState(1);
    for( ;; )
    {
       currentVal = switchState(1);		
        if (currentVal != preVal)
        {
                data.buttonValue = currentVal;
                preVal = currentVal;
								xStatus = xQueueSend( xQueue, &data, xTicksToWait );
                  xSemaphoreTake( serialPortMutex, portMAX_DELAY);
					
									printf("BTN2 Sending \n");
									xSemaphoreGive( serialPortMutex);	
                if( xStatus != pdPASS )
                {xSemaphoreTake( serialPortMutex, portMAX_DELAY);
                    printf( "Task 1 could not send to the queue.\n" );
									xSemaphoreGive( serialPortMutex);	
                }
								else{
									xSemaphoreTake( serialPortMutex, portMAX_DELAY);
									 printf( "Task 1 sent to the queue.\n" );
										xSemaphoreGive( serialPortMutex);	
								}
        }
			}
    
}

void vTaskButton2(void *pvParameters)
{
portBASE_TYPE xStatus;
    const portTickType xTicksToWait = 100 / portTICK_RATE_MS;
    sw_t    preVal, currentVal;
    Data_t  data;

    data.taskSource = xBTN2;
    preVal = switchState(2);
    for( ;; )
    {
       currentVal = switchState(2);		
        if (currentVal != preVal)
        {
                data.buttonValue = currentVal;
                preVal = currentVal;
								xStatus = xQueueSend( xQueue, &data, xTicksToWait );
          									xSemaphoreTake( serialPortMutex, portMAX_DELAY);
         
					printf("BTN2 Sending \n");
						xSemaphoreGive( serialPortMutex);	
                if( xStatus != pdPASS )
                {
																		xSemaphoreTake( serialPortMutex, portMAX_DELAY);

                    printf( "Task 2 could not send to the queue.\n" );
										xSemaphoreGive( serialPortMutex);	
                }
								else{
																		xSemaphoreTake( serialPortMutex, portMAX_DELAY);

									 printf( "Task 2 sent to the queue.\n" );
										xSemaphoreGive( serialPortMutex);	
								}
        }
			}
    
}

void vTaskButton3(void *pvParameters)
{
portBASE_TYPE xStatus;
    const portTickType xTicksToWait = 100 / portTICK_RATE_MS;
    sw_t    preVal, currentVal;
    Data_t  data;

    data.taskSource = xBTN3;
    preVal = switchState(3);
    for( ;; )
    {
        currentVal = switchState(3);		
        if (currentVal != preVal)
        {
                data.buttonValue = currentVal;
                preVal = currentVal;
								xStatus = xQueueSend( xQueue, &data, xTicksToWait );
                   printf("BTN3 Sending \n");
                if( xStatus != pdPASS )
                {
                    printf( "Task 3 could not send to the queue.\n" );
                }
								else{
									 printf( "Task 3 sent to the queue.\n" );
								}
        }
    }
}