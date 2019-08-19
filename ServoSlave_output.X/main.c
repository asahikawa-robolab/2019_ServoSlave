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
#include "macros.h"

#define num_comb 6 /* �T�[�{�̎�ނ̑g�ݍ��킹(combination)�̐� */
#define num_port 4 /* �|�[�g�� */
#define num_type 3 /* �T�[�{�̎�ނ̐� */

typedef struct
{
    double conv; /* deg->sec�@�̕ϊ�(conversion)�̌W�� */
    int min; /* duty�ŏ��l */
} type;

type port[num_port];
type param[num_type];

/*�֐��}�N��*/
#define sec_duty 2000000 //Fosc*pre/4 = 8000000/4

/*prototype declarations*/
void Initialize();
void Oscillator_Initialize();
void Pin_Initialize();
void Timer_Initialize();
void Renew_Raw_Data();
void Calc_Duty();
void Servo_Output_OFF();
void Servo_Initialize();

/*flags*/
bool data_renew[num_port] = {0, 0, 0, 0};

/* values & array */
unsigned char servo_raw_data[5]; /* �p�������ʐM�Ŏ󂯎�������f�[�^ */
unsigned int servo_duty[5]; /* �w��̊p�x���o�͂��邽�߂�Duty���i�[���� */
/* ***************
 * 
 *  5: num_port+1
 * SERVO_ADDRESS ���@1����n�܂邽��
 * 
 * *************** */

/*
 * 
 */

int main(int argc, char** argv)
{
    Initialize();

    /* �ŏ��̃J�E���g�l���v�Z����܂ő҂� */
    while (1)
    {
        Renew_Raw_Data();
        if (
                data_renew[0] ==
                data_renew[1] ==
                data_renew[2] ==
                data_renew[3]
                )break;
    }
    Calc_Duty();

    /* �o��ON */
    SERVO_OUTPUT_1 = on;
    SERVO_OUTPUT_2 = on;
    SERVO_OUTPUT_3 = on;
    SERVO_OUTPUT_4 = on;

    /* ���C�����[�v */
    while (1)
    {

        TMR1 = time_20ms;
        timer1_ON = on;

        while (!timer_InterruptFlag)
        {
            /* TMR1 �̒l�� �v�Z�����ڕW�l�𒴂�����o��OFF */
            Servo_Output_OFF();
            
            /* ���̖ڕW�l���v�Z���� */
            if (TMR1 >= calc_OK) /* �S�Ẵ|�[�g���m����OFF�ɂȂ����� */
            {
                /* �ڕW�l�̍X�V */
                Renew_Raw_Data();
                Calc_Duty();
            }
        }
        /* 20ms �o�߂����犄�荞�݃t���O���N���A���o�͂�ON�ɂ��� */
        timer_InterruptFlag = CLEAR;

        SERVO_OUTPUT_1 = on;
        SERVO_OUTPUT_2 = on;
        SERVO_OUTPUT_3 = on;
        SERVO_OUTPUT_4 = on;
    }
    return (EXIT_SUCCESS);
}

void Renew_Raw_Data()
{

    servo_raw_data[SERVO_ADDRESS] = SERVO_DATA;

    /* ����̂ݎg�p�B�S�Ẵ|�[�g�̃f�[�^���X�V���ꂽ���ǂ����̊m�F */
    data_renew[SERVO_ADDRESS] ^= 1;
}

void Calc_Duty()
{
    int i;
    /*�ϊ���*/


    for (i = 1; i < 5; i++)
    {
        servo_duty[i] = ((servo_raw_data[i] * port[i - 1].conv) * sec_duty);
    }

}

void Servo_Output_OFF()
{
 
    if (TMR1 >= (time_20ms + port[0].min + servo_duty[1])) SERVO_OUTPUT_1 = off;
    if (TMR1 >= (time_20ms + port[1].min + servo_duty[2])) SERVO_OUTPUT_2 = off;
    if (TMR1 >= (time_20ms + port[2].min + servo_duty[3])) SERVO_OUTPUT_3 = off;
    if (TMR1 >= (time_20ms + port[3].min + servo_duty[4])) SERVO_OUTPUT_4 = off;

    else;
}



///////////////////////////////////////////////////////////////////////////////

void Initialize()
{
    Oscillator_Initialize();
    Pin_Initialize();
    Timer_Initialize();
    Servo_Initialize();
}

void Oscillator_Initialize()
{
    OSCCON = 0x73; //Fosc:8MHz
}

void Pin_Initialize()
{

    /* data,address */
    ANSELA = 0x00; //digital
    ANSELB = 0x00;

    TRISA = 0xFF;
    TRISB = 0xFF; /* �p�����[�^�I��pDIP�X�C�b�`���� */

    /* address */
    TRISCbits.TRISC0 = 1;
    TRISCbits.TRISC1 = 1;
    TRISCbits.TRISC2 = 1;

    /* output */
    TRISCbits.TRISC3 = 0;
    TRISCbits.TRISC4 = 0;
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC6 = 0;

    /* debug */
    TRISCbits.TRISC7 = 0;
}

void Timer_Initialize()
{

    /* pre:1:1 source:Fosc/4 */
    T1CON = 0x00;

    /* enable interrupts */
    INTCONbits.GIE = disable;
    INTCONbits.PEIE = disable;

    PIE1bits.TMR1IE = disable;
}

void Servo_Initialize()
{
    const int num[2][num_comb] = {
        {0, 0, 0, 1, 1, 2},
        {0, 1, 2, 1, 2, 2}
    };

    /* �T�[�{�̎�ނ��ƂɈقȂ�p�����[�^�̒�` */
    param[0].conv = 667E-8; /* hitec */
    param[0].min = time_900us;

    param[1].conv = 1056E-8; /* sanwa */
    param[1].min = time_500us; //time_900us;

    param[2].conv = 5926E-9; /* kondo(270[deg]) */
    param[2].min = time_700us;


    /* DIP�X�C�b�`�ɂ��p�����[�^�̑I�� */
    port[0].conv = param[num[0][SERVO_TYPE_0_1]].conv;
    port[0].min = param[num[0][SERVO_TYPE_0_1]].min;

    port[1].conv = param[num[1][SERVO_TYPE_0_1]].conv;
    port[1].min = param[num[1][SERVO_TYPE_0_1]].min;

    port[2].conv = param[num[0][SERVO_TYPE_2_3]].conv;
    port[2].min = param[num[0][SERVO_TYPE_2_3]].min;

    port[3].conv = param[num[1][SERVO_TYPE_2_3]].conv;
    port[3].min = param[num[1][SERVO_TYPE_2_3]].min;

    /* �f�o�b�O */
    if (num[0][SERVO_TYPE_0_1] == 2)debug_LED1 = 1;
    else debug_LED1 = 0;
}

