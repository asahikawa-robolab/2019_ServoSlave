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

    /*��{�t���O*/
#define on 1
#define off 0

#define enable 1
#define disable 0

#define SET 1
#define CLEAR 0

#define true 1
#define false 0

    /* timer1���� */
#define timer1_ON T1CONbits.TMR1ON
#define timer_InterruptFlag PIR1bits.TMR1IF

    /* �T�[�{�ԍ��E�p�x�f�[�^�ǂݍ��� */
#define SERVO_ADDRESS_bit0 PORTCbits.RC0
#define SERVO_ADDRESS_bit1 PORTCbits.RC1
#define SERVO_ADDRESS_bit2 PORTCbits.RC2
#define SERVO_ADDRESS ((SERVO_ADDRESS_bit2 << 2) | (SERVO_ADDRESS_bit1 << 1) | (SERVO_ADDRESS_bit0))
#define SERVO_DATA PORTA

    /* �T�[�{�̎�ޓǂݍ��� */
#define SERVO_TYPE_0_1 (PORTB&0b00000111)
#define SERVO_TYPE_2_3 ((PORTB&0b00111000)>>3)

    /* �T�[�{�o�̓|�[�g */
#define SERVO_OUTPUT_1 LATCbits.LATC3
#define SERVO_OUTPUT_2 LATCbits.LATC6
#define SERVO_OUTPUT_3 LATCbits.LATC5
#define SERVO_OUTPUT_4 LATCbits.LATC4

    /* debug */
#define debug_LED1 LATCbits.LATC7

    /* timer������ݒ肷�� */
#define time_20ms 25536
#define calc_OK 45536 /* �ŐV��Duty���v�Z���Ă��悢���� */

    /* �T�[�{�̎�ނ��Ƃ̃^�C�}�̎��Ԑݒ� */
    /* 900us-2100us 180[deg] Hitec num:0*/
    //#define degree_sec 667E-8 
#define time_900us 1800
    /* ****************************************
     * 
     * HITEC��1�̃p�����[�^�ł����������Ƃ��ł���
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

