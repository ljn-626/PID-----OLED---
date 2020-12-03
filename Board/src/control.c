#include "control.h"
#include "math.h"
#include "bsp_adc.h"
#include "Servo.h"
#include "delay.h"


#define TURN_KP  0.1
#define TURN_KI  
#define TURN_KD  0.5

#define SPEED_KP  0.05
#define SPEED_KI  0.1
#define SPEED_KD  0.1

/*��һ����ȡ��ѹ�������������*/
extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];//���ö�ȡ�ⲿ��ѹ��ȫ�ֱ���
uint16_t ADC_First[4][5];//�ĸ���и���ȡ���ֵ

/*�ڶ��������˲����ñ���������*/
uint8_t i,j,k;
uint16_t Temp;
uint16_t ADC_Sum[4],ADC_Second_SumAverage[4];//��ȡ�˲����ƽ��ֵ

/*�����������˲����ñ���������(��̫����)*/
//#define ADC_Smoothtime 8
//uint16_t ADC_Third[4][ADC_Smoothtime];
//uint16_t ADC_Third_SumAverage[4];

/*���Ĳ���һ���������������*/
//float ADC_Max[4] = {4095,3309.9,3111.3,4095};        //��·ADCֵ�����ֵ 
//float ADC_Min[4] = {149,58,58,58};        //��·ADCֵ����Сֵ
float ADC_Max[4] = {4094.3,3475.3,3309.9,4094.3};        //��·ADCֵ�����ֵ(����)
float ADC_Min[4] = {149,58,58,58};        //��·ADCֵ����Сֵ(����)
float ADC_End[4];
uint8_t g;

/*���岽ƫ��������ñ���������*/
float ADC_Left_Sum=0,ADC_Right_Sum=0;//���ҵ�к�
float ADC_Left_Sqrt=0,ADC_Right_Sqrt=0;//���ҵ�п�����
float Servo_Error;//���ƫ��ֵ

/*������PID���ñ���������*/
float KPsum,KDsum;
float Last_Servo_Error,End_Error;

float Deviation_Calculation(void)
{
	/*��һ��***********************************************/
	 for(i=0;i < 5;i++)
  {
    ADC_First[0][i]=ADC_ConvertedValue[2];
    ADC_First[1][i]=ADC_ConvertedValue[3];
    ADC_First[2][i]=ADC_ConvertedValue[1];
    ADC_First[3][i]=ADC_ConvertedValue[0];
  }
	/*�ڶ���***********************************************/
	for(i=0;i<4;i++)//ð�����򣬰�ÿһ·��ADCֵ�Ӵ�С����
  {
    for(j=0;j<4;j++)  
    {
      for(k=0;k<4-j;k++)
      {
        if(ADC_First[i][k] > ADC_First[i][k+1])//ǰ��ıȺ���Ĵ�  ����н���
        {
          Temp = ADC_First[i][k+1];
          ADC_First[i][k+1] = ADC_First[i][k];
          ADC_First[i][k] = Temp;
        }
      }
    }
  }
  for(i=0;i<4;i++)//ȥ�������Сֵ�����м�����ĺͣ�������ƽ������
  {
    ADC_Sum[i] = ADC_First[i][1] + ADC_First[i][2] + ADC_First[i][3];       
    ADC_Second_SumAverage[i] = ADC_Sum[i] / 3;	
  }
	/*������***********************************************/
	//��ʱ����
	/*���Ĳ�***********************************************/
	for(g=0;g<4;g++)
	{
		ADC_End[g] = (float)(ADC_Second_SumAverage[g] - ADC_Min[g])/(float)(ADC_Max[g] - ADC_Min[g]);
    if(ADC_End[g]<=0.0)
		{
			ADC_End[g]=0.001;
		}
		if(ADC_End[g]>1.0)
		{
			ADC_End[g]=1.0;
		}
		ADC_End[g]=100*ADC_End[g];
	}
	/*���岽***********************************************/
	ADC_Left_Sum=ADC_End[0]+ADC_End[1];
	ADC_Right_Sum=ADC_End[2]+ADC_End[3];
	
	ADC_Left_Sqrt=sqrt(ADC_Left_Sum);
	ADC_Right_Sqrt=sqrt(ADC_Right_Sum);
	
	Servo_Error=(ADC_Left_Sqrt-ADC_Right_Sqrt)/(ADC_Left_Sqrt+ADC_Right_Sqrt);
	Servo_Error=Servo_Error*2000;
	/*������***********************************************/
	KPsum =  Servo_Error*TURN_KP;
  KDsum = (Servo_Error - Last_Servo_Error) * TURN_KD;
	
	Last_Servo_Error = Servo_Error;
	End_Error = KPsum + KDsum;
	
	return End_Error;
}
//�����������
void Motor_Control()
{
		TIM_SetCompare1(TIM1,350);//��
		TIM_SetCompare2(TIM1,450);
		TIM_SetCompare4(TIM1,350);//��
		TIM_SetCompare3(TIM1,450);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------*/
/*���PID********************/

/*�����������***************/
int BMQ; //��������ȡֵ
int Speed_Expect,Speed_Actual;//�ٶ�Ԥ��ֵ���ٶ�ʵ��ֵ
int Speed_Error;//�����
int Speed_Error_P,Speed_Error_I,Speed_Error_D;//p i d �ֱ������������ֵ
int Speed_Error_Last,Speed_End;

/*��������*******************/
int Speed_Control (int Speed_Expect)
{
	BMQ  = TIM3->CNT;  //��ȡ����ֵ
  TIM3->CNT = 0;          //����������
	if(BMQ < 32767)//�����
  {
    Speed_Actual = BMQ;
  }
  else
  {
    Speed_Actual = BMQ - 65535;     
  } 
  
  Speed_Error = Speed_Expect - Speed_Actual;//��ƫ��
  
  //�����ٶȻ�P��ƫ��
  Speed_Error_P = (int)(SPEED_KP * Speed_Error);
  
  //�����ٶȻ�I��ƫ��  
  Speed_Error_I += SPEED_KI * Speed_Error;
  
  //����������������ޣ�������޸���Ҫ����ʵ�����������
  if(Speed_Error_I > 500)
  {
    Speed_Error_I = 500;
  }
  else if(Speed_Error_I <= -500)
  {
    Speed_Error_I = -500;
  }
  
  //�����ٶȻ�D��ƫ��    
  Speed_Error_D = (int)(SPEED_KD * (Speed_Error - Speed_Error_Last));
  Speed_Error_Last = Speed_Error;
  
  //��� 
  Speed_End = Speed_Error_P + Speed_Error_I + Speed_Error_D;
  
  //�޸�
  if(Speed_End >= 999)
  {
    Speed_End = 999;
  }
  else if(Speed_End <= -999)
  {
    Speed_End = -999;
  }
  
  
  return Speed_End;

}

/*�����������Ԫ���ж�*-----------------------------------------------------------------------------*/
/*���ñ���******************/


/*�����ͨ�����ж�******************/

/*�������******************/
/*�����������**************/
u16 ServoOffset = 50;//�����ֵƫ����
float End_Servo_Error;
int ServoControlValue=0;
int x;//�ж�˫��в�Ⱥʹ���
//���ú���
void Normal_Judge_Servo()
{
	  End_Servo_Error = Deviation_Calculation();
		ServoControlValue = (int)End_Servo_Error+SERVO_MID ;//���ת���������
    Servo_Control(ServoControlValue);
		Element_Change_Flag=1;
}






/*��������ж�******************/

/*�������******************/
//���ú���
void Island_Judge_Servo()
{
	if(AD_Middle>3000)//&&AD_Left_Stright>AD_Right_Stright+2000
	{
		//TIM_SetCompare2(TIM2,1650);
			for(x=0;x<10;x++)
			{
				End_Servo_Error = Two_ADC_Calculation();
				ServoControlValue = (int)End_Servo_Error+SERVO_MID ;//���ת���������
				Servo_Control(ServoControlValue);
				//delay_ms(10);
			}
		
		Element_Change_Flag=1;	
		Island_Flag++;
	}
	
}






/*����µ��ж�******************/

/*�������******************/
//���ú���
void Hill_Judge_Servo()
{

}








/*�����S���ж�******************/

/*�������******************/
//���ú���
void SW_Judge_Servo()
{

}

/*--------------------------------------------------------------------------------------------------------------------------------*/

/*�����������Ԫ���ж�*-----------------------------------------------------------------------------*/
/*���ñ���******************/


/*�����ͨ�����ж�******************/

/*�������******************/
/*�����������**************/

//���ú���
void Normal_Judge_Motor()
{
	  Motor_Control();
}






/*��������ж�******************/

/*�������******************/
//���ú���
void Island_Judge_Motor()
{

}






/*����µ��ж�******************/

/*�������******************/
//���ú���
void Hill_Judge_Motor()
{

}








/*�����S���ж�******************/

/*�������******************/
//���ú���
void SW_Judge_Motor()
{

}

/*--------------------------------------------------------------------------------------------------------------------------------*/

/*������·��ֱ��в�Ⱥ�*/
float Two_ADC_Calculation(void)
{
	/*��һ��***********************************************/
	 for(i=0;i < 5;i++)
  {
    ADC_First[0][i]=ADC_ConvertedValue[2];//���ֱ
    ADC_First[1][i]=ADC_ConvertedValue[0];//�Ҳ�ֱ
  }
	/*�ڶ���***********************************************/
	for(i=0;i<2;i++)//ð�����򣬰�ÿһ·��ADCֵ�Ӵ�С����
  {
    for(j=0;j<4;j++)  
    {
      for(k=0;k<4-j;k++)
      {
        if(ADC_First[i][k] > ADC_First[i][k+1])//ǰ��ıȺ���Ĵ�  ����н���
        {
          Temp = ADC_First[i][k+1];
          ADC_First[i][k+1] = ADC_First[i][k];
          ADC_First[i][k] = Temp;
        }
      }
    }
  }
  for(i=0;i<2;i++)//ȥ�������Сֵ�����м�����ĺͣ�������ƽ������
  {
    ADC_Sum[i] = ADC_First[i][1] + ADC_First[i][2] + ADC_First[i][3];       
    ADC_Second_SumAverage[i] = ADC_Sum[i] / 3;	
  }
	/*������***********************************************/
	//��ʱ����
	/*���Ĳ�***********************************************/
	for(g=0;g<2;g++)
	{
		ADC_End[g] = (float)(ADC_Second_SumAverage[g] - ADC_Min[g])/(float)(ADC_Max[g] - ADC_Min[g]);
    if(ADC_End[g]<=0.0)
		{
			ADC_End[g]=0.001;
		}
		if(ADC_End[g]>1.0)
		{
			ADC_End[g]=1.0;
		}
		ADC_End[g]=100*ADC_End[g];
	}
	/*���岽***********************************************/
	ADC_Left_Sum=ADC_End[0];
	ADC_Right_Sum=ADC_End[1];
	
	ADC_Left_Sqrt=sqrt(ADC_Left_Sum);
	ADC_Right_Sqrt=sqrt(ADC_Right_Sum);
	
	Servo_Error=(ADC_Left_Sqrt-ADC_Right_Sqrt)/(ADC_Left_Sqrt+ADC_Right_Sqrt);
	Servo_Error=Servo_Error*10000;
	/*������***********************************************/
	KPsum =  Servo_Error*TURN_KP;
  KDsum = (Servo_Error - Last_Servo_Error) * TURN_KD;
	
	Last_Servo_Error = Servo_Error;
	End_Error = KPsum + KDsum;
	
	return End_Error;
}

