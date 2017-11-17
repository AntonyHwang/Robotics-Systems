/*
 * screenTask.c
 *
 *  Created on: 9 Oct 2016
 *      Author: shailes
 */

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>

/* Board Header files */
#include "Sharp96x96.h"
#include <stdio.h>

extern uint8_t distR[4], distL[4];
extern float PID_val;
extern float integ;
extern float speedR, speedL;
extern float errR, errL, integR, integL;
extern float setPointR, setPointL;

/*
 *  screenTaskFxn
 */
void screenTaskFxn (UArg arg0, UArg arg1)
{
	Graphics_Context g_sContext;

	char test1_string[20];
    char test2_string[20];

    const uint8_t LHS =  10;
    const uint8_t RHS = 210;
    uint8_t y;

    Sharp96x96_LCDInit();

    Graphics_initContext(&g_sContext, &g_sharp96x96LCD);
    Graphics_setForegroundColor(&g_sContext, ClrBlack);
    Graphics_setBackgroundColor(&g_sContext, ClrWhite);
    Graphics_setFont(&g_sContext, &g_sFontCm20b);
    Graphics_clearDisplay(&g_sContext);
    Graphics_flushBuffer(&g_sContext);

    while (1) {
        Graphics_clearBuffer(&g_sContext);
    	y = 0;

    	sprintf(test1_string,"L1 = %d", distL[0]) ;
        Graphics_drawString(&g_sContext, test1_string, GRAPHICS_AUTO_STRING_LENGTH, LHS, y, GRAPHICS_TRANSPARENT_TEXT);
    	sprintf(test2_string,"R1 = %d", distR[0]) ;
		Graphics_drawString(&g_sContext, test2_string, GRAPHICS_AUTO_STRING_LENGTH, RHS, y, GRAPHICS_TRANSPARENT_TEXT);
    	y += 20;

    	sprintf(test1_string,"L2 = %d", distL[1]) ;
    	Graphics_drawString(&g_sContext, test1_string, GRAPHICS_AUTO_STRING_LENGTH, LHS, y, GRAPHICS_TRANSPARENT_TEXT);
        sprintf(test2_string,"R2 = %d", distR[1]) ;
    	Graphics_drawString(&g_sContext, test2_string, GRAPHICS_AUTO_STRING_LENGTH, RHS, y, GRAPHICS_TRANSPARENT_TEXT);
    	y += 20;

    	sprintf(test1_string,"L3 = %d", distL[2]) ;
    	Graphics_drawString(&g_sContext, test1_string, GRAPHICS_AUTO_STRING_LENGTH, LHS, y, GRAPHICS_TRANSPARENT_TEXT);
        sprintf(test2_string,"R3 = %d", distR[2]) ;
        Graphics_drawString(&g_sContext, test2_string, GRAPHICS_AUTO_STRING_LENGTH, RHS, y, GRAPHICS_TRANSPARENT_TEXT);
        y += 20;

        sprintf(test1_string,"L4 = %d", distL[3]) ;
        Graphics_drawString(&g_sContext, test1_string, GRAPHICS_AUTO_STRING_LENGTH, LHS, y, GRAPHICS_TRANSPARENT_TEXT);
        sprintf(test2_string,"R4 = %d", distR[3]) ;
        Graphics_drawString(&g_sContext, test2_string, GRAPHICS_AUTO_STRING_LENGTH, RHS, y, GRAPHICS_TRANSPARENT_TEXT);
        y += 20;

        sprintf(test1_string,"Speed R = %.4f", speedR) ;
        Graphics_drawString(&g_sContext, test1_string, GRAPHICS_AUTO_STRING_LENGTH, LHS, y, GRAPHICS_TRANSPARENT_TEXT);
        sprintf(test2_string,"Speed L = %.4f", speedL) ;
        Graphics_drawString(&g_sContext, test2_string, GRAPHICS_AUTO_STRING_LENGTH, RHS, y, GRAPHICS_TRANSPARENT_TEXT);
        y += 20;

        sprintf(test1_string,"setpoint R = %.4f", setPointR) ;
        Graphics_drawString(&g_sContext, test1_string, GRAPHICS_AUTO_STRING_LENGTH, LHS, y, GRAPHICS_TRANSPARENT_TEXT);
        sprintf(test1_string,"setpoint L = %.4f", setPointL) ;
        Graphics_drawString(&g_sContext, test1_string, GRAPHICS_AUTO_STRING_LENGTH, RHS, y, GRAPHICS_TRANSPARENT_TEXT);
		y += 20;

		sprintf(test1_string,"errR = %.4f", errR) ;
		Graphics_drawString(&g_sContext, test1_string, GRAPHICS_AUTO_STRING_LENGTH, LHS, y, GRAPHICS_TRANSPARENT_TEXT);
		sprintf(test2_string,"errL = %.4f", errL) ;
		Graphics_drawString(&g_sContext, test2_string, GRAPHICS_AUTO_STRING_LENGTH, RHS, y, GRAPHICS_TRANSPARENT_TEXT);
		y += 20;

		sprintf(test1_string,"integ = %.4f", integ) ;
		Graphics_drawString(&g_sContext, test1_string, GRAPHICS_AUTO_STRING_LENGTH, LHS, y, GRAPHICS_TRANSPARENT_TEXT);
		sprintf(test2_string,"PID = %.4f", PID_val) ;
		Graphics_drawString(&g_sContext, test2_string, GRAPHICS_AUTO_STRING_LENGTH, RHS, y, GRAPHICS_TRANSPARENT_TEXT);
		y += 20;

        Graphics_flushBuffer(&g_sContext);

    	Task_sleep((UInt)arg0);
    }
}

