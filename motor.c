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

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/PWM.h>

/* Example/Board Header files */
#include "Board.h"


extern uint8_t distR[4];
extern uint8_t distL[4];
int Rotation_R=0;
int Rotation_L=0;
float speedR, speedL;
float Kp = 30;
float Ki = 0;
float Kd = 0;


void RHMotorChannelAFallingFxn()
{
    Rotation_R++;
}

void LHMotorChannelAFallingFxn()
{
    Rotation_L++;
}
void PWM_Fxn(UArg arg0, UArg arg1)
{
    PWM_Handle pwm1;
    PWM_Handle pwm2 = NULL;
    PWM_Handle pwm3;
    PWM_Handle pwm4 = NULL;
    PWM_Params params;
    uint16_t   pwmPeriod = 3000;      // Period and duty in microseconds
    uint16_t   dutyR = 1500;
    uint16_t   dutyL = 1500;
    uint16_t   dutyIncR, dutyIncL;
    uint32_t preT, currT, dT;
    float preRotatesR, currRotatesR, preRotatesL, currRotatesL;
    float setPoint = 1000, errR, errL, integR, integL;
    float diffR, diffL, prev_errR, prev_errL;
    //float speedR;

    PWM_Params_init(&params);
    params.dutyUnits = PWM_DUTY_US;
    params.dutyValue = 0;
    params.periodUnits = PWM_PERIOD_US;
    params.periodValue = pwmPeriod;

    pwm1 = PWM_open(Board_PWM0, &params);
    if (pwm1 == NULL) {
        System_abort("Board_PWM0 did not open");
    }
    PWM_start(pwm1);

    pwm2 = PWM_open(Board_PWM1, &params);
    if (pwm2 == NULL) {
        System_abort("Board_PWM1 did not open");
    }
    PWM_start(pwm2);

    pwm3 = PWM_open(Board_PWM2, &params);
    if (pwm3 == NULL)
    {
        System_abort("Board_PWM0 did not open");
    }
    PWM_start(pwm3);

    pwm4 = PWM_open(Board_PWM3, &params);
    if (pwm4 == NULL)
    {
        System_abort("Board_PWM0 did not open");
    }
    PWM_start(pwm4);

    PWM_setDuty(pwm1, 0);
    PWM_setDuty(pwm2, dutyR);

    PWM_setDuty(pwm3, 0);
    PWM_setDuty(pwm4, dutyL);

    /* Loop forever incrementing the PWM duty */
    preT = Clock_getTicks();
    preRotatesR = 0;
    preRotatesL = 0;
    Task_sleep(50);
    prev_errR = 0;
    prev_errL = 0;
    while (1)
    {
        currT = Clock_getTicks();
        dT = currT - preT;
        currRotatesR = Rotation_R;
        currRotatesL = Rotation_L;
        speedR = (currRotatesR - preRotatesR) / dT * 1000;
        speedL = (currRotatesL - preRotatesL) / dT * 1000;
        errR = setPoint - speedR;
        errL = setPoint - speedL;
        integR = integR + errR * dT;
        integL = integL + errL * dT;
        diffR = (errR - prev_errR) / dT;
        diffL = (errL - prev_errL) / dT;
        dutyIncR = (int) (Kp * errR + Ki * integR + Kd * diffR);
        dutyIncL = (int) (Kp * errL + Ki * integL + Kd * diffL);
        dutyR += dutyIncR;
        dutyL += dutyIncL;
        PWM_setDuty(pwm2, dutyR);
        PWM_setDuty(pwm4, dutyL);

        preRotatesR = currRotatesR;
        preRotatesL = currRotatesL;
        prev_errR = errR;
        prev_errL = errL;
        preT = currT;
        Task_sleep(50);
        if ((distR[2] <= 100) || (distL[1] <= 100))
        {
            PWM_setDuty(pwm1, 3000);
            PWM_setDuty(pwm2, 3000);
            PWM_setDuty(pwm3, 3000);
            PWM_setDuty(pwm4, 3000);
            break;
        }
    }
}

