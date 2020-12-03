#include "led.h"


/**
  * @brief  LED初始化
  * @param  Led：LED灯（LED1 - LED6）
  * @retval 无
  * @example Led_Init(LED1);
  */
void Led_Init(LED Led)
{
	if(Led==LED1)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
		GPIO_InitStructure.GPIO_Pin = PIN_LED1;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOF ,&GPIO_InitStructure);
	}
	else if(Led==LED2)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
		GPIO_InitStructure.GPIO_Pin = PIN_LED2;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOF ,&GPIO_InitStructure);
	}
	else if(Led==LED3)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
		GPIO_InitStructure.GPIO_Pin = PIN_LED3;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOF ,&GPIO_InitStructure);
	}
	else if(Led==LED4)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
		GPIO_InitStructure.GPIO_Pin = PIN_LED4;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOG ,&GPIO_InitStructure);
	}
	else if(Led==LED5)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
		GPIO_InitStructure.GPIO_Pin = PIN_LED5;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOG ,&GPIO_InitStructure);
	}
	else if(Led==LED6)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
		GPIO_InitStructure.GPIO_Pin = PIN_LED6;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOE ,&GPIO_InitStructure);
	}
	else if(Led==LED_ALL)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG, ENABLE);
		
		GPIO_InitStructure.GPIO_Pin = PIN_LED6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOE ,&GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = PIN_LED1|PIN_LED2|PIN_LED3;
		GPIO_Init(GPIOF ,&GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = PIN_LED4|PIN_LED5;
		GPIO_Init(GPIOG ,&GPIO_InitStructure);
	}
	else if(Led==LED_246)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG, ENABLE);
		
		GPIO_InitStructure.GPIO_Pin = PIN_LED6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOE ,&GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = PIN_LED2;
		GPIO_Init(GPIOF ,&GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = PIN_LED4;
		GPIO_Init(GPIOG ,&GPIO_InitStructure);
		
	}
	else if(Led==LED_135)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG, ENABLE);
		
		
		
		GPIO_InitStructure.GPIO_Pin = PIN_LED1|PIN_LED3;
		GPIO_Init(GPIOF ,&GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = PIN_LED5;
		GPIO_Init(GPIOG ,&GPIO_InitStructure);
			
	}
}

/**
  * @brief  设置LED状态
  * @param  Led：LED灯（LED1 - LED_ALL）
  * @param  State： 设置的状态（0、1）
  * @retval 无
  * @example Led_Init(LED1,1);
  */
void Led_Set(LED Led, uint8_t State)
{
	if(Led==LED1)
	{
		if(State==1)
			GPIO_ResetBits(GPIOF,PIN_LED1);
		else if(State==0)
			GPIO_SetBits(GPIOF,PIN_LED1);
	}
	else if(Led==LED2)
	{
		if(State==1)
			GPIO_ResetBits(GPIOF,PIN_LED2);
		else if(State==0)
			GPIO_SetBits(GPIOF,PIN_LED2);
	}
	else if(Led==LED3)
	{
		if(State==1)
			GPIO_ResetBits(GPIOF,PIN_LED3);
		else if(State==0)
			GPIO_SetBits(GPIOF,PIN_LED3);
	}
	else if(Led==LED4)
	{
		if(State==1)
			GPIO_ResetBits(GPIOG,PIN_LED4);
		else if(State==0)
			GPIO_SetBits(GPIOG,PIN_LED4);
	}
	else if(Led==LED5)
	{
		if(State==1)
			GPIO_ResetBits(GPIOG,PIN_LED5);
		else if(State==0)
			GPIO_SetBits(GPIOG,PIN_LED5);
	}
	else if(Led==LED6)
	{
		if(State==1)
			GPIO_ResetBits(GPIOE,PIN_LED6);
		else if(State==0)
			GPIO_SetBits(GPIOE,PIN_LED6);
	}
	else if(Led==LED_ALL)
	{
		if(State==1)
		{
			GPIO_ResetBits(GPIOE,PIN_LED6);
			GPIO_ResetBits(GPIOF,PIN_LED1|PIN_LED2|PIN_LED3);
			GPIO_ResetBits(GPIOG,PIN_LED4|PIN_LED5);
		}
		else if(State==0)
		{
			GPIO_SetBits(GPIOE,PIN_LED6);
			GPIO_SetBits(GPIOF,PIN_LED1|PIN_LED2|PIN_LED3);
			GPIO_SetBits(GPIOG,PIN_LED4|PIN_LED5);
		}
	}
	else if(Led==LED_246)
	{
		if(State==1)
		{
			GPIO_ResetBits(GPIOE,PIN_LED6);
			GPIO_ResetBits(GPIOF,PIN_LED2);
			GPIO_ResetBits(GPIOG,PIN_LED4);
		}
		else if(State==0)
		{
			GPIO_SetBits(GPIOE,PIN_LED6);
			GPIO_SetBits(GPIOF,PIN_LED2);
			GPIO_SetBits(GPIOG,PIN_LED4);
		}
	}
	else if(Led==LED_135)
	{
		if(State==1)
		{
			GPIO_ResetBits(GPIOF,PIN_LED1|PIN_LED3);
			GPIO_ResetBits(GPIOG,PIN_LED5);
		}
		else if(State==0)
		{
			GPIO_SetBits(GPIOF,PIN_LED1|PIN_LED3);
			GPIO_SetBits(GPIOG,PIN_LED5);
		}
	}
}

/**
  * @brief  流水灯实验
  * @param  none
  * @param  none
  * @retval none
  * @example waterfall_led();   
  */

