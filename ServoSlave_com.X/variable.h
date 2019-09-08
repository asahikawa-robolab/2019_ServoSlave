/*
 * File:   variable.h
 * Author: nemoto
 *
 * Created on 2018/04/19, 17:28
 */

#ifndef VARIABLE_H
#define	VARIABLE_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* VARIABLE_H */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <xc.h>

#include "Controller_Protocol.h"

////バッファ////
#define ADD_upper Buffer0[1].upper_data//Address data from master
#define ADD_lower Buffer0[2].upper_data



////////////////////////////////////////////////////////////////////////////////

//基本フラグ
#define on 1
#define off 0

#define enable 1
#define disable 0

#define SET 1
#define CLEAR 0

