#ifndef _BUTTON_H__
#define _BUTTON_H__
/* Private define ------------------------------------------------------------*/

#define xBTN1 1
#define xBTN2 2
#define xBTN3 3
/* Private typedef ------------------------------------------------------------*/


typedef     enum 
{PRESSED=0,
RELEASED}  sw_t;

typedef struct
{
    sw_t buttonValue;
    unsigned int taskSource;
} Data_t;


/* Private function prototype ------------------------------------------------------------*/
sw_t        switchState(int SWnumber);
void vTaskButton1(void *pvParameters);
void vTaskButton2(void *pvParameters);
void vTaskButton3(void *pvParameters);
#endif

