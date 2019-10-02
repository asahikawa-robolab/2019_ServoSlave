#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "Interrupt.h"
#include "Config.h"
#include "ShareFunc.h"
#include "ExternalVariable.h"

int IntCnt = 0;/**/

void Interrupt_TMR1(void)
{
    Output(_CHs[IntCnt], OFF);/* IntCnt番目のサーボの出力を切る */
    T1CONbits.TMR1ON = 0;/* TMR1の動作停止 */
    IntCnt++;/* 次のサーボを選択する */
}

void Interrupt_TMR2(void)
{
    TMR2 = 255 - 123; /* 125:4ms (波形の長さの実測から123に設定) */
    
    if(IntCnt != 4)
    {
        TMR1 = _TMRCnt[IntCnt];/* IntCnt番目のサーボのカウント数を代入する */
        Output(_CHs[IntCnt], ON);/* IntCnt番目のサーボを出力する */
        T1CONbits.TMR1ON = 1;/* TMR1の動作開始 */
    }else{
        IntCnt = 0;/* 最初のサーボに戻す */
    }
}

void Interrupt_TX(void)
{
    static int cnt = 0;

    TXREG = _DebugTxData[cnt];

    cnt++;
    if (cnt >= DATA_SIZE)
    {
        cnt = 0;
    }
}