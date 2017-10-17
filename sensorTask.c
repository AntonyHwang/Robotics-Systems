/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>

/* Example/Board Header files */
#include "Board.h"

#define TASKSTACKSIZE       640

Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];

/*
 *  ======== taskFxn ========
 *  Task for this function is created statically. See the project's .cfg file.
 */
Void sensorTaskFxn(UArg arg0, UArg arg1)
{
    // Locals
    I2C_Handle left_handle;
    I2C_Handle right_handle;
    I2C_Params params;
    I2C_Transaction i2cTrans;
    uint8_t         txBuffer[1];
    uint8_t         rxBuffer[1];

    // Configure I2C parameters.
    I2C_Params_init(&params);

    // Initialize master I2C transaction structure
    txBuffer[0] = 0x01;
    i2cTrans.writeCount   = 2;
    i2cTrans.writeBuf     = txBuffer;
    i2cTrans.readCount    = 1;
    i2cTrans.readBuf      = rxBuffer;
    i2cTrans.slaveAddress = 0x70;

    // Open I2C
    left_handle = I2C_open(Board_I2C1, &params);
    right_handle = I2C_open(Board_I2C2, &params);

    // Do I2C transfer receive
    I2C_transfer(left_handle, &i2cTrans);
    I2C_transfer(right_handle, &i2cTrans);

    // Locals
    uint16_t        distance;
    I2C_Transaction i2cSensorTrans;
    uint8_t         txSensorBuffer[1];
    uint8_t         rxSensorBuffer[1];
    txSensorBuffer[0] = 0x01;
    i2cSensorTrans.writeCount   = 2;
    i2cSensorTrans.writeBuf     = txSensorBuffer;
    i2cSensorTrans.readCount    = 0;
    i2cSensorTrans.readBuf      = rxSensorBuffer;
    i2cSensorTrans.slaveAddress = 0x29;

    while (1) {
        if (I2C_transfer(left_handle, &i2cSensorTrans)) {
            distance = rxBuffer[1];
            System_printf("Sample %u: left %d (C)\n", i, distance);
            System_printf("Left Handle Successful\n");
        }
        else {
            System_printf("I2C Bus fault\n");
        }
        if (I2C_transfer(right_handle, &i2cSensorTrans)) {
            distance = rxBuffer[1];
            System_printf("Sample %u: right %d (C)\n", i, distance);
            System_printf("Right Handle Successful\n");
        }
        else {
            System_printf("I2C Bus fault\n");
        }

        System_flush();
        Task_sleep(1000);
    }
    /* Deinitialized I2C */
    I2C_close(left_handle);
    I2C_close(right_handle);
    System_printf("I2C closed!\n");

    System_flush();

//    unsigned int    i;
//    uint16_t        distance;
//    uint8_t         txBuffer[1];
//    uint8_t         rxBuffer[2];
//    I2C_Handle      i2c;
//    I2C_Handle      i2cLeft;
//    I2C_Params      i2cParams;
//    I2C_Params      i2cParamsLeft;
//    I2C_Transaction i2cTransaction;
//
//    /* Create I2C for usage */
//    I2C_Params_init(&i2cParams);
//    I2C_Params_init(&i2cParamsLeft);
//    i2cParams.bitRate = I2C_400kHz;
//    i2cParamsLeft.bitRate = I2C_400kHz;
//    i2c = I2C_open(Board_I2C1, &i2cParams);
//    i2cLeft = I2C_open(Board_I2C2, &i2cParamsLeft);
//    if (i2c == NULL || i2cLeft == NULL) {
//        System_abort("Error Initializing I2C\n");
//    }
//    else {
//        System_printf("I2C Initialized!\n");
//    }
//
//    /* Point to the T ambient register and read its 2 bytes */
//    txBuffer[0] = 0x01;
//
//    i2cTransaction.slaveAddress = (0x70);
//    i2cTransaction.writeBuf = txBuffer;
//    i2cTransaction.writeCount = 0;
//    i2cTransaction.readBuf = rxBuffer;
//    i2cTransaction.readCount = 0;
//
//    /* Take 20 samples and print them out onto the console */
//    for (i = 0;i < 20; i++) {
//        if (I2C_transfer(i2c, &i2cTransaction)) {
//            System_printf("Successful\n");
//        }
//        else {
//            System_printf("I2C Bus fault\n");
//        }
//
//        System_flush();
//        Task_sleep(1000);
//    }
//    /* Deinitialized I2C */
//    I2C_close(i2c);
//    I2C_close(i2cLeft);
//    System_printf("I2C closed!\n");
//
//    System_flush();
}
