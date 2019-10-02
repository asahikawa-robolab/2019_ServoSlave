#include <xc.h>
#include "Init.h"

void Init()
{
    InitOsc();
    InitPin();
    InitTmr1();
    InitTmr2();
    //    InitUART();

    /* 割り込み許可 */
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
}

void InitOsc()
{
    OSCCONbits.IRCF = 0b1110; /* 8MHz */
    OSCCONbits.SCS = 0b11;    /* 内部オシレータクロック */
}

void InitPin()
{
    /* data,address */
    ANSELA = 0x00; //digital
    ANSELB = 0x00;

    TRISA = 0xFF;
    TRISB = 0xFF; /* パラメータ選択用DIPスイッチ入力 */

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

void InitTmr1()
{
    T1CONbits.TMR1CS = 0;    /* Fosc/4 */
    T1CONbits.T1CKPS = 0b00; /* pre 1:1 */
    PIE1bits.TMR1IE = 1;     /* 割り込み許可 */
    TMR1 = 0;
    TMR1ON = 1; /* タイマ有効化 */
}

void InitTmr2()
{
    //T1CONbits.TMR1CS = 0;    /* Fosc/4 */
    T2CONbits.T2CKPS = 0b11; /* pre 1:64 */
    PIE1bits.TMR2IE = 1;     /* 割り込み許可 */
    TMR2 = 0;
    TMR2ON = 1; /* タイマ有効化 */
}

void InitUART(void)
{
    /* ボーレート（9600） */
    SPBRG = 12;
    SYNC = 0;
    BRG16 = 0;
    BRGH = 0;

    /* 有効化 */
    TXSTAbits.TXEN = 1;
    RCSTAbits.SPEN = 1;

    /* 割り込み */
    PIE1bits.TXIE = 0;
}