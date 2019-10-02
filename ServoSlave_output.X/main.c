#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "Config.h"
#include "Init.h"
#include "Interrupt.h"
#include "ShareFunc.h"
#include "Structures.h"
#include "ExternalVariable.h"
#include "Parameter.h"

void ReceiveTargetAngle(SERVO Servo[]);
void SetParameter(SERVO Servo[]);
void CalcNextCnt(SERVO Servo[]);
void Swap(SERVO *a, SERVO *b);
uint16_t CalcWidth(uint8_t angle, PARAMETER Parameter);
uint16_t CalcTMRCnt(uint16_t pulse_width);

int main(void)
{
    bool isFirst = true;
    SERVO Servo[SERVO_NUM];

    Init();
    Output(ALL, OFF);

    while (1)
    {
        /* パルスを出力するためのタイマのカウントを計算 */
        ReceiveTargetAngle(Servo); /* 目標角度を受信する */
        SetParameter(Servo); /* SERVO のパラメータを計算する */
        CalcNextCnt(Servo); /* タイマカウントを計算 */

    }
    return 0;
}

/*-----------------------------------------------
 *
 * 目標角度を受信する
 *
-----------------------------------------------*/
void ReceiveTargetAngle(SERVO Servo[])
{
    static uint16_t pre_angle[SERVO_NUM];
    bool rx[4] = {false, false, false, false};
    uint16_t angle_buf[SERVO_NUM];

    while (!(rx[0] & rx[1] & rx[2] & rx[3]))
    {
        if (SERVO_ADDRESS > 0)
        {
            angle_buf[SERVO_ADDRESS - 1] = SERVO_INPUT;
            rx[SERVO_ADDRESS - 1] = true;
        }
    }

    int i;
    for (i = 0; i < SERVO_NUM; i++)
    {
        if (angle_buf[i] == pre_angle[i])
        {
            Servo[i].angle = angle_buf[i];
        }
        pre_angle[i] = angle_buf[i];
    }
}

/*-----------------------------------------------
 *
 * SERVO のパラメータを設定する
 *
-----------------------------------------------*/
void SetParameter(SERVO Servo[])
{
    static PARAMETER Parameter[SERVO_NUM];
    static bool isFirst = true;
    int i;

    /* 初回のみパラメータを読み込む */
    if (isFirst == true)
    {
        isFirst = false;
        uint8_t Mode = MODE_INPUT;
        for (i = 0; i < SERVO_NUM; i++)
        {
            Parameter[i] = _Parameters[Mode][i];
        }
    }

    /* パラメータを計算 */
    for (i = 0; i < SERVO_NUM; i++)
    {
        Servo[i].width = CalcWidth(Servo[i].angle, Parameter[i]);
        //        Servo[i].width = 800;
        Servo[i].ch = _CHs[i];
    }
}

/*-----------------------------------------------
 *
 * タイマ割り込みに渡すタイマの値を計算する
 *
-----------------------------------------------*/
void CalcNextCnt(SERVO Servo[])
{
    _TMRCnt[0] = CalcTMRCnt(Servo[0].width);
    _TMRCnt[1] = CalcTMRCnt(Servo[1].width);
    _TMRCnt[2] = CalcTMRCnt(Servo[2].width);
    _TMRCnt[3] = CalcTMRCnt(Servo[3].width);
}

/*-----------------------------------------------
 *
 * 値を入れ替える
 *
-----------------------------------------------*/
void Swap(SERVO *a, SERVO *b)
{
    SERVO tmp = *a;
    *a = *b;
    *b = tmp;
}

/*-----------------------------------------------
 *
 * 引数に指定した角度になるようなパルス幅（μs）を求める
 *
-----------------------------------------------*/
uint16_t CalcWidth(uint8_t angle, PARAMETER Parameter)
{
    uint16_t width = (uint16_t) ((Parameter.max - Parameter.min) / 180.0 * angle + Parameter.min);
    return width;
}

/*-----------------------------------------------
 *
 * 引数に指定された時間（μs）経過したらタイマがオーバーフローするようなタイマの値を計算する
 *
-----------------------------------------------*/
uint16_t CalcTMRCnt(uint16_t pulse_width)
{
    uint16_t cnt = (uint16_t) (65535 - 2 * pulse_width);
    return cnt;
}

/*-----------------------------------------------
 *
 * 割り込み
 *
-----------------------------------------------*/
void interrupt INTERRUPT_HANDLER(void)
{
    if (INTCONbits.PEIE & PIE1bits.TMR1IE & PIR1bits.TMR1IF)
    {
        TMR1IF = false;
        Interrupt_TMR1();
    }else if(INTCONbits.PEIE & PIE1bits.TMR2IE & PIR1bits.TMR2IF)
    {
        TMR2IF = false;
        Interrupt_TMR2();
    }
    else if (INTCONbits.PEIE & PIE1bits.TXIE & PIR1bits.TXIF)
    {
        TXIF = false;
        Interrupt_TX();
    }
}