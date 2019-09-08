#include "Init.h"
#include "communication.h"
#include "variable.h"

void Initialize()
{
    Oscillator_Initialize();
    Pin_Initialize();
    UART_Initialize();
    Initialize_Parameters();
}

void Oscillator_Initialize()
{
    OSCCON = 0xF0; /* 8MHz-> 4xPLL -> 32MHz */
}

void Pin_Initialize()
{
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

    TRISCbits.TRISC4 = 0; // LED
}

void UART_Initialize()
{
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
