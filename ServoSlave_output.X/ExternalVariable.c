#include <stdint.h>
#include <stdbool.h>
#include "Structures.h"
#include "Config.h"
#include "ExternalVariable.h"
#include "ShareFunc.h"

uint16_t _TMRCnt[SERVO_NUM]; /* タイマにセットする値 */
uint8_t _DebugTxData[DATA_SIZE];
bool _isBusy = false;
SERVO_CH _CHq[SERVO_NUM]; /* パルスを出すサーボのキュー */
int _IntNum;              /* タイマ割り込みを発生させる回数 */

/* サーボの番号 */
const SERVO_CH _CHs[4] = {
    BIT(0),
    BIT(1),
    BIT(2),
    BIT(3)
};