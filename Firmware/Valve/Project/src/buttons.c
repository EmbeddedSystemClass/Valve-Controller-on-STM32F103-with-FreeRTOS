/**
  ******************************************************************************
  * @file    Project/User/button
  * @author  NTT - BKSTEK Team
  * @version V1.0
  * @date    15-11-2019
  * @brief   Button functions
	*/
	
#include "buttons.h"
#include "hw_config.h"
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "queue.h"
#include "task.h"
#include "main.h"
#include "semphr.h"
#include "Valve.h"
//extern xQueueHandle xQueue;
//extern xQueueHandle xQueueControl;
//extern xSemaphoreHandle serialPortMutex;
sw_t switchState(int SWnumber)
{
	
    switch    (SWnumber)
        {
            case 0:
                if (GPIO_ReadInputDataBit(BTN_1_GPIO_Port,BTN_1_Pin)==0) return PRESSED;
                else return RELEASED;

            case 1:
               if (GPIO_ReadInputDataBit(BTN_2_GPIO_Port,BTN_2_Pin)==0) return PRESSED;
                else return RELEASED;
						case 2:
						if (GPIO_ReadInputDataBit(BTN_3_GPIO_Port,BTN_3_Pin)==0) return PRESSED;
                else return RELEASED;

        }
}

void vTaskButton1(void *pvParameters)
{
	ValveHandles_t *pxValveHandles = (ValveHandles_t*) pvParameters;
	vTaskDelay(10);
    portBASE_TYPE xStatus;
   const portTickType xTicksToWait = 100 / portTICK_RATE_MS;
   sw_t    preVal[3], currentVal[3];
   Data_t  data[3];
	

   data[0].taskSource = xBTN1;
   data[1].taskSource = xBTN2;
   data[2].taskSource = xBTN3;
   preVal[0] = switchState(0);
   preVal[1] = switchState(1);
   preVal[2] = switchState(2);
   for( ;; )
   {
       for (uint8_t btn_num =0 ; btn_num<3 ; btn_num++)
       {
            currentVal[btn_num] = switchState(btn_num);		
        if (currentVal[btn_num] != preVal[btn_num])
            {
					preVal[btn_num] = currentVal[btn_num];
						if (currentVal[btn_num] == PRESSED)
								{
									data[btn_num].buttonValue = currentVal[btn_num];
									xStatus = xQueueSend( pxValveHandles->xQueue, &data[btn_num], xTicksToWait );
									if (xStatus == pdPASS)
									{
										printf("BTN send ok");
									}
									else {
										printf("BTN sent not ok");
									}
								}
								else
								{
	
								}
            }
	    }
  }
     
   
}

//void vTaskButton2(void *pvParameters)
//{
//	ValveHandles_t *pxValveHandles = (ValveHandles_t*) pvParameters;
//portBASE_TYPE xStatus;
//    const portTickType xTicksToWait = 100 / portTICK_RATE_MS;
//    sw_t    preVal, currentVal;
//    Data_t  data;
//	Data_motor data_control;
//    data.taskSource = xBTN2;
//    preVal = switchState(2);
//    for( ;; )
//    {
//			vTaskDelay(10);
//       currentVal = switchState(2);		
//        if (currentVal != preVal)
//        {
//					preVal = currentVal;
//						if (currentVal == PRESSED)
//								{
//									//data.buttonValue = currentVal;
//									//xStatus = xQueueSend( xQueueControl, &data, xTicksToWait );
//								}
//								else
//								{
//	
//								}
//        
//        }
//			}
//}
//			
//    


//void vTaskButton3(void *pvParameters)
//{
//	ValveHandles_t *pxValveHandles = (ValveHandles_t*) pvParameters;
//portBASE_TYPE xStatus;
//    const portTickType xTicksToWait = 1000 / portTICK_RATE_MS;
//    sw_t    preVal, currentVal;
//    Data_t  data;
//		Data_control_t data_control;
//		uint8_t test=1;
//		data.taskSource = xBTN3;
//    preVal = switchState(3);
//    for( ;; )
//    {
//			vTaskDelay(10);
//         currentVal = switchState(3);		
//        if (currentVal != preVal)
//        {
//					preVal = currentVal;
//						if (currentVal == PRESSED)
//								{
//									data_control.motor_num = 1;
//									data_control.state  	 = 1;
//									printf("FreeRTOS freeHeapsize = %d\n",xPortGetFreeHeapSize());
//									xStatus = xQueueSend( pxValveHandles->xQueueControl, &test, xTicksToWait );	
//									 if( xStatus != pdPASS )
//                {
//                    printf("Could not send to the queue.\n" );
//                }
//								else {
//									printf("data sent the queue.\n" );
//								}
//								}
//								else
//								{
//									data_control.motor_num = 1;
//									data_control.state  	 = 0;
//									xStatus = xQueueSend( pxValveHandles->xQueueControl, &data_control, xTicksToWait );	
//									 if( xStatus != pdPASS )
//                {
//                    printf("Could not send to the queue.\n" );
//                }
//								else {printf("data sent the queue.\n" );}
//								}
//        
//        }
//        
//    }
//}