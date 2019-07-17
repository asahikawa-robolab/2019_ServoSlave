/* 
 * File:   main.c
 * Author: pguz1
 *
 * Created on 2019/06/05, 15:35
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
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will not cause a Reset)
#pragma config BORV = HI        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), high trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


/*include files*/
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*defines*/
//*timer1*/
#define time_500us 64536 //1
#define time_19_5ms 0
#define time_1_9ms 61736 //2
#define time_17_6ms 30336 //3
#define timer1_ON T1CONbits.TMR1ON
#define timer_InterruptFlag PIR1bits.TMR1IF

/*pin*/
//#define SERVO_ADDRESS (PORTC&0b00000111)
#define SERVO_ADDRESS_bit0 PORTCbits.RC0
#define SERVO_ADDRESS_bit1 PORTCbits.RC1
#define SERVO_ADDRESS_bit2 PORTCbits.RC2
#define SERVO_DATA PORTA

#define SERVO_OUTPUT_1 LATCbits.LATC3
#define SERVO_OUTPUT_2 LATCbits.LATC6
#define SERVO_OUTPUT_3 LATCbits.LATC5
#define SERVO_OUTPUT_4 LATCbits.LATC4

#define debug_LED0 LATBbits.LATB0
#define debug_LED1 LATCbits.LATC7

#define stop 0xFF  //�p�x�̃f�[�^��8bit�ɂȂ�����A�@0xFF�@�ɕς��邱��

//��{�t���O
#define on 1
#define off 0

#define enable 1
#define disable 0

#define SET 1
#define CLEAR 0

#define true 1
#define false 0

/*�֐��}�N��*/
#define degree_sec 1056E-8 /* sec/degree �A�d�l����Z�o */
#define sec_duty 2000000 //Fosc*pre/4 = 8000000/4
#define SERVO_ADDRESS ((SERVO_ADDRESS_bit2 << 2) | (SERVO_ADDRESS_bit1 << 1) | (SERVO_ADDRESS_bit0))

/*prototype declarations*/
//*initializes*/
void Initialize();
void Oscillator_Initialize();
void Pin_Initialize();
void Timer_Initialize();

/*Servo*/
void Renew_Raw_Data();
void Calc_Duty();
void Servo_Output_OFF();

/*flags*/
int Timer1_time = time_500us;
bool data_renew[4] = {0, 0, 0, 0};
bool EmergencyStop = off;

/*values&array*/
unsigned char servo_raw_data[5];
unsigned int servo_duty[5]; // 5:number of servo + 1

/*
 * 
 */

int main(int argc, char** argv) {
    Initialize();

    /*�ŏ��̃J�E���g�l���v�Z����܂ł͑҂��Ȃ��Ƃ����Ȃ�*/
    while (1) {
        Renew_Raw_Data();
        if (
                data_renew[0] ==
                data_renew[1] ==
                data_renew[2] ==
                data_renew[3]
                )break;
    }
    Calc_Duty();

    /*�o��ON*/
    SERVO_OUTPUT_1 = on;
    SERVO_OUTPUT_2 = on;
    SERVO_OUTPUT_3 = on;
    SERVO_OUTPUT_4 = on;

    /*���C�����[�v*/
    while (1) {
        /*�@�͂��߂�500us�͂���@*/
        TMR1 = time_500us; // Timer1ON(time_500us);
        timer1_ON = on;
        while (!timer_InterruptFlag);

        /*�@�c���1.9ms�͎w�肳�ꂽ�f���[�e�B�𒴂��邩�Ď����Ȃ���*/
        timer_InterruptFlag = CLEAR;
        TMR1 = time_1_9ms;

        while (!timer_InterruptFlag) {
            /*�֐�����߂��Ă��邲�Ƃɔ��肷��*/
            Servo_Output_OFF();
        }

        /*�o�͂�OFF�̊ԂɎ��̖ڕW�p�x�ǂݍ��݁E�v�Z*/
        timer_InterruptFlag = CLEAR;
        TMR1 = time_17_6ms;
        do {
            Renew_Raw_Data();
            Calc_Duty();
        } while (!timer_InterruptFlag);
        timer_InterruptFlag = CLEAR;

        SERVO_OUTPUT_1 = on;
        SERVO_OUTPUT_2 = on;
        SERVO_OUTPUT_3 = on;
        SERVO_OUTPUT_4 = on;
    }
    return (EXIT_SUCCESS);
}

void Renew_Raw_Data() {
    /*�E�A�h���X�ɉ����Ĕz��Ɋp�x�f�[�^�i�[
     * �E�ϊ�������
     * �E���W�X�^���*/


    //        debug_LED0 ^= 1;
    if (SERVO_DATA == stop) {
        EmergencyStop = on;
    } else {
        EmergencyStop = off;
        servo_raw_data[SERVO_ADDRESS] = SERVO_DATA;
    }

    data_renew[SERVO_ADDRESS] ^= 1;
}

void Calc_Duty() {
    int i;
    /*�ϊ���*/

    debug_LED0 = PORTAbits.RA0;
    debug_LED1 = PORTAbits.RA1;
    for (i = 1; i < 5; i++) {
        servo_duty[i] = ((servo_raw_data[i] * degree_sec) * sec_duty);
    }
}

void Servo_Output_OFF() {
    if (TMR1 >= (time_1_9ms + servo_duty[1]) && (!EmergencyStop)) SERVO_OUTPUT_1 = off;
    if (TMR1 >= (time_1_9ms + servo_duty[2]) && (!EmergencyStop)) SERVO_OUTPUT_2 = off;
    if (TMR1 >= (time_1_9ms + servo_duty[3]) && (!EmergencyStop)) SERVO_OUTPUT_3 = off;
    if (TMR1 >= (time_1_9ms + servo_duty[4]) && (!EmergencyStop)) SERVO_OUTPUT_4 = off;
    else;
}



///////////////////////////////////////////////////////////////////////////////

void Initialize() {
    Oscillator_Initialize();
    Pin_Initialize();
    Timer_Initialize();
}

void Oscillator_Initialize() {
    OSCCON = 0x73; //Fosc:8MHz
}

void Pin_Initialize() {

    /*data,address*/
    ANSELA = 0x00; //digital
    ANSELB = 0x00;

    TRISA = 0xFF;

    /*address*/
    TRISCbits.TRISC0 = 1;
    TRISCbits.TRISC1 = 1;
    TRISCbits.TRISC2 = 1;

    /*output*/
    TRISCbits.TRISC3 = 0;
    TRISCbits.TRISC4 = 0;
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC6 = 0;

    /*debug*/
    TRISBbits.TRISB0 = 0;
    TRISCbits.TRISC7 = 0;
}

void Timer_Initialize() {

    /*pre:1:1 source:Fosc/4*/
    T1CON = 0x00;

    /*enable interrupts*/
    INTCONbits.GIE = disable;
    INTCONbits.PEIE = disable;

    PIE1bits.TMR1IE = disable;
}

