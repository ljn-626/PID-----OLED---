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

/*第一步求取电压所需变量及数组*/
extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];//调用读取外部电压的全局变量
uint16_t ADC_First[4][5];//四个电感各读取五个值

/*第二步初级滤波所用变量及数组*/
uint8_t i,j,k;
uint16_t Temp;
uint16_t ADC_Sum[4],ADC_Second_SumAverage[4];//求取滤波后的平均值

/*第三步二次滤波所用变量及数组(不太会用)*/
//#define ADC_Smoothtime 8
//uint16_t ADC_Third[4][ADC_Smoothtime];
//uint16_t ADC_Third_SumAverage[4];

/*第四步归一化所需变量及数组*/
//float ADC_Max[4] = {4095,3309.9,3111.3,4095};        //四路ADC值的最大值 
//float ADC_Min[4] = {149,58,58,58};        //四路ADC值的最小值
float ADC_Max[4] = {4094.3,3475.3,3309.9,4094.3};        //四路ADC值的最大值(赛道)
float ADC_Min[4] = {149,58,58,58};        //四路ADC值的最小值(赛道)
float ADC_End[4];
uint8_t g;

/*第五步偏差计算所用变量及数组*/
float ADC_Left_Sum=0,ADC_Right_Sum=0;//左右电感和
float ADC_Left_Sqrt=0,ADC_Right_Sqrt=0;//左右电感开方和
float Servo_Error;//存放偏差值

/*第六步PID所用变量及数组*/
float KPsum,KDsum;
float Last_Servo_Error,End_Error;

float Deviation_Calculation(void)
{
	/*第一步***********************************************/
	 for(i=0;i < 5;i++)
  {
    ADC_First[0][i]=ADC_ConvertedValue[2];
    ADC_First[1][i]=ADC_ConvertedValue[3];
    ADC_First[2][i]=ADC_ConvertedValue[1];
    ADC_First[3][i]=ADC_ConvertedValue[0];
  }
	/*第二步***********************************************/
	for(i=0;i<4;i++)//冒泡排序，把每一路的ADC值从大到小排列
  {
    for(j=0;j<4;j++)  
    {
      for(k=0;k<4-j;k++)
      {
        if(ADC_First[i][k] > ADC_First[i][k+1])//前面的比后面的大  则进行交换
        {
          Temp = ADC_First[i][k+1];
          ADC_First[i][k+1] = ADC_First[i][k];
          ADC_First[i][k] = Temp;
        }
      }
    }
  }
  for(i=0;i<4;i++)//去掉最大最小值，求中间三项的和，并进行平均运算
  {
    ADC_Sum[i] = ADC_First[i][1] + ADC_First[i][2] + ADC_First[i][3];       
    ADC_Second_SumAverage[i] = ADC_Sum[i] / 3;	
  }
	/*第三步***********************************************/
	//暂时不用
	/*第四步***********************************************/
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
	/*第五步***********************************************/
	ADC_Left_Sum=ADC_End[0]+ADC_End[1];
	ADC_Right_Sum=ADC_End[2]+ADC_End[3];
	
	ADC_Left_Sqrt=sqrt(ADC_Left_Sum);
	ADC_Right_Sqrt=sqrt(ADC_Right_Sum);
	
	Servo_Error=(ADC_Left_Sqrt-ADC_Right_Sqrt)/(ADC_Left_Sqrt+ADC_Right_Sqrt);
	Servo_Error=Servo_Error*2000;
	/*第六步***********************************************/
	KPsum =  Servo_Error*TURN_KP;
  KDsum = (Servo_Error - Last_Servo_Error) * TURN_KD;
	
	Last_Servo_Error = Servo_Error;
	End_Error = KPsum + KDsum;
	
	return End_Error;
}
//电机开环控制
void Motor_Control()
{
		TIM_SetCompare1(TIM1,350);//左
		TIM_SetCompare2(TIM1,450);
		TIM_SetCompare4(TIM1,350);//右
		TIM_SetCompare3(TIM1,450);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------*/
/*电机PID********************/

/*所需变量定义***************/
int BMQ; //编码器读取值
int Speed_Expect,Speed_Actual;//速度预期值，速度实际值
int Speed_Error;//总误差
int Speed_Error_P,Speed_Error_I,Speed_Error_D;//p i d 分别计算出来的误差值
int Speed_Error_Last,Speed_End;

/*函数定义*******************/
int Speed_Control (int Speed_Expect)
{
	BMQ  = TIM3->CNT;  //读取脉冲值
  TIM3->CNT = 0;          //计数器清零
	if(BMQ < 32767)//辨别方向
  {
    Speed_Actual = BMQ;
  }
  else
  {
    Speed_Actual = BMQ - 65535;     
  } 
  
  Speed_Error = Speed_Expect - Speed_Actual;//求偏差
  
  //计算速度环P项偏差
  Speed_Error_P = (int)(SPEED_KP * Speed_Error);
  
  //计算速度环I项偏差  
  Speed_Error_I += SPEED_KI * Speed_Error;
  
  //积分上限与积分下限，这里的限副需要根据实际情况做调整
  if(Speed_Error_I > 500)
  {
    Speed_Error_I = 500;
  }
  else if(Speed_Error_I <= -500)
  {
    Speed_Error_I = -500;
  }
  
  //计算速度环D项偏差    
  Speed_Error_D = (int)(SPEED_KD * (Speed_Error - Speed_Error_Last));
  Speed_Error_Last = Speed_Error;
  
  //求和 
  Speed_End = Speed_Error_P + Speed_Error_I + Speed_Error_D;
  
  //限副
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

/*舵机赛道特殊元素判断*-----------------------------------------------------------------------------*/
/*所用变量******************/


/*舵机普通赛道判断******************/

/*所需变量******************/
/*所需变量定义**************/
u16 ServoOffset = 50;//舵机中值偏移量
float End_Servo_Error;
int ServoControlValue=0;
int x;//判断双电感差比和次数
//所用函数
void Normal_Judge_Servo()
{
	  End_Servo_Error = Deviation_Calculation();
		ServoControlValue = (int)End_Servo_Error+SERVO_MID ;//舵机转向变量处理
    Servo_Control(ServoControlValue);
		Element_Change_Flag=1;
}






/*舵机环岛判断******************/

/*所需变量******************/
//所用函数
void Island_Judge_Servo()
{
	if(AD_Middle>3000)//&&AD_Left_Stright>AD_Right_Stright+2000
	{
		//TIM_SetCompare2(TIM2,1650);
			for(x=0;x<10;x++)
			{
				End_Servo_Error = Two_ADC_Calculation();
				ServoControlValue = (int)End_Servo_Error+SERVO_MID ;//舵机转向变量处理
				Servo_Control(ServoControlValue);
				//delay_ms(10);
			}
		
		Element_Change_Flag=1;	
		Island_Flag++;
	}
	
}






/*舵机坡道判断******************/

/*所需变量******************/
//所用函数
void Hill_Judge_Servo()
{

}








/*舵机大S弯判断******************/

/*所需变量******************/
//所用函数
void SW_Judge_Servo()
{

}

/*--------------------------------------------------------------------------------------------------------------------------------*/

/*电机赛道特殊元素判断*-----------------------------------------------------------------------------*/
/*所用变量******************/


/*电机普通赛道判断******************/

/*所需变量******************/
/*所需变量定义**************/

//所用函数
void Normal_Judge_Motor()
{
	  Motor_Control();
}






/*电机环岛判断******************/

/*所需变量******************/
//所用函数
void Island_Judge_Motor()
{

}






/*电机坡道判断******************/

/*所需变量******************/
//所用函数
void Hill_Judge_Motor()
{

}








/*电机大S弯判断******************/

/*所需变量******************/
//所用函数
void SW_Judge_Motor()
{

}

/*--------------------------------------------------------------------------------------------------------------------------------*/

/*左右两路竖直电感差比和*/
float Two_ADC_Calculation(void)
{
	/*第一步***********************************************/
	 for(i=0;i < 5;i++)
  {
    ADC_First[0][i]=ADC_ConvertedValue[2];//左侧直
    ADC_First[1][i]=ADC_ConvertedValue[0];//右侧直
  }
	/*第二步***********************************************/
	for(i=0;i<2;i++)//冒泡排序，把每一路的ADC值从大到小排列
  {
    for(j=0;j<4;j++)  
    {
      for(k=0;k<4-j;k++)
      {
        if(ADC_First[i][k] > ADC_First[i][k+1])//前面的比后面的大  则进行交换
        {
          Temp = ADC_First[i][k+1];
          ADC_First[i][k+1] = ADC_First[i][k];
          ADC_First[i][k] = Temp;
        }
      }
    }
  }
  for(i=0;i<2;i++)//去掉最大最小值，求中间三项的和，并进行平均运算
  {
    ADC_Sum[i] = ADC_First[i][1] + ADC_First[i][2] + ADC_First[i][3];       
    ADC_Second_SumAverage[i] = ADC_Sum[i] / 3;	
  }
	/*第三步***********************************************/
	//暂时不用
	/*第四步***********************************************/
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
	/*第五步***********************************************/
	ADC_Left_Sum=ADC_End[0];
	ADC_Right_Sum=ADC_End[1];
	
	ADC_Left_Sqrt=sqrt(ADC_Left_Sum);
	ADC_Right_Sqrt=sqrt(ADC_Right_Sum);
	
	Servo_Error=(ADC_Left_Sqrt-ADC_Right_Sqrt)/(ADC_Left_Sqrt+ADC_Right_Sqrt);
	Servo_Error=Servo_Error*10000;
	/*第六步***********************************************/
	KPsum =  Servo_Error*TURN_KP;
  KDsum = (Servo_Error - Last_Servo_Error) * TURN_KD;
	
	Last_Servo_Error = Servo_Error;
	End_Error = KPsum + KDsum;
	
	return End_Error;
}

