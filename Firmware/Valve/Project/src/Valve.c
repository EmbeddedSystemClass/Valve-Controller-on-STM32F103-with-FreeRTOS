/**
  ******************************************************************************
  * @file    Project/User/Valve
  * @author  NTT - BKSTEK Team
  * @version V1.0
  * @date    15-11-2019
  * @brief   Valve functions
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
//extern xQueueHandle xQueueControl;
/* Define GPIO Port Array for ez Index */
GPIO_TypeDef* SENSOR_PORT[MAX_MOTOR] = 
{	MOTOR1_SA_GPIO_Port, 
	MOTOR2_SA_GPIO_Port,
	MOTOR3_SA_GPIO_Port,
	MOTOR4_SA_GPIO_Port
}; 
GPIO_TypeDef* ENCODER_PORT[MAX_MOTOR] = 
{	MOTOR1_ENC_GPIO_Port, 
	MOTOR2_ENC_GPIO_Port,
	MOTOR3_ENC_GPIO_Port,
	MOTOR4_ENC_GPIO_Port
}; 
GPIO_TypeDef* MOTOR_PORT[MAX_MOTOR] = 
{
	MOTOR1_A_GPIO_Port, 
	MOTOR2_B_GPIO_Port,
	MOTOR4_B_GPIO_Port,
	MOTOR3_A_GPIO_Port
};

/* Define GPIO Pin Array for ez Index */
const uint16_t 	SENSOR_PIN_A[MAX_MOTOR] =
{
	MOTOR1_SA_Pin,
	MOTOR2_SA_Pin,
	MOTOR3_SA_Pin,
	MOTOR4_SA_Pin
};
const uint16_t 	SENSOR_PIN_B[MAX_MOTOR] =
{
	MOTOR1_SB_Pin,
	MOTOR2_SB_Pin,
	MOTOR3_SB_Pin,
	MOTOR4_SB_Pin
};
const uint16_t ENCODER_PIN[MAX_MOTOR]=
{
	MOTOR1_ENC_Pin,
	MOTOR2_ENC_Pin,
	MOTOR3_ENC_Pin,
	MOTOR4_ENC_Pin
};	
const uint16_t MOTOR_PIN_A[MAX_MOTOR] =
{
	MOTOR1_A_Pin,
	MOTOR2_A_Pin,
	MOTOR3_A_Pin,
	MOTOR4_A_Pin
};
const uint16_t MOTOR_PIN_B[MAX_MOTOR] =
{
	MOTOR1_B_Pin,
	MOTOR2_B_Pin,
	MOTOR3_B_Pin,
	MOTOR4_B_Pin
};

void Motor_Init(void)
{
		  /* GPIOD Periph clock enable */
	//Using GPIO to control Driver. To secure in future, please refer to using PWM with soft-start
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	
  GPIO_InitTypeDef GPIO_InitStructure;
	
	/*Init ouput and disable driver for safe in start up*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_10MHz;		
	for (int motor= 0; motor < MAX_MOTOR;motor++)
		{
			GPIO_InitStructure.GPIO_Pin = MOTOR_PIN_A[motor]|MOTOR_PIN_B[motor];
			GPIO_Init(MOTOR_PORT[motor],&GPIO_InitStructure);
			GPIO_ResetBits(MOTOR_PORT[motor],MOTOR_PIN_A[motor]|MOTOR_PIN_B[motor]);
		}
		/*Init input and pull up Sensor GPIO */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_10MHz;	
		for (int motor= 0; motor < MAX_MOTOR;motor++)
		{
			GPIO_InitStructure.GPIO_Pin = SENSOR_PIN_A[motor]|SENSOR_PIN_B[motor];
			GPIO_Init(SENSOR_PORT[motor],&GPIO_InitStructure);
		//	GPIO_SetBits(SENSOR_PORT[motor],SENSOR_PIN_A[motor]|SENSOR_PIN_B[motor]);
		}
		/*Init Encoder as Input (note: Encoder pin is not mapped with any timer input counter)*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Speed= GPIO_Speed_10MHz;	
		for (int motor= 0; motor < MAX_MOTOR;motor++)
		{
			GPIO_InitStructure.GPIO_Pin = ENCODER_PIN[motor];
			GPIO_Init(ENCODER_PORT[motor],&GPIO_InitStructure);
		//	GPIO_SetBits(SENSOR_PORT[motor],SENSOR_PIN_A[motor]|SENSOR_PIN_B[motor]);
		}
		
		/*NVIC for StopSensor/*/
			EXTI_InitTypeDef   EXTI_InitStructure;
		
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource0);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);
			EXTI_InitStructure.EXTI_Line = EXTI_Line0|EXTI_Line1;
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
			EXTI_Init(&EXTI_InitStructure);
		
			NVIC_InitTypeDef   NVIC_InitStructure;
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

			NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x08;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x08;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
			NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
			NVIC_Init(&NVIC_InitStructure);	
	//Home all
		Motor_Run_Right(3);
		for (int motor= 0; motor < MAX_MOTOR;motor++)
		{
			
		}
}

static void Motor_Home(uint8_t motor_num)
	{
		while(GPIO_ReadInputDataBit(SENSOR_PORT[motor_num],SENSOR_PIN_A[motor_num]))
		{
			Motor_Run_Left(motor_num);
		}
	}
	
static void Motor_Run_Left(uint8_t motor_num)
	{
		if(GPIO_ReadInputDataBit(SENSOR_PORT[motor_num],SENSOR_PIN_A[motor_num]))
		{
		GPIO_ResetBits(MOTOR_PORT[motor_num],MOTOR_PIN_A[motor_num]);
		GPIO_SetBits(MOTOR_PORT[motor_num],MOTOR_PIN_B[motor_num]);
		}
		
	}
static void Motor_Run_Right(uint8_t motor_num)
	{
		if(GPIO_ReadInputDataBit(SENSOR_PORT[motor_num],SENSOR_PIN_B[motor_num]))
		{
		GPIO_SetBits(MOTOR_PORT[motor_num],MOTOR_PIN_A[motor_num]);
		GPIO_ResetBits(MOTOR_PORT[motor_num],MOTOR_PIN_B[motor_num]);
		}
	}
void Motor_Stop(uint8_t motor_num)
{
	GPIO_ResetBits(MOTOR_PORT[motor_num],MOTOR_PIN_A[motor_num]);
	GPIO_ResetBits(MOTOR_PORT[motor_num],MOTOR_PIN_B[motor_num]);
}
//static bool Motor_Get_State()
void vTaskControlMotor(void *pvParameters)
{
			  ValveHandles_t *pxValveHandles = (ValveHandles_t*) pvParameters;
		
		Data_motor_t data_control;
		Data_motor_t data_reponse;
		portBASE_TYPE xStatus;
    const portTickType xTicksToWait = 100 / portTICK_RATE_MS;
		state_t MotorState[MAX_MOTOR];
		uint8_t buf = 0;
		while (TRUE)
		{
			
			xStatus = xQueueReceive( pxValveHandles->xQueueControl, &data_control, NULL );
	
			if (xStatus == pdPASS){
				printf("Motor controller recevied data: %d %d",data_control.motor_num,data_control.state);				
				MotorState[data_control.motor_num] = (data_control.state ==0)?2:3;
			}
			
			//Get state for each motor refer from queue
			for (uint8_t motor_num=0;motor_num<MAX_MOTOR;motor_num++)
					{
						switch (MotorState[motor_num])
							{
								case MOTOR_AT_LEFT: // this case seem to be a valve off
								{
									
									break;
								}
								case MOTOR_AT_RIGHT: //this case seem to be a valve on
								{
									
									break;
								}
								case MOTOR_RUN_LEFT:
								{
									Motor_Run_Left(motor_num);
									MotorState[motor_num] = MOTOR_AT_LEFT;
									break;
								}
								case MOTOR_RUN_RIGHT:
								{
									Motor_Run_Right(motor_num);
									MotorState[motor_num] = MOTOR_AT_RIGHT;
									break;
								}
								
							}
						}
				}
			
}

	