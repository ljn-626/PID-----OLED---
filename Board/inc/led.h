#ifndef _LED_H_
#define _LED_H_

#include "stm32f10x.h"

//用枚举体进行封装
typedef enum 
{
	LED1,
	LED2,
	LED3,
	LED4,
	LED5,
	LED6,
	LED_ALL,
	LED_246,
	LED_135,
}LED;

extern void Led_Init(LED Led);
extern void Led_Set(LED Led, uint8_t State);
void waterfall_led(void);
	
#define PIN_LED1  GPIO_Pin_13
#define PIN_LED2  GPIO_Pin_14
#define PIN_LED3  GPIO_Pin_15
#define PIN_LED4  GPIO_Pin_0
#define PIN_LED5  GPIO_Pin_1
#define PIN_LED6  GPIO_Pin_7


#endif
