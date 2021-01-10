/*
 * @Descripttion: 小车底盘驱动
 * @version: 1.0
 * @Author: hxk
 * @Date: 2020-09-18 19:48:22
 * @LastEditors: hxk
 * @LastEditTime: 2020-10-07 19:49:15
 */
#ifndef __CONTROL_H
#define __CONTROL_H

#include "main.h"


extern int count_time;

extern float Position_KP;
extern float Position_KI;
extern float Position_KD;
extern float Velocity_KP;
extern float Velocity_KI;


extern int speed_tar_LF;
extern int speed_tar_RF;
extern int speed_tar_LB;
extern int speed_tar_RB;

extern int speed_now_LF;
extern int speed_now_RF;
extern int speed_now_LB;
extern int speed_now_RB;

extern int PWM_LF;
extern int PWM_RF;
extern int PWM_LB;
extern int PWM_RB;

extern int speed;



void PWM_Init(void);
void Encoder_Init(void);
void Remoter(int speed);
int Position_PID(int Encoder,int Target);
int Incremental_PI (int Encoder,int Target);
void PID_Calculate(void);
int ReadEncoder_LF(void);
int ReadEncoder_RF(void);
int ReadEncoder_LB(void);
int ReadEncoder_RB(void);
void ReadEncoder(void);
void Motor_LF(int pwm);
void Motor_RF(int pwm);
void Motor_LB(int pwm);
void Motor_RB(int pwm);
void Motor(void);
int limit_pwm(int pwm);
void SendData(void);


#endif

