/* 
 * File:   main.c
 * Author: pguz1
 *
 * Created on 2019/06/04, 15:40
 */

/*Configuration Bits*/


// PIC16F18325 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FEXTOSC = OFF    // FEXTOSC External Oscillator mode Selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINT32 // Power-up default value for COSC bits (HFINTOSC with 2x PLL (32MHz))
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; I/O or oscillator function on OSC2)
#pragma config CSWEN = OFF      // Clock Switch Enable bit (The NOSC and NDIV bits cannot be changed by user software)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config MCLRE = OFF      // Master Clear Enable bit (MCLR/VPP pin function is digital input; MCLR internally disabled; Weak pull-up under control of port pin's WPU control bit.)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config WDTE = OFF       // Watchdog Timer Enable bits (WDT disabled; SWDTEN is ignored)
#pragma config LPBOREN = OFF    // Low-power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled)
#pragma config BORV = HIGH      // Brown-out Reset Voltage selection bit (Brown-out voltage (Vbor) set to 2.7V)
#pragma config PPS1WAY = OFF    // PPSLOCK bit One-Way Set Enable bit (The PPSLOCK bit can be set and cleared repeatedly (subject to the unlock sequence))
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will not cause a Reset)
#pragma config DEBUG = OFF      // Debugger enable bit (Background debugger disabled)

// CONFIG3
#pragma config WRT = OFF        // User NVM self-write protection bits (Write protection off)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (High Voltage on MCLR/VPP must be used for programming.)

// CONFIG4
#pragma config CP = OFF         // User NVM Program Memory Code Protection bit (User NVM code protection disabled)
#pragma config CPD = OFF        // Data NVM Memory Code Protection bit (Data NVM code protection disabled)

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

/*prototype declarations*/
//*initializes*/
void Initialize();
void Oscillator_Initialize();
void Pin_Initialize();
void UART_Initialize();

//*interrupt*/
void interrupt INTERRUPT_HANDLER();

//*others*/
void SetData();
void ChangeAddress();
void ChangeData();
void Write_Address(Processed);
void Write_data(Processed);

/*defines*/
/*servo address*/
//*define per bit*/ 
#define SERVO_ADDRESS_WRITE_bit0 LATCbits.LATC5
#define SERVO_ADDRESS_WRITE_bit1 LATCbits.LATC3  
#define SERVO_ADDRESS_WRITE_bit2 LATAbits.LATA4

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
#define SERVO_DATA_WRITE_bit3 LATCbits.LATC0
#define SERVO_DATA_WRITE_bit4 LATCbits.LATC1
#define SERVO_DATA_WRITE_bit5 LATCbits.LATC2
#define SERVO_DATA_WRITE_bit6 LATCbits.LATC4
//#define SERVO_DATA_WRITE_bit7 LATCbits.LATC3

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
    //SERVO_DATA_WRITE_bit7 = servo_data.data7;  //8bitになったらコメントアウトすること

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
    OSCCON1 = 0x00;
    OSCFRQ = 0x06; //16MHz
    while (PLLR == 0); //Wait for PLL to stablize 
}

void Pin_Initialize() {
    ANSELA = 0x00; //全てデジタル入出力

    TRISAbits.TRISA5 = 1; //UART RX

    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC4 = 0;

    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC3 = 0;
    TRISAbits.TRISA4 = 0;


    /*PPS*/
    bool state = GIE;
    GIE = 0;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x00; // unlock PPS

    RXPPS = 0x5;

    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x01; // lock PPS

    GIE = state;
}

void UART_Initialize() {
    // disable interrupts before changing states
    RXInterruptEnable = disable;

    /*baud rate*/
    SP1BRGH = 0x00;
    SP1BRGL = 0x8A; //138
    TX1STAbits.BRGH = SET; //hi speed
    BAUD1CONbits.BRG16 = SET; //16 bit generator

    /*enable UART RX interrupt*/
    RC1STAbits.SPEN = enable; //enable serial port
    RC1STAbits.CREN = enable;
    TX1STAbits.SYNC = disable;

    /*enable interrupts*/
    INTCONbits.GIE = enable;
    INTCONbits.PEIE = enable;
    RXInterruptEnable = enable;

    /*enable reception*/
    ContinuousRXEnable = enable;
}

