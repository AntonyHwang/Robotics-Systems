/*
 * deadReckon.c
 *
 *  Created on: 17 Nov 2017
 *      Author: Jingwen
 */

#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/cfg/global.h>
// BIOS Header files
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <time.h>
#include <math.h>
//
// TI-RTOS Header files
#include <ti/drivers/GPIO.h>
#include <ti/drivers/PWM.h>

/* Example/Board Header files */
#include "Board.h"
#include "motor.h"

extern int Rotation_R;
extern int Rotation_L;
extern float setPointR;
extern float setPointL;
float x = 200;
float y = 200;
float deltaL = 14.13717;
float L1, L2;

void turn90degree() {
    int DeltaL, R0, L0, dR, dL;
    DeltaL = (int) (deltaL * K);
    R0 = Rotation_R;
    L0 = Rotation_L;
    do {
        setPointR = 10.0 + 2.0;
        setPointL = 10.0 - 2.0;
        dR = Rotation_R - R0;
        dL = Rotation_L - L0;
        Task_sleep(10);
    }
    while ((dR - dL) <= DeltaL);
}

void goStraight(float distance) {
    int Rotates, MaxR;
    MaxR = (int) (distance * K);
    do {
        setPointR = 10.0;
        setPointL = 10.0;
        Rotates = (int) ((Rotation_R + Rotation_L) / 2.0);
        Task_sleep(10);
    }
    while (Rotates <= MaxR);
}

void deadReckonFxn(UArg arg0, UArg arg1) {
    goStraight(50);
    turn90degree();
    setPointR = 0.0;
    setPointL = 0.0;
}

