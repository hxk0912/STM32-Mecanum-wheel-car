/*
 * @Descripttion:  小车底盘驱动文件
 * @version: 1.0
 * @Author: hxk
 * @Date: 2020-09-18 19:48:05
 * @LastEditors: hxk
 * @LastEditTime: 2020-10-07 21:56:47
 */


#include "control.h"
#include "gpio.h"
#include "tim.h"
#include "main.h"
#include "protocol.h"

int count_time = 0;


//记录PID系数

float Position_KP = 5;
float Position_KI = 0.1;
float Position_KD =0;
float Velocity_KP = 1;    
float Velocity_KI = 0.05;

//电机目标速度

int speed_tar_LF = 0;
int speed_tar_RF = 0;
int speed_tar_LB = 0;
int speed_tar_RB = 0;


// 记录电机实际速度

int speed_now_LF = 0;
int speed_now_RF = 0;
int speed_now_LB = 0;
int speed_now_RB = 0;

// 记录电机控制输出量

int PWM_LF = 0;
int PWM_RF = 0;
int PWM_LB = 0;
int PWM_RB = 0;

int speed = 0;


/**
 * @name: PWM_Init
 * @msg: 初始化PWM输出
 * @param {void} 
 * @return {void} 
 */

void PWM_Init(void)
{
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_4);
}

/**
 * @name: Encoder_Init
 * @msg: 初始化编码器读取，并将每个编码器通道的初始值设为10000防止溢出
 * @param {void} 
 * @return {void} 
 */

void Encoder_Init(void)
{
	HAL_TIM_Encoder_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&htim1,TIM_CHANNEL_2);
	__HAL_TIM_SET_COUNTER(&htim1,10000);
	HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_2);
	__HAL_TIM_SET_COUNTER(&htim2,10000);
	HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_2);
	__HAL_TIM_SET_COUNTER(&htim3,10000);
	HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_2);
	__HAL_TIM_SET_COUNTER(&htim4,10000);
}

/**
 * @name: Remoter 
 * @msg: 控制小车前进,更改各个轮子的目标值
 * @param {int speed} 
 * @return {void} 
 */

void Remoter(int speed)
{
	speed_tar_LF = speed;
	speed_tar_RF = speed;
	speed_tar_LB = speed;
	speed_tar_RB = speed;
	
}

/**
 * @name: Motor
 * @msg: 驱动函数，将各个轮子的PWM值赋给各个轮子的控制函数
 * @param {void} 
 * @return {void} 
 */

void Motor(void)
{
	Motor_LF(PWM_LF);
	Motor_RF(PWM_RF);
	Motor_LB(PWM_LB);
	Motor_RB(PWM_RB);
}

int myabs(int num)
{
	if(num<0)
	{
		num = -num;
	}
	return num;
}

/**
 * @name: Position_PID
 * @msg: 位置式PID
 * @param {int Encoder,int Target} 
 * @return {int Pwm} 
 */

int Position_PID(int Encoder,int Target)
{ 	
	static int Bias,Pwm,Integral_bias,Last_Bias;
	Bias=Target - Encoder;                                  
	Integral_bias+=Bias;	                                 
	Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias); 
	Last_Bias=Bias;                                       
	return Pwm;                                           
}

/**
 * @name: Incremental_PI
 * @msg: 位置式PI
 * @param {int Encoder,int Target} 
 * @return {int Pwm} 
 */

int Incremental_PI (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Last_bias;
	 Bias=Target-Encoder;                           
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;  
	 Last_bias=Bias;	                                     
	 return Pwm;                                    
}

/**
 * @name: PID_Calculate
 * @msg: 调用各个轮子的pid函数，得出运算后的PWM值
 * @param {void} 
 * @return {void} 
 */

void PID_Calculate(void)
{
	PWM_LF = Position_PID(speed_now_LF,speed_tar_LF);
	PWM_RF = Position_PID(speed_now_RF,speed_tar_RF);
	PWM_LB = Position_PID(speed_now_LB,speed_tar_LB);
	PWM_RB = Position_PID(speed_now_RB,speed_tar_RB);
	PWM_LF = limit_pwm(PWM_LF);
	PWM_RF = limit_pwm(PWM_RF);
	PWM_LB = limit_pwm(PWM_LB);
	PWM_RB = limit_pwm(PWM_RB);
}

/**
 * @name: ReadEncoder 
 * @msg: 读取编码器的值，并将编码器初始值置回10000，用于速度计算
 * @param {void} 
 * @return {1} 
 */

int ReadEncoder_LF(void)
{
	speed_now_LF = -(__HAL_TIM_GET_COUNTER(&htim1)-10000);
	__HAL_TIM_SET_COUNTER(&htim1,10000);
	return 1;
}

int ReadEncoder_RF(void)
{
	speed_now_RF = (__HAL_TIM_GET_COUNTER(&htim2)-10000);
	__HAL_TIM_SET_COUNTER(&htim2,10000);
	return 1;
}

int ReadEncoder_LB(void)
{
	speed_now_LB = -(__HAL_TIM_GET_COUNTER(&htim3)-10000);
	__HAL_TIM_SET_COUNTER(&htim3,10000);
	return 1;
}

int ReadEncoder_RB(void)
{
	speed_now_RB = (__HAL_TIM_GET_COUNTER(&htim4)-10000);
	__HAL_TIM_SET_COUNTER(&htim4,10000);
	return 1;
}

/**
 * @name: ReadEncoder 
 * @msg: 读取各个编码器的值，保存到记录速度变量中
 * @param {void} 
 * @return {void} 
 */

void ReadEncoder(void)
{
	ReadEncoder_LF();
	ReadEncoder_RF();
	ReadEncoder_LB();
	ReadEncoder_RB();
}

/**
 * @name: Motor
 * @msg: 驱动电机，pwm输入正数正转，输入负数反转
 * @param {int pwm} 
 * @return {void} 
 */

void Motor_LF(int pwm)
{   
    if (pwm>0)
    {
      	HAL_GPIO_WritePin(LF_IN1_GPIO_Port,LF_IN1_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LF_IN2_GPIO_Port,LF_IN2_Pin,GPIO_PIN_SET);
      	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,pwm);
    }else
    {
      	HAL_GPIO_WritePin(LF_IN1_GPIO_Port,LF_IN1_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(LF_IN2_GPIO_Port,LF_IN2_Pin,GPIO_PIN_RESET);
      	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,-pwm);
    }   
}
void Motor_RF(int pwm)
{   
    if (pwm>0)
    {
      	HAL_GPIO_WritePin(RF_IN1_GPIO_Port,RF_IN1_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(RF_IN2_GPIO_Port,RF_IN2_Pin,GPIO_PIN_RESET);
      	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_2,pwm);
    }else
    {
      	HAL_GPIO_WritePin(RF_IN1_GPIO_Port,RF_IN1_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(RF_IN2_GPIO_Port,RF_IN2_Pin,GPIO_PIN_SET);
      	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_2,-pwm);
    }   
}

void Motor_LB(int pwm)
{   
    if (pwm>0)
    { 
      	HAL_GPIO_WritePin(LB_IN1_GPIO_Port,LB_IN1_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(LB_IN2_GPIO_Port,LB_IN2_Pin,GPIO_PIN_RESET);
      	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_3,pwm);
    }else
    {
      	HAL_GPIO_WritePin(LB_IN1_GPIO_Port,LB_IN1_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LB_IN2_GPIO_Port,LB_IN2_Pin,GPIO_PIN_SET);
      	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_3,-pwm);
    }   
}
void Motor_RB(int pwm)
{   
    if (pwm>0)
    {
      	HAL_GPIO_WritePin(RB_IN1_GPIO_Port,RB_IN1_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(RB_IN2_GPIO_Port,RB_IN2_Pin,GPIO_PIN_SET);
      	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_4,pwm);
    }else
    {
      	HAL_GPIO_WritePin(RB_IN1_GPIO_Port,RB_IN1_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(RB_IN2_GPIO_Port,RB_IN2_Pin,GPIO_PIN_RESET);
      	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_4,-pwm);
    }   
}

/**
 * @name: limit_pwm 
 * @msg: pwm限幅，这里的幅值是+-1000
 * @param {int pwm} 
 * @return {int pwm} 
 */

int limit_pwm(int pwm)
{
  if (pwm>=0)
  {
    if (pwm>=1000)
    {
      pwm = 1000;
    }
    
  }
  else if (pwm<=0)
  {
    if (pwm<-1000)
    {
      pwm = -1000;
    }
    
  }
  return pwm;
}

/**
 * @name: SendData 
 * @msg: 发送数据到上位机四个通道，包括实际速度和目标速度
 * @param {void} 
 * @return {void} 
 */

void SendData(void)
{
	set_computer_value(SEND_FACT_CMD, CURVES_CH1,(int32_t *)&speed_now_LF,1);  
	set_computer_value(SEND_FACT_CMD, CURVES_CH2,(int32_t *)&speed_now_RF,1); 
	set_computer_value(SEND_FACT_CMD, CURVES_CH3,(int32_t *)&speed_now_LB,1);  
	set_computer_value(SEND_FACT_CMD, CURVES_CH4,(int32_t *)&speed_now_RB,1); 
	set_computer_value(SEND_TARGET_CMD, CURVES_CH1,(int32_t *)&speed_tar_LF,1);  
	set_computer_value(SEND_TARGET_CMD, CURVES_CH2,(int32_t *)&speed_tar_RF,1); 
	set_computer_value(SEND_TARGET_CMD, CURVES_CH3,(int32_t *)&speed_tar_LB,1);  
	set_computer_value(SEND_TARGET_CMD, CURVES_CH4,(int32_t *)&speed_tar_RB,1); 
}