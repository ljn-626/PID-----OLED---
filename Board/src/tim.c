
// 基本定时器TIMx,x[6,7]定时初始化函数
#include "stm32f10x_tim.h"
#include "bsp_usart.h"
#include "tim.h"
__IO uint64_t time;         //使用volatile修饰,防止编译器优化
/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{ TIM_Prescaler            都有
 *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
 *  TIM_Period               都有
 *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
 *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
 *}TIM_TimeBaseInitTypeDef;
 *-----------------------------------------------------------------------------
 */

/**
 * @brief   定时器中断初始化
 * @param   arr :自动重装载寄存器值
 * @param   psc:时钟预分频数
 * @retval
 * @note    计数器频率为freq=72M/(psc+1),每隔 (arr+1)/freq 秒进入一次定时器中断
*/
void TIM_Init(uint16_t arr,uint16_t psc)
{
	  NVIC_InitTypeDef NVIC_InitStructure;
    //定时器TIM初始化
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_APBxClock_FUN(TIM_CLK, ENABLE);		                                           // 开启定时器时钟,即内部时钟CK_INT=72M
    TIM_TimeBaseStructure.TIM_Period = arr; 		                                        // 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Prescaler= psc;                                          // 设置时钟预分频数
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;               //时钟分频因子（互补输出，死区才用得到）
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;   //计数器计数模式，选择向上计数
    TIM_TimeBaseInit(TIM, &TIM_TimeBaseStructure);  	                        // 初始化定时器
    TIM_ClearFlag(TIM, TIM_FLAG_Update);                                            // 清除计数器中断标志位
    TIM_ITConfig(TIM,TIM_IT_Update,ENABLE);                                     //使能定时器中断,允许更新中断
    TIM_Cmd(TIM, ENABLE);                                                                   // 使能计数器

	//中断优先级NVIC设置
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);     	             //设置NVIC中断分组2:2位抢占优先级,2位响应优先级
    NVIC_InitStructure.NVIC_IRQChannel = TIM_IRQ ;                    // 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   // 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;              // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);                                               //初始化NVIC寄存器
}


/*编码器中断函数*/

u16 count1;//编码器计数 
u16 count2;//编码器计数 
double pi=3.1415926;
double v1;//速度
double v2;//速度
void TIM_IRQHandler(void)
{
		if(TIM_GetITStatus(TIM5, TIM_IT_Update))
		{
			count2=TIM_GetCounter(TIM4);
			count1=TIM_GetCounter(TIM3);
			v1=count1*pi*0.0635/(4*130*0.5);
			v2=count2*pi*0.0635/(4*130*0.5);
			//printf("left  %lf  ",v1);
			//printf("right  %lf",v2);
			TIM4->CNT=0;
			TIM3->CNT=0;
			TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //清除TIMx更新中断标志
		}
}


/**
 * @brief   定时器中断初始化
 * @param   arr :自动重装载寄存器值
 * @param   psc:时钟预分频数
 * @retval
 * @note    计数器频率为freq=72M/(psc+1),每隔 (arr+1)/freq 秒进入一次定时器中断
*/
void TIM7_Init(uint16_t arr,uint16_t psc)
{
	  NVIC_InitTypeDef NVIC_InitStructure;
    //定时器TIM初始化
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_APBxClock_FUN(RCC_APB1Periph_TIM7, ENABLE);		                                           // 开启定时器时钟,即内部时钟CK_INT=72M
    TIM_TimeBaseStructure.TIM_Period = arr; 		                                        // 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Prescaler= psc;                                          // 设置时钟预分频数
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;               //时钟分频因子（互补输出，死区才用得到）
    //TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;   //计数器计数模式，选择向上计数
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);  	                        // 初始化定时器
    TIM_ClearFlag(TIM7, TIM_FLAG_Update);                                            // 清除计数器中断标志位
    TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);                                     //使能定时器中断,允许更新中断
    TIM_Cmd(TIM7, ENABLE);                                                                   // 使能计数器

	//中断优先级NVIC设置
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);     	             //设置NVIC中断分组2:2位抢占优先级,2位响应优先级
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn ;                    // 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   // 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;              // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);                                               //初始化NVIC寄存器
}
/*********************************************END OF FILE**********************/
