/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "encode.h"
#include "misc.h"

#include "delay.h"
#include "bsp_usart.h"

/*����TIM4��Ӧ�ı�������ֵ********************/
void TIM4_Mode_Config(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;   	
/*������������*********************************88*///PD12 ch1  ,PD13 ch2 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);//ʹ��GPIODʱ��
  GPIO_StructInit(&GPIO_InitStructure);//��GPIO_InitStruct�еĲ�����ȱʡֵ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13|GPIO_Pin_15;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//PD12 PD13��������	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
//   //���ø�����Ϊ�����������,���TIM4 CH4��PWM���岨��	GPIOD.15
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //TIM4_CH4
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIO	
  /*��ʱ������**********************************/
	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);//���������ӳ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//ʹ��TIM4ʱ��	
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 65535  ;    //0xffff ;  //�趨��������װֵ   TIMx_ARR = 359*4
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM4ʱ��Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//����ʱ�ӷָ� T_dts = T_ck_int	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ��� 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              
	/*TIMͨ������***************************************/
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);//�����Ϊ�����ؼ���TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdgeʹ�ñ�����ģʽ3�������½�������
	TIM_ICInitStructure.TIM_ICFilter =0;  //ѡ������Ƚ��˲��� 
  TIM_ICStructInit(&TIM_ICInitStructure);//Ĭ�ϲ�����ֵ�������ṹ���е�����ȱʡ���룿����������������������
  TIM_ICInit(TIM4, &TIM_ICInitStructure);//��TIM_ICInitStructure�е�ָ��������ʼ��TIM4
	/*-���ʹ��-*/
	TIM4->CNT = 0;
	TIM_Cmd(TIM4, ENABLE);   //����TIM4��ʱ��
}
void TIM3_Mode_Config(void)//CH1 PC6 /CH2  PC7
{

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;   	
  //���ų�ʼ��***********************************
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);//ʹ��GPIOCʱ��
  GPIO_StructInit(&GPIO_InitStructure);//��GPIO_InitStruct�еĲ�����ȱʡֵ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//PD12 PD13��������	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
  /*��ʱ������**********************************/
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3 ,ENABLE);//���������ӳ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//ʹ��TIM3ʱ��	
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 65535  ;    //0xffff ;  //�趨��������װֵ   TIMx_ARR = 359*4
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM3ʱ��Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//����ʱ�ӷָ� T_dts = T_ck_int	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ��� 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);              
	/*TIMͨ������***************************************/
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);//�����Ϊ�����ؼ���TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdgeʹ�ñ�����ģʽ3�������½�������
	TIM_ICInitStructure.TIM_ICFilter =0;  //ѡ������Ƚ��˲��� 
  TIM_ICStructInit(&TIM_ICInitStructure);//Ĭ�ϲ�����ֵ�������ṹ���е�����ȱʡ���룿����������������������
  TIM_ICInit(TIM3, &TIM_ICInitStructure);//��TIM_ICInitStructure�е�ָ��������ʼ��TIM4
	/*-���ʹ��-*/
	TIM3->CNT = 0;
	TIM_Cmd(TIM3, ENABLE);   //����TIM4��ʱ��
}
void TIM4_Init(void)
{
  TIM4_Mode_Config();
	
}
void TIM3_Init(void)
{
  TIM3_Mode_Config();
	
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
