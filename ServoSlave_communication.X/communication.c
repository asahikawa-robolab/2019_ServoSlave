/* 
 * File:   communication.c
 * Author: nemoto
 *
 * Created on 2018/05/27, 9:32
 */
#include "variable.h"
#include "communication.h"
#include "Controller_Protocol.h"



/*
 * 
 */

bool Receive_flag = waiting_to_receive;
//Data to controller
//char Transmit_line_data[6] = {'\0', 0x11, 0x00, 0x12, 0x00, 'E'};
//char Transmit_line_data2[5] = {0x11, 0x01, 0x12, 0x00, 'E'};

uint8_t slave_different_flag = continuation;

bool EUSART_ERROR_from_master(void) {
    if ((FLAG_FERR == 1) || (FLAG_OERR == 1)) {
        //�t���[�~���O�G���[����
        uint8_t waste_data = REG_for_RX;
        //�I�[�o�[�����G���[����
        ContinuousRXEnable = disable;
        ContinuousRXEnable = enable;
        return error;
    }
    return not_error;
}

void Reception_from_master(uint8_t slave_address) {
    static uint8_t Receive_data_count = 0;
    uint8_t error_complete = 0;

//    if (Store_Datas(Buffer_from_master, REG_for_RX, NOB, master_COM, EUSART_ERROR_from_master()) == error) {
//        Receive_data_count = CLEAR; //�G���[�N���A
//
//    } else {
//        Receive_data_count++; //��M�񐔃J�E���g
//        slave_different_flag = slave_address_check(Receive_data_count, slave_address, slave_different_flag);
//    }
//
//    if (Receive_data_count == NOB) {//�S�f�[�^��M����
//        Receive_flag = reception_complete;
//        Receive_data_count = CLEAR;
//    }
    
    error_complete = Store_Datas(Buffer_from_master, REG_for_RX, NOB, master_COM, EUSART_ERROR_from_master);


    if (error_complete == error) {
        Receive_data_count = CLEAR;
    } else if ((error_complete == complete) && (Receive_data_count != (NOB - 1))) {
        Receive_data_count = CLEAR;
    } else {
        slave_different_flag = slave_address_check(Receive_data_count, slave_address, slave_different_flag);
        Receive_data_count++;
    }

    if ((error_complete == complete)/*(slave_different_flag == slave_match)*/ && (Receive_data_count == NOB)) {
        Receive_flag = reception_complete;
        Receive_data_count = CLEAR;

    }
    
    
}

uint8_t slave_address_check(uint8_t data_count, uint8_t slave_address, uint8_t slave_flag) {
    if (slave_flag == continuation) {
        if (data_count == 4) {
            uint8_t sample_data = (ADD_upper << 4) | ADD_lower;
            //OUT1_LAT=ADD_lower;
            if (sample_data == slave_address) {
                return slave_match; //�X���[�u�A�h���X���v�I
            }
        }
        return continuation; //��M�p��
    } else {

        return slave_flag;
    }
}

void Reception_from_master_main(void) {
    if (Receive_flag == reception_complete) {
        //DEBUG1_LAT^=1;
        if (slave_different_flag == slave_match) {
            Organize_Datas(Data_from_master, Buffer_from_master, number_of_rxdata0, master_COM);
            slave_different_flag = continuation;
        } else {
            slave_different_flag = continuation;
        }
        Receive_flag = waiting_to_receive;
    } else;

}
