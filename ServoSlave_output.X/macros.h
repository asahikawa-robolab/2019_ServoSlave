/* 
 * File:   macros.h
 * Author: pguz1
 *
 * Created on 2019/07/27, 7:42
 */

#ifndef MACROS_H
#define	MACROS_H

#ifdef	__cplusplus
extern "C" {
#endif

    /*基本フラグ*/
#define on 1
#define off 0

#define enable 1
#define disable 0

#define SET 1
#define CLEAR 0

#define true 1
#define false 0

    /* timer1操作 */
#define timer1_ON T1CONbits.TMR1ON
#define timer_InterruptFlag PIR1bits.TMR1IF

    /* サーボ番号・角度データ読み込み */
#define SERVO_ADDRESS_bit0 PORTCbits.RC0
#define SERVO_ADDRESS_bit1 PORTCbits.RC1
#define SERVO_ADDRESS_bit2 PORTCbits.RC2
#define SERVO_ADDRESS ((SERVO_ADDRESS_bit2 << 2) | (SERVO_ADDRESS_bit1 << 1) | (SERVO_ADDRESS_bit0))
#define SERVO_DATA PORTA

    /* サーボの種類読み込み */
#define SERVO_TYPE_0_1 (PORTB&0b00000111)
#define SERVO_TYPE_2_3 ((PORTB&0b00111000)>>3)

    /* サーボ出力ポート */
#define SERVO_OUTPUT_1 LATCbits.LATC3
#define SERVO_OUTPUT_2 LATCbits.LATC6
#define SERVO_OUTPUT_3 LATCbits.LATC5
#define SERVO_OUTPUT_4 LATCbits.LATC4

    /* debug */
#define debug_LED1 LATCbits.LATC7

    /* timer周期を設定する */
#define time_20ms 25536
#define calc_OK 45536 /* 最新のDutyを計算してもよい時間 */

    /* サーボの種類ごとのタイマの時間設定 */
    /* 900us-2100us 180[deg] Hitec num:0*/
    //#define degree_sec 667E-8 
#define time_900us 1800
    /* ****************************************
     * 
     * HITECは1のパラメータでも動かすことができる
     * 
     * **************************************** */

    /* 500us-2.4ms 180[deg] sanwa erg-vb etc.num:1 */
    //#define degree_sec 1056E-8 /* sec/degree */
#define time_500us 1000 //1

    /* 700us-2300us 270[deg] KONDO num:2 */
    //#define degree_sec 5926E-9
#define time_700us 1400

#ifdef	__cplusplus
}
#endif

#endif	/* MACROS_H */

