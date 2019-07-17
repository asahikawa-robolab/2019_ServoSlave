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

////�t���Obit////
#define NORD number_of_rxdata0                  ////data��(RxDatax�̔z��)////
#define NOB NORD * 2 + 3                        ////data��M��(Bufferx�̔z��)////
#define RXInterruptEnable PIE1bits.RCIE    ////��M���r�b�g////
#define RX_InterruptFlag PIR1bits.RCIF      ////��M�t���O�r�b�g////
#define REG_for_RX RCREG                  ////��M���W�X�^////
#define Buffer_from_master Buffer0              ////Buffer////
#define Data_from_master RxData0                ////RxData////
#define FLAG_FERR RCSTAbits.FERR        ////�t���[�~���O�G���[�t���O////
#define FLAG_OERR RCSTAbits.OERR        ////�I�[�o�[�����G���[�t���O////
#define ContinuousRXEnable RCSTAbits.CREN        ////�A���I�Ȏ�M���r�b�g////


////��M�p�X�e�[�^�X////
#define reception_complete 0 //��M����(��M�ҋ@��)
#define waiting_to_receive 1 //��M��
////��M�p�g������////
#define master_COM 0

////slave_check�֐��p�X�e�[�^�X
#define different_slave 0
#define slave_match 1    //�X���[�u�A�h���X�ƃ}�X�^���w�肵���A�h���X����v����
#define continuation 2   //��M�p������



//bool Receive_flag;

void Reception_from_master(uint8_t); //slave��master  interrupt �֐��ŌĂяo��
void Reception_from_master_main(void);//���C�����[�v���ŌĂяo��
bool EUSART_ERROR_from_master(void);
uint8_t slave_address_check(uint8_t, uint8_t, uint8_t);

























