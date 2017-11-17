/*
 * motor.c
 *
 *  Created on: 2017Äê10ÔÂ27ÈÕ
 *      Author: jingwen
 */
/* XDCtools Header files */
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/cfg/global.h>
/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/PWM.h>

/* Example/Board Header files */
#include "Board.h"

PWM_Handle pwm1;
PWM_Handle pwm2;
PWM_Handle pwm3;
PWM_Handle pwm4;
PWM_Params params;
uint16_t   pwmPeriod = 3000;      // Period and duty in microseconds
int   dutyR = 1500;
int   dutyL = 1500;
int   dutyIncR, dutyIncL;
int preT, currT, dT;
float preRotatesR, currRotatesR, preRotatesL, currRotatesL;
float setPointR = 8.0, setPointL = 8.0, errR, errL, integR, integL;
float diffR, diffL, prev_errR, prev_errL;
float speedR, speedL;
int Rotation_R=0;
int Rotation_L=0;
int dirR = 1;
int dirL = 1;

extern uint8_t distR[4];
extern uint8_t distL[4];
// P=10, I = 0.42, D = 0.10
float Kp = 10;
float Ti = 0.42;
float Td = 0.10;
float integL = 0;
float K = 39.7887;

void RHMotorChannelAFallingFxn()
{
    Rotation_R++;
}

void LHMotorChannelAFallingFxn()
{
    Rotation_L++;
}

void init_motors() {
    PWM_Params_init(&params);
    params.dutyUnits = PWM_DUTY_US;
    params.dutyValue = 0;
    params.periodUnits = PWM_PERIOD_US;
    params.periodValue = pwmPeriod;

    pwm1 = PWM_open(Board_PWM0, &params);
    pwm2 = PWM_open(Board_PWM1, &params);
    pwm3 = PWM_open(Board_PWM2, &params);
    pwm4 = PWM_open(Board_PWM3, &params);
    if (pwm1 == NULL || pwm2 == NULL || pwm3 == NULL || pwm4 == NULL) {
        System_abort("Board_PWM did not open");
    }
    else {
        PWM_start(pwm1);
        PWM_start(pwm2);
        PWM_start(pwm3);
        PWM_start(pwm4);
    }
    PWM_setDuty(pwm1, 0);
    PWM_setDuty(pwm2, 0);
    PWM_setDuty(pwm3, 0);
    PWM_setDuty(pwm4, 0);
}

void speedPID(float setPointR, float setPointL) {
    currT = Clock_getTicks();
    dT = currT - preT;
    currRotatesR = Rotation_R;
    currRotatesL = Rotation_L;
    speedR = (currRotatesR - preRotatesR) / dT * 1000;
    speedR = speedR / K;
    speedL = (currRotatesL - preRotatesL) / dT * 1000;
    speedL = speedL / K;

    if (!isnan(setPointR) && !isnan(setPointL)) {
        errR = setPointR - speedR;
        errL = setPointL - speedL;
        integR = integR + errR * dT / 1000;
        integL = integL + errL * dT / 1000;
        diffR = (errR - prev_errR) / dT * 1000;
        diffL = (errL - prev_errL) / dT * 1000;
    }
    dutyIncR = (int)(Kp * (errR + integR / Ti + Td * diffR));
    dutyIncL = (int)(Kp * (errL + integL / Ti + Td * diffL));
    dutyR += dutyIncR;
    dutyL += dutyIncL;

    if (dutyR > 3000) {
        dutyR = 3000;
    }
    if (dutyL > 3000) {
        dutyL = 3000;
    }
    if (dutyR < 0) {
       dutyR = 0;
    }
    if (dutyL < 0) {
       dutyL = 0;
    }

    preRotatesR = currRotatesR;
    preRotatesL = currRotatesL;
    prev_errR = errR;
    prev_errL = errL;
    preT = currT;
    System_flush();
    Task_sleep(20);
}

void setMotorR(int dutyR, int dir) {
    if (dir == 1) {
        PWM_setDuty(pwm2, dutyR);
        PWM_setDuty(pwm1, 0);
    }
    else {
        PWM_setDuty(pwm2, 0);
        PWM_setDuty(pwm1, dutyR);
    }
}

void setMotorL(int dutyL, int dir) {
    if (dir == 1) {
        PWM_setDuty(pwm4, dutyL);
        PWM_setDuty(pwm3, 0);
    }
    else {
        PWM_setDuty(pwm4, 0);
        PWM_setDuty(pwm3, dutyL);
    }
}

void MoveStraight(UArg arg0, UArg arg1)
{
    //Task_sleep(2000);
    init_motors();
    /* Loop forever incrementing the PWM duty */
    preT = Clock_getTicks();
    preRotatesR = 0;
    preRotatesL = 0;
    Task_sleep(20);
    prev_errR = 0;
    prev_errL = 0;
    while (1)
    {
        speedPID(setPointR, setPointL);
        setMotorR(dutyR, dirR);
        setMotorL(dutyL, dirL);
    }
}

