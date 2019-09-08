#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdint.h>

typedef uint8_t SERVO_CH;

#define INVALID_WIDTH 65535

/* サーボの角度が最小になるときと最大になるときのパルス幅 */
typedef struct
{
    int min, max;
} PARAMETER;

/* サーボのデータ */
typedef struct
{
    uint16_t angle;
    uint16_t width;
    SERVO_CH ch;
} SERVO;

#endif