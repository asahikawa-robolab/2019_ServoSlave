/* 
 * File:   communication.h
 * Author: nemoto
 *
 * Created on 2018/05/27, 9:33
 */

#ifndef COMMUNICATION_H
#define	COMMUNICATION_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* COMMUNICATION_H */

#include <stdint.h>
#include <stdbool.h>

////フラグbit////
#define NORD number_of_rxdata0                  ////data数(RxDataxの配列数)////
#define NOB NORD * 2 + 3                        ////data受信回数(Bufferxの配列数)////
#define RXInterruptEnable PIE1bits.RCIE    ////受信許可ビット////
#define RX_InterruptFlag PIR1bits.RCIF      ////受信フラグビット////
#define REG_for_RX RCREG                  ////受信レジスタ////
#define Buffer_from_master Buffer0              ////Buffer////
#define Data_from_master RxData0                ////RxData////
#define FLAG_FERR RCSTAbits.FERR        ////フレーミングエラーフラグ////
#define FLAG_OERR RCSTAbits.OERR        ////オーバーランエラーフラグ////
#define ContinuousRXEnable RCSTAbits.CREN        ////連続的な受信許可ビット////


////受信用ステータス////
#define reception_complete 0 //受信完了(受信待機中)
#define waiting_to_receive 1 //受信中
////受信用使い分け////
#define master_COM 0

////slave_check関数用ステータス
#define different_slave 0
#define slave_match 1    //スレーブアドレスとマスタが指定したアドレスが一致した
#define continuation 2   //受信継続する



//bool Receive_flag;

void Reception_from_master(uint8_t); //slave→master  interrupt 関数で呼び出す
void Reception_from_master_main(void);//メインループ内で呼び出す
bool EUSART_ERROR_from_master(void);
uint8_t slave_address_check(uint8_t, uint8_t, uint8_t);

























