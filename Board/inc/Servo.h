#ifndef _SERVO_H_
#define _SERVO_H_
#include "stm32f10x.h"
#define	RCC_APBPeriph_TIMx_GPIOx	RCC_APB2Periph_GPIOA
#define	RCC_APBPeriph_TIMx	RCC_APB1Periph_TIM2
#define	TIM_GPIO	GPIOA
#define TIM_SERVO	TIM2

#define SERVO_MID 1500
#define SERVO_MIN 1350
#define SERVO_MAX 1650

extern void TIM_PWM_Init(void);

extern void  Servo_Control(uint16_t DutyCycle);

extern float Delta(float VLeft,float VRight);
#endif
