/*
 * motor.h
 *
 *  Created on: 2017Äê10ÔÂ27ÈÕ
 *      Author: jingwen
 */

#include <ti/drivers/PWM.h>


#ifndef MOTOR_H_
#define MOTOR_H_
void RHMotorChannelAFallingFxn();
void LHMotorChannelAFallingFxn();
void speedPID(float setPointR, float setPointL);
extern PWM_Handle pwm1;
extern PWM_Handle pwm2;
extern PWM_Handle pwm3;
extern PWM_Handle pwm4;
extern PWM_Params params;
extern uint16_t   pwmPeriod;      // Period and duty in microseconds
extern uint16_t   dutyR;
extern uint16_t   dutyL;
extern uint16_t   dutyIncR, dutyIncL;
extern uint32_t preT, currT, dT;
extern float preRotatesR, currRotatesR, preRotatesL, currRotatesL;
extern float setPoint, errR, errL, integR, integL;
extern float diffR, diffL, prev_errR, prev_errL;

extern uint8_t distR[4];
extern uint8_t distL[4];
extern int Rotation_R;
extern int Rotation_L;
extern double speedR, speedL;
extern double dist;


#endif /* MOTOR_H_ */

