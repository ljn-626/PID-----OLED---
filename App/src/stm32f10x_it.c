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
#include "stm32f10x_it.h" 

int Element_Change_Flag;
int Island_Flag=0;
extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];

 
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
void SysTick_Handler(void)
{
}



//舵机中断控制5ms触发一次
void TIM7_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update))  //检查TIM3更新中断发生与否
	{
		Element_Change_Flag=0;
		if(Element_Change_Flag==0&&Island_Flag<=1)
		{
			Island_Judge_Servo();
		}
//		if(Element_Change_Flag==0)
//		{
//			SW_Judge_Servo();
//		}
//		if(Element_Change_Flag==0)
//		{
//			Hill_Judge_Servo();
//		}
		if(Element_Change_Flag==0)
			
		{
			Normal_Judge_Servo();
		}	
		////////////////////////////////////////////////////
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  //清除TIMx更新中断标志
	}
	
}

void TIM1_UP_IRQHandler(void)//电机中断控制，20ms触发一次
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update))  //检查TIM3更新中断发生与否
	{
		Element_Change_Flag=0;
		if(ADC_ConvertedValue[0]<100&&ADC_ConvertedValue[2]<240)
		{
			TIM_SetCompare1(TIM1,200);//反
			TIM_SetCompare2(TIM1,200);//正
			TIM_SetCompare3(TIM1,200);//左正
			TIM_SetCompare4(TIM1,200);//左反
		}
		else
		{
			if(Element_Change_Flag==0)
			{
				Island_Judge_Motor();
			}
			if(Element_Change_Flag==0)
			{
				SW_Judge_Motor();
			}
			if(Element_Change_Flag==0)
			{
				Hill_Judge_Motor();
			}
			if(Element_Change_Flag==0)
			{
				Normal_Judge_Motor();
			}	
		}
		
	////////////////////////////////////////////////////////
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //清除TIMx更新中断标志
	}
	
}


void TIM8_UP_IRQHandler(void)//右编码器中断控制
{
	
	
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

