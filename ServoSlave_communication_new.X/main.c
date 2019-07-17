/* 
 * File:   main.c
 * Author: pguz1
 *
 * Created on 2019/07/15, 17:52
 */

/*Configuration bits*/

// PIC16F1933 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable (All VCAP pin functionality is disabled)
#pragma config PLLEN = ON      // PLL Enable (4x PLL enabled)
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will not cause a Reset)
#pragma config BORV = HI        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), high trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


/*include files*/
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Controller_Protocol.h"
#include "communication.h"
#include "variable.h"

/* プロトタイプ宣言---------------------*/
//*initializes*/
void Initialize();
void Oscillator_Initialize();
void Pin_Initialize();
void UART_Initialize();
void interrupt INTERRUPT_HANDLER();
void SetData();
void ChangeAddress();
void ChangeData();
void Write_Address(Processed);
void Write_data(Processed);
/*-------------------------------------*/

/*defines*/
/*servo address*/
//*define per bit*/ 
#define SERVO_ADDRESS_WRITE_bit0 LATBbits.LATB1
#define SERVO_ADDRESS_WRITE_bit1 LATBbits.LATB0  
#define SERVO_ADDRESS_WRITE_bit2 LATCbits.LATC5

#define none 0b000
#define data 1

/*servo data*/
#define servo1_data RxData0[1]
#define servo2_data RxData0[2]
#define servo3_data RxData0[3]
#define servo4_data RxData0[4]
#define EmergencyStop RxData0[5].all_data
#define stop 0x0
#define start 0x1

//*define per bit*/
#define SERVO_DATA_WRITE_bit0 LATAbits.LATA0
#define SERVO_DATA_WRITE_bit1 LATAbits.LATA1
#define SERVO_DATA_WRITE_bit2 LATAbits.LATA2
#define SERVO_DATA_WRITE_bit3 LATAbits.LATA3
#define SERVO_DATA_WRITE_bit4 LATBbits.LATB5
#define SERVO_DATA_WRITE_bit5 LATBbits.LATB4
#define SERVO_DATA_WRITE_bit6 LATBbits.LATB2
#define SERVO_DATA_WRITE_bit7 LATBbits.LATB3

#define debug LATCbits.LATC4

/*variables*/
//*servo address*/
Processed address;
Processed zero;

/*flags*/
//*current status*/
bool address_status = none; //現在送られているアドレス

/*UART RX*/
extern bool Receive_flag;

/*slave address*//**********///
#define SLAVE_ADDRESS 0xC0
///*************************///

/*
 * 
 */

int main(int argc, char** argv) {
    Initialize();

    /*初回受信*/
    while (Receive_flag != reception_complete);
    Organize_Datas(Data_from_master, Buffer_from_master, number_of_rxdata0, master_COM);
    Receive_flag = waiting_to_receive;
    /*初回受信終わり*/

    while (1) {
        if (Receive_flag == reception_complete) {
            Reception_from_master_main();
            SetData();

        }
    }

    return (EXIT_SUCCESS);
}

void SetData() {
    debug^=1;
    /*1->none->2->none->3->none->4->none->1->...*/
    ChangeAddress();
    ChangeData();
}

void ChangeAddress() {
    Processed None;
    None.all_data = 0b000;

    /*アドレス変更*/

    if (address_status == none) {
        /*現在送られているアドレスがnoneだったら
         * LATに次のアドレス反映*/
        Write_Address(address);

        /*フラグチェンジ*/
        address_status = data;

    } else {
        /*none以外なら
         LATをnoneに*/
        Write_Address(None);

        /*フラグチェンジ*/
        address_status = none;

        /*アドレスをインクリメント*/
        address.all_data++;

        /*4までいったら1に戻る*/
        if (address.all_data >= 0b101) {
            address.all_data = 0b001;
        }

    }

}

void Write_Address(Processed address) {
    SERVO_ADDRESS_WRITE_bit0 = address.data0;
    SERVO_ADDRESS_WRITE_bit1 = address.data1;
    SERVO_ADDRESS_WRITE_bit2 = address.data2;
}

void ChangeData(void) {
    /*データ変更*/

    if (address.all_data == 1) {
        if (EmergencyStop == stop)Write_data(zero);
        else Write_data(servo1_data);
    } else if (address.all_data == 2) {
        if (EmergencyStop == stop)Write_data(zero);
        else Write_data(servo2_data);
    } else if (address.all_data == 3) {
        if (EmergencyStop == stop)Write_data(zero);
        else Write_data(servo3_data);
    } else if (address.all_data == 4) {
        if (EmergencyStop == stop)Write_data(zero);
        else Write_data(servo4_data);
    } else;

}

void Write_data(Processed servo_data) {

    SERVO_DATA_WRITE_bit0 = servo_data.data0;
    SERVO_DATA_WRITE_bit1 = servo_data.data1;
    SERVO_DATA_WRITE_bit2 = servo_data.data2;
    SERVO_DATA_WRITE_bit3 = servo_data.data3;
    SERVO_DATA_WRITE_bit4 = servo_data.data4;
    SERVO_DATA_WRITE_bit5 = servo_data.data5;
    SERVO_DATA_WRITE_bit6 = servo_data.data6;
    SERVO_DATA_WRITE_bit7 = servo_data.data7;

}

void interrupt INTERRUPT_HANDLER() {
    if (RX_InterruptFlag) {
        Reception_from_master(SLAVE_ADDRESS);
    } else;
}

void Initialize() {
    Oscillator_Initialize();
    Pin_Initialize();
    UART_Initialize();
    Initialize_Parameters();
    address.all_data = 0b001; //アドレス初期設定
    zero.all_data = 0xFF; //脱力状態命令
}

void Oscillator_Initialize() {
    OSCCON = 0xF0; /* 8MHz-> 4xPLL -> 32MHz */
}

void Pin_Initialize() {
    ANSELA = 0x00; //全てデジタル入出力
    ANSELB = 0x00;

    TRISCbits.TRISC7 = 1; //UART RX

    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA3 = 0;
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;

    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB0 = 0;
    TRISCbits.TRISC5 = 0;
    
    TRISCbits.TRISC4=0;  //debug
}

void UART_Initialize() {
    // disable interrupts before changing states
    RXInterruptEnable = disable;

    /*baud rate*/
    SPBRGH = 0x00;
    SPBRGL = 0x8A; //138
    TXSTAbits.BRGH = SET; //hi speed
    BAUDCONbits.BRG16 = SET; //16 bit generator

    /*enable UART RX interrupt*/
    RCSTAbits.SPEN = enable; //enable serial port
    RCSTAbits.CREN = enable;
    TXSTAbits.SYNC = disable;

    /*enable interrupts*/
    INTCONbits.GIE = enable;
    INTCONbits.PEIE = enable;
    RXInterruptEnable = enable;

    /*enable reception*/
    ContinuousRXEnable = enable;
}




