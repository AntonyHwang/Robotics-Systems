/*
 *  ======== main.c ========
 */
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>

/* Board Header files */
#include "Board.h"
#include "buttons.h"
#include "motor.h"
#include "robot.h"
#include <stdio.h>

/*
 *  ======== global system state variables ========
 */
uint8_t	state = 1;

/*
 *  ======== main ========
 */
int main(void)
{   /* Call board init functions */
    Board_initGeneral();
    Board_initGPIO();
    Board_initPWM();
    Board_initI2C();

    GPIO_setCallback(Board_BUTTON0, gpioButtonFxn0);
    GPIO_enableInt(Board_BUTTON0);
    GPIO_setCallback(Board_BUTTON1, gpioButtonFxn1);
    GPIO_enableInt(Board_BUTTON1);
    GPIO_write(MSP_EXP432P401R_P5_2, 1);
    GPIO_write(MSP_EXP432P401R_P4_6, 1);
    GPIO_write(MSP_EXP432P401R_P5_2, 1);
    GPIO_write(MSP_EXP432P401R_P4_6, 1);
    GPIO_write(MSP_EXP432P401R_P3_0, 1);//motor nsleep open

    GPIO_setCallback(Robot_MotorRIN1f, RHMotorChannelAFallingFxn);
    GPIO_enableInt(Robot_MotorRIN1f);
    GPIO_setCallback(Robot_MotorLIN1f, LHMotorChannelAFallingFxn);
    GPIO_enableInt(Robot_MotorLIN1f);
    // Switch on the LEDs
    //
    //GPIO_write(Board_LED0, Board_LED_ON);	// LH LED
    //GPIO_write(Board_LED1, Board_LED_ON);	// RH LED, Red
    //GPIO_write(Board_LED2, Board_LED_ON);	// RH LED, Green
    //GPIO_write(Board_LED3, Board_LED_ON);	// RH LED, Blue

    System_printf("Starting the example\nSystem provider is set to SysMin. "
                  "Halt the target to view any SysMin contents in ROV.\n");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

    /* Start BIOS */
    BIOS_start();
    Task_sleep(2000);

    return (0);
}
