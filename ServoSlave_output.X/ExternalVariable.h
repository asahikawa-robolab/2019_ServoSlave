#ifndef EXTERNAL_VARIABLE_H
#define EXTERNAL_VARIABLE_H
#include <stdint.h>
#include <stdbool.h>
#include "Structures.h"
#include "Config.h"

extern uint16_t _TMRCnt[SERVO_NUM]; /* タイマにセットする値 */
extern uint8_t _DebugTxData[DATA_SIZE];
extern bool _isBusy;
extern SERVO_CH _CHq[SERVO_NUM];       /* パルスを出すサーボのキュー */
extern int _IntNum;                    /* タイマ割り込みを発生させる回数 */
extern const SERVO_CH _CHs[SERVO_NUM]; /* サーボの番号 */

#endif