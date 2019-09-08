#ifndef CONFIG_H
#define CONFIG_H

/*-----------------------------------------------
 *
 * config
 *
-----------------------------------------------*/
// CONFIG1
#pragma config FOSC = INTOSC  // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF     // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF    // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF    // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF       // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF      // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF    // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF     // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF    // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF    // Flash Memory Self-Write Protection (Write protection off)
#pragma config VCAPEN = OFF // Voltage Regulator Capacitor Enable (All VCAP pin functionality is disabled)
#pragma config PLLEN = OFF  // PLL Enable (4x PLL disabled)
#pragma config STVREN = OFF // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will not cause a Reset)
#pragma config BORV = HI    // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), high trip point selected.)
#pragma config LVP = OFF    // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

/*-----------------------------------------------
 *
 * define
 *
-----------------------------------------------*/
/* サーボ番号・角度データ読み込み */
#define SERVO_ADDRESS_bit0 PORTCbits.RC0
#define SERVO_ADDRESS_bit1 PORTCbits.RC1
#define SERVO_ADDRESS_bit2 PORTCbits.RC2
#define SERVO_ADDRESS ((SERVO_ADDRESS_bit2 << 2) | (SERVO_ADDRESS_bit1 << 1) | (SERVO_ADDRESS_bit0))
#define SERVO_DATA PORTA

/* サーボ出力ポート */
#define SERVO_OUTPUT_1 LATCbits.LATC3
#define SERVO_OUTPUT_2 LATCbits.LATC6
#define SERVO_OUTPUT_3 LATCbits.LATC5
#define SERVO_OUTPUT_4 LATCbits.LATC4

/* 目標値入力 */
#define SERVO_INPUT PORTA
#define SERVO_ADDRESS_bit0 PORTCbits.RC0
#define SERVO_ADDRESS_bit1 PORTCbits.RC1
#define SERVO_ADDRESS_bit2 PORTCbits.RC2
#define SERVO_ADDRESS ((SERVO_ADDRESS_bit2 << 2) | (SERVO_ADDRESS_bit1 << 1) | (SERVO_ADDRESS_bit0))

/* パラメータ設定 */
#define MODE_INPUT (PORTB & 0b00111111)

/* debug */
#define LED LATCbits.LATC7

/* 通信用 */
#define UP(data) ((((int16_t)data) >> 8) & 0xFF)
#define LOW(data) (((int16_t)data) & 0xFF)
#define ASBL(up, low) ((int16_t)((up) << 8) | (low)) /* Assemble */
#define _UP(data) ((((uint32_t)data) >> 16) & 0xFF)
#define _MID(data) ((((uint32_t)data) >> 8) & 0xFF)
#define _LOW(data) (((uint32_t)data) & 0xFF)
#define _ASBL(up, mid, low) ((int32_t)((up) << 16) | (mid) << 8) | (low)) /* Assemble */

/* 基本フラグ */
#define ON 1
#define OFF 0

/* Output */
#define ALL 0b1111

/* サーボモータの数 */
#define SERVO_NUM 4

/* 送信データ数 */
#define DATA_SIZE 5

/* SERVO_CH 用 */
#define BIT(x) (1 << (x))

#endif