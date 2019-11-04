#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Controller_Protocol.h"
#include "communication.h"
#include "variable.h"
#include "Config.h"
#include "Init.h"

#define _XTAL_FREQ 32000000

void interrupt INTERRUPT_HANDLER();
int ChangeAddress(void);
void WriteAddress(int Address);
void WriteData(int Address);

/*UART RX*/
extern bool Receive_flag;

/*slave address*/ /**********/ //
#define SLAVE_ADDRESS 0x00
///*************************///

int main(int argc, char **argv)
{
    Initialize();

    /*初回受信*/
    while (Receive_flag != reception_complete)
        ;
    Organize_Datas(Data_from_master, Buffer_from_master, number_of_rxdata0, master_COM);
    Receive_flag = waiting_to_receive;
    /*初回受信終わり*/

    int Address;

    while (1)
    {
        /* 受信 */
        Reception_from_master_main();

        /* アドレス */
        Address = ChangeAddress();
        WriteAddress(Address);

        /* データ */
        if (Address != 0)
            WriteData(Address);

        /* デバッグ */
        LED ^= 1;

        __delay_ms(1);
    }
    return (EXIT_SUCCESS);
}

int ChangeAddress(void)
{
    static uint8_t Addresses[8] = {0, 1, 0, 2, 0, 3, 0, 4};
    static int AddCnt = 0; /* アドレスカウンタ */

    AddCnt++;
    if (AddCnt >= 8)
        AddCnt = 0;

    return Addresses[AddCnt];
}

/*-----------------------------------------------
 *
 * アドレスを書き込む
 *
-----------------------------------------------*/
void WriteAddress(int Address)
{
    SERVO_ADDRESS_WRITE_bit0 = Address & 0b001;
    SERVO_ADDRESS_WRITE_bit1 = (Address >> 1) & 0b001;
    SERVO_ADDRESS_WRITE_bit2 = (Address >> 2) & 0b001;
}

/*-----------------------------------------------
 *
 * データを書き込む
 *
-----------------------------------------------*/
void WriteData(int Address)
{
    SERVO_DATA_WRITE_bit0 = RxData0[Address].data0;
    SERVO_DATA_WRITE_bit1 = RxData0[Address].data1;
    SERVO_DATA_WRITE_bit2 = RxData0[Address].data2;
    SERVO_DATA_WRITE_bit3 = RxData0[Address].data3;
    SERVO_DATA_WRITE_bit4 = RxData0[Address].data4;
    SERVO_DATA_WRITE_bit5 = RxData0[Address].data5;
    SERVO_DATA_WRITE_bit6 = RxData0[Address].data6;
    SERVO_DATA_WRITE_bit7 = RxData0[Address].data7;
}

void interrupt INTERRUPT_HANDLER()
{
    if (RX_InterruptFlag)
        Reception_from_master(SLAVE_ADDRESS);
}