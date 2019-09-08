#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "Interrupt.h"
#include "Config.h"
#include "ShareFunc.h"
#include "ExternalVariable.h"

void Interrupt_TMR1(void)
{
    static int IntCnt = 0;

    TMR1 = _TMRCnt[IntCnt];

    if (IntCnt == 0)
    {
        _isBusy = true;
        Output(ALL, ON);
    }
    else
    {
        Output(_CHq[IntCnt - 1], OFF);
    }

    IntCnt++;
    if (IntCnt >= _IntNum)
    {
        _isBusy = false;
        IntCnt = 0;
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