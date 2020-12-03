#include "Servo.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "control.h"
#include "math.h"
#include "bsp_adc.h"

extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];
float VLeft;
float VRight;
float Error0=0.0;
float leftsqrt=0.0;
float rightsqrt=0.0;
/**
  * @brief	�߼���ʱ����ʼ��
  *	@param	��
  *	@retval ��
  */
void TIM_PWM_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO��ʼ���ṹ��
	//����Ƚ�ͨ��1��2��3 GPIO��ʼ��
	RCC_APB2PeriphClockCmd(RCC_APBPeriph_TIMx_GPIOx, ENABLE);	//ʹ����������ʱ��
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_2;	//����GPIO�ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�������Ƶ��
	GPIO_Init(TIM_GPIO, &GPIO_InitStructure);	//��ʼ��GPIO
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	//��ʱ��ʱ���ṹ��
	TIM_OCInitTypeDef TIM_OCInitStructure;	 //��ʱ������ȽϽṹ��
	
	RCC_APB1PeriphClockCmd(RCC_APBPeriph_TIMx, ENABLE);	 //ʹ�ܶ�ʱ��ʱ��
	
	/*--------------------ʱ���ṹ���ʼ��-------------------------*/
	// �������ڣ���������Ϊ0.02s,20ms
	TIM_TimeBaseStructure.TIM_Period = 20000-1;	//�Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Prescaler= 71;	 //����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		//ʱ�ӷ�Ƶ���� ���������������������ʱ����Ҫ�õ�
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		//����������ģʽ������Ϊ���ϼ���
	//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	//�����ظ���������ֵΪ0���������ظ�����
	TIM_TimeBaseInit(TIM_SERVO, &TIM_TimeBaseStructure);  //��ʼ����ʱ��

	/*--------------------����ȽϽṹ���ʼ��-------------------*/	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //ʹ������Ƚ�
	TIM_OCInitStructure.TIM_Pulse = 0;  //���ó�ʼռ�ձ�
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //�������:TIM����Ƚϼ��Ը�(�ߵ�ƽ��Ч)
	
	TIM_OC1Init(TIM_SERVO, &TIM_OCInitStructure);  //��ʼ������Ƚ�ͨ��1
	TIM_OC1PreloadConfig(TIM_SERVO, TIM_OCPreload_Enable);  //ʹ��ͨ��1��CCR4�ϵ�Ԥװ�ؼĴ���
	
	TIM_OC2Init(TIM_SERVO, &TIM_OCInitStructure);  //��ʼ������Ƚ�ͨ��2
	TIM_OC2PreloadConfig(TIM_SERVO, TIM_OCPreload_Enable);  //ʹ��ͨ��2��CCR4�ϵ�Ԥװ�ؼĴ���
	
	TIM_OC3Init(TIM_SERVO, &TIM_OCInitStructure);  //��ʼ������Ƚ�ͨ��3
	TIM_OC3PreloadConfig(TIM_SERVO, TIM_OCPreload_Enable);  //ʹ��ͨ��3��CCR4�ϵ�Ԥװ�ؼĴ���
	
	
 
	
	
	TIM_Cmd(TIM_SERVO, ENABLE);  //ʹ��TIM
	TIM_CtrlPWMOutputs(TIM_SERVO, ENABLE);  //�����ʹ�ܣ���ʹ�õ���ͨ�ö�ʱ��������Ҫ�����ʹ��
	
//	//�ж����ȼ�NVIC����
//	  TIM_ClearFlag(TIM2, TIM_FLAG_Update);                                            // ����������жϱ�־λ
//    NVIC_InitTypeDef NVIC_InitStructure;
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);     	             //����NVIC�жϷ���2:2λ��ռ���ȼ�,2λ��Ӧ���ȼ�
//    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn ;                    // �����ж���Դ
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;   // ���������ȼ�Ϊ 0
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;              // ������ռ���ȼ�Ϊ3
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQͨ����ʹ��
//    NVIC_Init(&NVIC_InitStructure);                                               //��ʼ��NVIC�Ĵ��� 
//		TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);                                     //ʹ�ܶ�ʱ���ж�,��������ж�
}

void Servo_Control(uint16_t DutyCycle)
{
		if(DutyCycle >= SERVO_MAX)  //����޸�
		{
			DutyCycle = SERVO_MAX;
		}
		else if(DutyCycle <= SERVO_MIN)  //�Ҳ��޸�
		{
			DutyCycle = SERVO_MIN;
		}
	 TIM_SetCompare2(TIM2,DutyCycle);
}

