#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f10x.h"
#include "delay.h"
#include "Servo.h"
#include "bsp_usart.h"
#include "bsp_adc.h"
#include "control.h"
#include "motor.h"
#include "encode.h"
#include "delay.h"
#include "led.h"
#include "tim.h"
extern int ServoControlValue;

// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];

// �ֲ����������ڱ���ת�������ĵ�ѹֵ 	 
//float ADC_ConvertedValueLocal[NOFCHANEL]; 
#endif

/* end of file cppyright reserve by team of yanjun ,More information please browse www.yanjuntech.cn */
