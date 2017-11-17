/*

 * wallFollower.c
 *
 *  Created on: 13 Nov 2017
 *      Author: Jingwen
 *
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

int wallDist = 100;
int r_corner, l_corner, r_front, l_front, r_f_side, l_f_side, r_b_side, l_b_side;
float PID_val;
//float b_PID_val;
float base_speed = 10.0;
extern float setPointR, setPointL;
extern int dirR;
extern int dirL;

int wall_dist = 150;
int T, T_minus, deltaT;

float P = 0.20;
float I = 0.04;
float D = 0.07;
float integ;
float b_integ;
float last_error;
float b_last_error;
float error;
float b_error;
float diff;
float b_diff;

void calc_sensor_pid(int setPoint) {
    //r_f right wheel; r_b left wheel
    T = Clock_getTicks();
    deltaT = T - T_minus;
    error = setPoint - r_f_side;
    if (error >= -5 && error <= 5) {
        error = 0;
    }
    integ = integ + (error * deltaT / 1000.0) / 255.0;
    //b_integ = b_integ + b_error * deltaT / 1000.0;
    diff = (error - last_error) / deltaT * 1000.0;
    //diff = (b_error - b_last_error) / deltaT * 1000.0;
    last_error = error;
    //b_last_error = b_error;
    PID_val = P * error + I * integ + D * diff;
    //b_PID_val = P * b_error + I * b_integ + D * b_diff;
    T_minus = T;
}

void readSensor() {
    r_corner = distR[1];
    l_corner = distL[2];
    r_front = distR[2];
    l_front = distL[1];
    r_f_side = distR[0];
    l_f_side = distL[3];
    r_b_side = distR[3];
    l_b_side = distL[0];
}

int checkRightSide(void)
{
    if (r_front >= 150 && l_front >= 150) {
        if (r_f_side >= 240 && r_b_side >= 240) {
            return 1; // Initial condition, detecting walls
        }
        else {
            return 3; // walking along the wall, use PID
        }
    }
    else if (r_front < 150 || l_front < 150) {
            return 2; // wall immediately in front, turn left
    }
    else {
        return 4;
    }
}

void wallFollow(UArg arg0, UArg arg1) {
    /*int frontDistDiff = 0;

    double avgDistL = (distL[3] + distL[0]) / 2.0;
    double avgDistR = (distR[3] + distR[0]) / 2.0;*/
    T_minus = Clock_getTicks();
    last_error = 0;
    last_error = 0;
    while(1) {
        readSensor();
        if (checkRightSide() == 1) {
            setPointR = base_speed;
            setPointL = base_speed;
            dirR = 1;
            dirL = 1;
        }
        else if (checkRightSide() == 2){
            setPointR = base_speed / 3;
            setPointL = base_speed / 3;
            dirR = 1;
            dirL = 2;
        }
        else if (checkRightSide() == 3) {
            calc_sensor_pid(wallDist);
            setPointR = base_speed + PID_val;
            setPointL = base_speed;
            if (setPointR < 0) {
                setPointR = 0;
            }
            if (setPointR > 2 * base_speed) {
                setPointR = 2 * base_speed;
            }
            dirR = 1;
            dirL = 1;
        }
        else {
            setPointR = base_speed;
            setPointL = base_speed;
            dirR = 1;
            dirL = 1;
        }
        //difference between current distance and target distance
        Task_sleep(200);
    }
}
