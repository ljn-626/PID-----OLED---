#ifndef _CONTROL_H
#define	_CONTROL_H

#define AD_Left_Stright  ADC_ConvertedValue[2] 
#define AD_Left_Across   ADC_ConvertedValue[3]
#define AD_Right_Stright ADC_ConvertedValue[0]
#define AD_Right_Across  ADC_ConvertedValue[1]
#define AD_Middle        ADC_ConvertedValue[4]

//变量定义
extern float fLeft ;
extern float fRight ;
int Speed_Control (int Speed_Expect);
extern int BMQ;
extern int Element_Change_Flag;
extern int Island_Flag;
//函数定义
float Deviation_Calculation(void);
float Two_ADC_Calculation(void);
//舵机函数
void SW_Judge_Servo(void);
void Motor_Control_Servo(void);
void Island_Judge_Servo(void);
void Hill_Judge_Servo(void);
void Normal_Judge_Servo(void);
//电机函数
void SW_Judge_Motor(void);
void Motor_Control_Motor(void);
void Island_Judge_Motor(void);
void Hill_Judge_Motor(void);
void Normal_Judge_Motor(void);
#endif 

/* end of file cppyright reserve by team of yanjun ,More information please browse www.yanjun.tech */
