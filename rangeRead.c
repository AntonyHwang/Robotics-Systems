#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>

/* Board Header files*/
#include "Board.h"
#include "buttons.h"
#include <stdio.h>

/* LCD header file */
#include "Sharp96x96.h"

#define sensor_addr 0x29

uint8_t distR[4] = {255, 255, 255, 255};
uint8_t distL[4] = {255, 255, 255, 255};

void sensor_write(I2C_Handle handle, uint16_t reg_addr, uint8_t data){
    I2C_Transaction i2c_transaction;
    uint8_t writeBuffer[3];
    writeBuffer[0] = (reg_addr >> 8) & 0xFF;
    writeBuffer[1] = reg_addr & 0xFF;
    writeBuffer[2] = data;

    i2c_transaction.slaveAddress = sensor_addr;
    i2c_transaction.writeBuf = writeBuffer;
    i2c_transaction.writeCount = 3;
    i2c_transaction.readBuf = NULL;
    i2c_transaction.readCount = 0;

    if (!I2C_transfer(handle, &i2c_transaction)) {
        System_printf("Write to sensor failure !\n");
    }

}

uint8_t sensor_read(I2C_Handle handle, uint16_t reg_addr){
    I2C_Transaction i2c_transaction;
    uint8_t writeBuffer[2], readBuffer[1];
    writeBuffer[0] = (reg_addr >> 8) & 0xFF;
    writeBuffer[1] = reg_addr & 0xFF;

    i2c_transaction.slaveAddress = sensor_addr;
    i2c_transaction.writeBuf = writeBuffer;
    i2c_transaction.writeCount = 2;
    i2c_transaction.readBuf = readBuffer;
    i2c_transaction.readCount = 1; // Read 1 byte each time

    if (!I2C_transfer(handle, &i2c_transaction)) {
        System_printf("Reading from sensor failure !\n");
    }
    return readBuffer[0];
}

void sensorInit(I2C_Handle handle){

    sensor_write(handle, 0x0207, 0x01);
    sensor_write(handle, 0x0208, 0x01);
    sensor_write(handle, 0x0096, 0x00);
    sensor_write(handle, 0x0097, 0xfd);
    sensor_write(handle, 0x00e3, 0x00);
    sensor_write(handle, 0x00e4, 0x04);
    sensor_write(handle, 0x00e5, 0x02);
    sensor_write(handle, 0x00e6, 0x01);
    sensor_write(handle, 0x00e7, 0x03);
    sensor_write(handle, 0x00f5, 0x02);
    sensor_write(handle, 0x00d9, 0x05);
    sensor_write(handle, 0x00db, 0xce);
    sensor_write(handle, 0x00dc, 0x03);
    sensor_write(handle, 0x00dd, 0xf8);
    sensor_write(handle, 0x009f, 0x00);
    sensor_write(handle, 0x00a3, 0x3c);
    sensor_write(handle, 0x00b7, 0x00);
    sensor_write(handle, 0x00bb, 0x3c);
    sensor_write(handle, 0x00b2, 0x09);
    sensor_write(handle, 0x00ca, 0x09);
    sensor_write(handle, 0x0198, 0x01);
    sensor_write(handle, 0x01b0, 0x17);
    sensor_write(handle, 0x01ad, 0x00);
    sensor_write(handle, 0x00ff, 0x05);
    sensor_write(handle, 0x0100, 0x05);
    sensor_write(handle, 0x0199, 0x05);
    sensor_write(handle, 0x01a6, 0x1b);
    sensor_write(handle, 0x01ac, 0x3e);
    sensor_write(handle, 0x01a7, 0x1f);
    sensor_write(handle, 0x0030, 0x00);

}

void switchOpen(I2C_Handle handle, uint8_t channel){
    I2C_Transaction i2c_transaction_r1;
    uint8_t txBuffer[1]; // transmitter buffer
    /* First transaction. Slave is the I2C switch */
    txBuffer[0] = 0xFF & channel;
    i2c_transaction_r1.slaveAddress = 0x70;
    i2c_transaction_r1.writeBuf = txBuffer;
    i2c_transaction_r1.writeCount = 1; // Number of bytes to be written to the switch
    i2c_transaction_r1.readBuf = NULL;
    i2c_transaction_r1.readCount = 0;

    if (I2C_transfer(handle, &i2c_transaction_r1)) {
        System_printf("Open channel %d\n", channel);
    }
    else {
        System_printf("I2C Bus fault\n");
    }
}

void readResult(I2C_Handle handle, uint8_t *data) {
    switchOpen(handle, 0x01);
    sensor_write(handle, 0x0018, 0x01);
    while (sensor_read(handle, 0x004f) & 0x04 == 0)
        Task_sleep(2);
    data[0] = sensor_read(handle, 0x0062);
    //System_printf("Data from channel %d: %d\n", channel, distance);
    sensor_write(handle, 0x0015, 0x07);

    switchOpen(handle, 0x02);
    sensor_write(handle, 0x0018, 0x01);
    while (sensor_read(handle, 0x004f) & 0x04 == 0)
        Task_sleep(2);
    data[1] = sensor_read(handle, 0x0062);
    //System_printf("Data from channel %d: %d\n", channel, distance);
    sensor_write(handle, 0x0015, 0x07);

    switchOpen(handle, 0x04);
    sensor_write(handle, 0x0018, 0x01);
    while (sensor_read(handle, 0x004f) & 0x04 == 0)
        Task_sleep(2);
    data[2] = sensor_read(handle, 0x0062);
    //System_printf("Data from channel %d: %d\n", channel, distance);
    sensor_write(handle, 0x0015, 0x07);

    switchOpen(handle, 0x08);
    sensor_write(handle, 0x0018, 0x01);
    while (sensor_read(handle, 0x004f) & 0x04 == 0)
        Task_sleep(2);
    data[3] = sensor_read(handle, 0x0062);
    //System_printf("Data from channel %d: %d\n", channel, distance);
    sensor_write(handle, 0x0015, 0x07);
}

Void readRangeFxn(UArg arg0, UArg arg1)
{
    uint8_t i;
    uint8_t channel = 1;
    I2C_Handle i2cR, i2cL;
    I2C_Params i2cParamsR, i2cParamsL;
    I2C_Params_init(&i2cParamsR);
    i2cParamsR.bitRate = I2C_400kHz;
    I2C_Params_init(&i2cParamsL);
    i2cParamsL.bitRate = I2C_400kHz;

    i2cR = I2C_open(Board_I2C1, &i2cParamsR); // Create I2C bus for RHS sensor
    if (i2cR == NULL) {
        System_abort("Error Initializing I2C\n");
    }
    i2cL = I2C_open(Board_I2C2, &i2cParamsL); // Create I2C bus for RHS sensor
    if (i2cL == NULL) {
        System_abort("Error Initializing I2C\n");
    }

    for (i = 0; i < 4; i++) {
        switchOpen(i2cR, channel);
        switchOpen(i2cL, channel);
        sensorInit(i2cR);
        sensorInit(i2cL);
        channel = channel << 1;
    }

    channel = 1;

    /* Take 20 samples and print them out onto the console */
    while (1) {
        /*switchOpen(i2cR, channel);
        sensor_write(i2cR, 0x0018, 0x01);
        while (sensor_read(i2cR, 0x004f) & 0x04 == 0)
            Task_sleep(5);
        distR = sensor_read(i2cR, 0x0062);
        //System_printf("Right channel %d: %d\n", channel, distR);
        sensor_write(i2cR, 0x0015, 0x07);

        switchOpen(i2cL, channel);
        sensor_write(i2cL, 0x0018, 0x01);
        while (sensor_read(i2cL, 0x004f) & 0x04 == 0)
            Task_sleep(5);
        distL = sensor_read(i2cL, 0x0062);
        //System_printf("Left channel %d: %d\n", channel, distL);
        sensor_write(i2cL, 0x0015, 0x07);

        if (channel != 0x08) {
            channel = channel << 1;
        }
        else {
            channel = 0x01;
        }*/
        readResult(i2cR, distR);
        readResult(i2cL, distL);
        System_flush();
        Task_sleep(30);
    }

    /* Deinitialized I2C */
    I2C_close(i2cR);
    System_printf("I2C closed!\n");
    System_flush();
}
