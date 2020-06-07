
/* Built-in C library includes ----------------------*/

#include <stdio.h>
#include <stdint.h>
/* Platform includes --------------------------------*/
#include "main.h"
#include "FreeRTOS.h"
#include "Valve.h"
#include "sharedmem.h"
// #include "sharedmem.h"
/* Modbus includes ---------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "mbtask.h"
#define MEM_MAX_SIZE (40)
#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 8

extern ValveHandles_t ValveHandles;
static USHORT usRegInputStart = REG_INPUT_START;
static USHORT usRegInputBuf[REG_INPUT_NREGS];

void vTaskModbusStack(void *pvParameter)
{

    eMBErrorCode eStatus = eMBInit(MB_RTU, 1, 3, 9600, MB_PAR_NONE);
    eStatus = eMBEnable();
    while (1)
    {
        eMBPoll();
        vTaskDelay(10);
    }
}

void vTaskModbus(void *pvParameter)
{
}

eMBErrorCode
eMBRegInputCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs)
{
    eMBErrorCode eStatus = MB_ENOERR;
    int iRegIndex;

    if ((usAddress >= REG_INPUT_START) && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS))
    {
        iRegIndex = (int)(usAddress - usRegInputStart);
        while (usNRegs > 0)
        {
            *pucRegBuffer++ =
                (unsigned char)(usRegInputBuf[iRegIndex] >> 8);
            *pucRegBuffer++ =
                (unsigned char)(usRegInputBuf[iRegIndex] & 0xFF);
            iRegIndex++;
            usNRegs--;
        }

        //	HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
    }
    else
    {
        // HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                eMBRegisterMode eMode)
{
    eMBErrorCode eStatus = MB_ENOERR;
    USHORT iRegIndex;
    USHORT *pusRegHoldingBuf;
    USHORT REG_HOLDING_START;
    USHORT REG_HOLDING_NREGS;
    USHORT usRegHoldStart;
    Data_motor_t data_control;
    //   pusRegHoldingBuf = usSRegHoldBuf;
    //   REG_HOLDING_START = S_REG_HOLDING_START;
    //   REG_HOLDING_NREGS = S_REG_HOLDING_NREGS;
    //   usRegHoldStart = usSRegHoldStart;
    printf("\r\n MB callback function");
    usAddress--;
    if (usAddress < MEM_MAX_SIZE)
    {
        switch (eMode)
        {
        case MB_REG_READ:
            *pucRegBuffer++ = (UCHAR)(uiMemGet(usAddress) >> 8);
            *pucRegBuffer++ = (UCHAR)(uiMemGet(usAddress) & 0xFF);
            break;
        case MB_REG_WRITE:
            data_control.cmdSource = MODBUS;
            data_control.motor_num = usAddress - 20;
            data_control.state     = *(pucRegBuffer + 1);
             xQueueSend(ValveHandles.xQueueControlDownlink, &data_control, 10);
            //					uiMemSet(usAddress, *(pucRegBuffer+1) );
            printf("\r\n MB callback uiMemSet: adr: %d val: %d", usAddress, *(pucRegBuffer + 1));
            break;
        }
    }

    return MB_ENOERR;
}

eMBErrorCode
eMBRegCoilsCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNCoils,
              eMBRegisterMode eMode)
{
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNDiscrete)
{
    return MB_ENOREG;
}