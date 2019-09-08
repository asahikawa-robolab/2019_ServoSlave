#include <stdbool.h>
#include <xc.h>
#include "Config.h"
#include "ShareFunc.h"
#include "Structures.h"
#include "ExternalVariable.h"

/*-----------------------------------------------
 *
 * サーボへの信号の ON/OFF を制御する
 *
-----------------------------------------------*/
void Output(SERVO_CH ch, bool value)
{
    if (ch & _CHs[0]) SERVO_OUTPUT_1 = value;
    if (ch & _CHs[1]) SERVO_OUTPUT_2 = value;
    if (ch & _CHs[2]) SERVO_OUTPUT_3 = value;
    if (ch & _CHs[3]) SERVO_OUTPUT_4 = value;
}