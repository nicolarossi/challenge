/*
 * PACKET.cpp
 *
 *  Created on: Sep 8, 2017
 *      Author: Nicola Rossi
 */

#include <string.h>
#include "PACKET.h"
#include "PES_packet.h"

namespace challenge {
    bool PACKET::construct_packet(std::ifstream &input){

        auto readed=0L;
        /**/
        while (input.read((char*)(buffer+readed),1)) {
            readed++;
            if (readed==SIZE_PACKET) {
                break;
            }
        }

        /* if the packet is incomplete ... */
        if (readed != SIZE_PACKET ) return false;

        return true;
    }

    bool PACKET::is_valid(){
        bool val= buffer[0]==0x47;
        return val;
    }

    int PACKET::get_TEI(){
        int val= (buffer[1] & 0x80 )>>7 ;
        return val;
    }
    int PACKET::get_PUSI(){
        int val= (buffer[1] & 0x40 )>>6 ;
        return val;
    }
    int PACKET::get_Transport_Priority(){
        int val= (buffer[1] & 0x20 )>>5 ;
        return val;
    }


    int PACKET::get_continuity(){
     int val = buffer[3] & 0xf;
     return val  ;
    };

    int PACKET::get_PID(){
     int val = (buffer[1]<< 8 | buffer[2] ) & 0x1fff;
     return val ;
    };

    bool PACKET::is_carrying_PES_packet(){
        return (buffer[1]&0x40 ) ==0x40;
    }

    int PACKET::get_adaptation_field_control(){
        int val=(buffer[3] & 0x30 )>>4 ;
        return val;
    }
    int PACKET::get_adaptation_field_length(){
        int val= buffer[4]  ;
        return val;
    }
    int PACKET::get_random_access_indicator(){
        int val=(buffer[5] & 0x40) >>6  ;
        return val;
    }
    int PACKET::get_elementary_stream_priority_indicator(){
        int val=(buffer[5] & 0x20) >>5  ;
        return val;
    }
    int PACKET::get_PCR_flag(){
        int val=(buffer[5] & 0x10) >>4  ;
        return val;
    }
    int PACKET::get_OPCR_flag(){
        int val= (buffer[5] & 0x8) >>3  ;
        return val;
    }
    int PACKET::get_splicing_point_flag(){
        int val= (buffer[5] & 0x4) >>2  ;
        return val;
    }

    int PACKET::get_transport_private_data_flag() {
        int val= (buffer[5] & 0x2) >>1  ;
        return val;
    }

    int PACKET::get_adaptation_field_extension_flag() {
        int val= (buffer[5] & 0x1)   ;
        return val;
    }

    /**/
    int PACKET::get_transport_private_data_length() {
        int offset=get_PCR_flag()*6+get_OPCR_flag()*6+get_splicing_point_flag();

        int val= (buffer[5+1+offset] )   ;
        return val;
    }


    int PACKET::get_adaptation_field_extension_length() {
        int offset=get_PCR_flag()*6+
                get_OPCR_flag()*6+
                get_splicing_point_flag()+
                get_transport_private_data_flag()*(1+get_transport_private_data_length());
        int val= buffer[5+1+offset];
        return val;
    }
    /*
    int PACKET::get_ltw_flag() {
        return buffer[4]  ;
    }
    int PACKET::get_piecewise_rate_flag() {
        return buffer[4]  ;
    }

    int PACKET::get_seamless_splice_flag() {
        return buffer[4]  ;
    }
    */



    /* TODO Rename this method to -> get_all_packet_header_offset */
    int PACKET::get_adaptation_field_control_offset() {
        int afc=this->get_adaptation_field_control();
        if (afc==3) {
            int len_af_payload=buffer[4];
            if (this->is_carrying_PES_packet()) {
                PES_packet pes_packet(buffer+(4+1+len_af_payload),188-(4+1+len_af_payload));
                if (pes_packet.is_valid()) {
                    return 1+len_af_payload+pes_packet.get_PES_not_payload_length();
                } else {
                    return 1+len_af_payload;
                }
            } else {
                return 1+len_af_payload;
            }

            int offset=1;
            if (this->get_adaptation_field_length()>0) {
                offset+=1; /* various flag */

                if (get_PCR_flag()) {
                    offset+=6 ; /* PCR */
                }
                if (get_OPCR_flag()) {
                    offset+=6 ; /* OPCR */
                }
                if (get_splicing_point_flag()){
                    offset+=1;
                }
                if (get_transport_private_data_flag()){
                    offset+= (1+get_transport_private_data_length());
                }
                if (get_adaptation_field_extension_flag()){
                    offset+=1+get_adaptation_field_extension_length();
                }
            }

            return offset;
        } else {
          return 0;
        }
    }


    int PACKET::get_header_size(){
        int val=4+this->get_adaptation_field_control_offset();
        return val;
    }

     uint8_t *PACKET::get_payload() {
        uint8_t *ptr=buffer+this->get_header_size();
        return ptr;
    }
    int PACKET::get_payload_size(){
        int afc=this->get_adaptation_field_control();
        if (afc==2) {
            return 0;
        }

        int val=SIZE_PACKET-this->get_header_size();
        return val;
    }

    /* print the packet in hex */
    std::ostream& operator<<(std::ostream &output,PACKET &o){

        for (int i=0;i<188; i++) {
            if (i%4==0) {
                output<< "    ";
            }
            output<< std::hex << std::uppercase << static_cast<int>(o.buffer[i]) << std::dec << " ";
        }
        output<< std::endl;

        return output;
    }


}
