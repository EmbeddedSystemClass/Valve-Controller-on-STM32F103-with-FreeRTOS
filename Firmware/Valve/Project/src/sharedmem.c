/* Built-in C library includes ----------------------*/

#include <stdio.h>
#include <stdint.h>

/* Platform includes --------------------------------*/
#include "main.h"
#include <sharedmem.h>
#include "Valve.h"
/* Private typedef -----------------------------------*/

//typedef struct {
//	uint8_t uiFunctionAdr;
//	uint8_t uiMem;
//} xMemHandler_t;

typedef uint8_t xMemHandler_t;

/* Private define -----------------------------------*/
#define MEM_MAX_SIZE (40)
#define MEM_RST_VAL

static xMemHandler_t xMemSharedHandler[MEM_MAX_SIZE];
static uint8_t xMemShareReg;

/* Start Implementation -----------------------------*/

/**
 * Brief: Get data
 * uiFuncAdr:
 * retVal: Value of Mem at uiFuncAdr
 *
 */
uint8_t uiMemGet(uint8_t uiFuncAdr)
{

	return xMemSharedHandler[uiFuncAdr];
}
/**
 * Brief: Set data
 * uiFuncAdr:
 * retVal: None
 *
 */
void uiMemSet(uint8_t uiFuncAdr, uint8_t uiValue)
{

	xMemSharedHandler[uiFuncAdr] = uiValue;
}

/*
*


*/
void vTaskShareMem(void *pvParameters)
{
	ValveHandles_t *pxValveHandles = (ValveHandles_t *)pvParameters;
	Data_motor_t data_recv;
	Data_motor_t data_send;
	data_send.cmdSource = SHARED_MEM;
	portBASE_TYPE xStatus;
	/*
	- Protocol change shared mem directly and notify sharedmem
	- Shared mem will update to motor
	- Motor Callback update mem and the Taskshared mem will be notify to relay ack
	- 
	
	
	
	*/
	while (1)

	{
		/*Process the downlink*/
		xStatus = xQueuePeek(pxValveHandles->xQueueControlDownlink, &data_recv, NULL);
		if (xStatus == pdPASS)
		{
			xStatus = pdFAIL;
			if (data_recv.cmdSource != SHARED_MEM)
			{
				xStatus = xQueueReceive(pxValveHandles->xQueueControlDownlink, &data_recv, NULL);
				{
					if (xStatus == pdPASS)
					{
						/*Update Value to Shared mem*/
						uiMemSet(FUNC_WRITE_USER0_ADR + data_recv.motor_num, data_recv.state);
						/*And the forward to valve controller*/
						data_send.motor_num = data_recv.motor_num;
						data_send.state = data_recv.state;
						data_send.cmdSource = SHARED_MEM;
						xQueueSend(pxValveHandles->xQueueControlDownlink, &data_send, NULL);
					}
				}
			}
		}
		xStatus = xQueuePeek(pxValveHandles->xQueueReponseUplink, &data_recv, NULL);
		if (xStatus == pdPASS)
		{
			xStatus = pdFAIL;
			if (data_recv.cmdSource == MOTOR)
			{
				xStatus = xQueueReceive(pxValveHandles->xQueueReponseUplink, &data_recv, NULL);
				{
					if (xStatus == pdPASS)
					{
						/*Update Value to Shared mem*/
						//uiMemSet(FUNC_WRITE_USER0_ADR + data_recv.motor_num, data_recv.state);
						/*And the forward to valve controller*/
						data_send.motor_num = data_recv.motor_num;
						data_send.state = data_recv.state;
						data_send.cmdSource = SHARED_MEM;
						xQueueSend(pxValveHandles->xQueueReponseUplink, &data_send, NULL);
					}
				}
			}
		}
		// vTaskDelay(10);

		/*Waiting for the control from some protocol*/

		/*Update the database */

		/*Reponse to the specific protocol*/
	}
}