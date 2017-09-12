/*
 * PACKET.cpp
 *
 *  Created on: Sep 8, 2017
 *      Author: Nicola Rossi
 */

#include <string.h>
#include "exception_handling.h"
#include "PES_packet.h"
#include "TS_Packet.h"

namespace challenge {
    void TS_Packet::construct_packet(std::vector<uint8_t>::iterator &input) {
        buffer=&(input[0]);
        input+=SIZE_PACKET;
    }

    bool TS_Packet::is_valid() {
        bool val = (buffer[0] == 0x47);
        return val;
    }

    int TS_Packet::get_TEI() {
        int val = (buffer[1] & 0x80 ) >> 7 ;
        return val;
    }

    int TS_Packet::get_PUSI() {
        int val = (buffer[1] & 0x40 ) >> 6 ;
        return val;
    }

    int TS_Packet::get_Transport_Priority() {
        int val = (buffer[1] & 0x20 ) >> 5 ;
        return val;
    }

    int TS_Packet::get_continuity() {
        int val = buffer[3] & 0xf;
        return val;
    }

    int TS_Packet::get_PID() {
        int val = (buffer[1] << 8 | buffer[2]) & 0x1fff;
        return val;
    }

    bool TS_Packet::is_carrying_PES_packet() {
        return (buffer[1]&0x40 ) ==0x40;
    }

    int TS_Packet::get_adaptation_field_control() {
        int val = (buffer[3] & 0x30 )>>4 ;
        return val;
    }

    int TS_Packet::get_adaptation_field_length() {
        int val = (buffer[4]) ;
        return val;
    }

    int TS_Packet::get_random_access_indicator() {
        int val = (buffer[5] & 0x40) >>6  ;
        return val;
    }

    int TS_Packet::get_elementary_stream_priority_indicator() {
        int val=(buffer[5] & 0x20) >>5  ;
        return val;
    }

    int TS_Packet::get_PCR_flag() {
        int val=(buffer[5] & 0x10) >>4  ;
        return val;
    }

    int TS_Packet::get_OPCR_flag() {
        int val= (buffer[5] & 0x8) >>3  ;
        return val;
    }

    int TS_Packet::get_splicing_point_flag() {
        int val= (buffer[5] & 0x4) >>2  ;
        return val;
    }

    int TS_Packet::get_transport_private_data_flag() {
        int val= (buffer[5] & 0x2) >>1  ;
        return val;
    }

    int TS_Packet::get_adaptation_field_extension_flag() {
        int val= (buffer[5] & 0x1)   ;
        return val;
    }

    int TS_Packet::get_transport_private_data_length() {
        int offset=get_PCR_flag()*6+get_OPCR_flag()*6+get_splicing_point_flag();

        int val= (buffer[5+1+offset] )   ;
        return val;
    }

    int TS_Packet::get_adaptation_field_extension_length() {
        int offset=get_PCR_flag()*6+
                get_OPCR_flag()*6+
                get_splicing_point_flag()+
                get_transport_private_data_flag()*(1+get_transport_private_data_length());

        int val= buffer[5+1+offset];
        return val;
    }

    /* This method return the offset of payload due to underlying packets (eg PES )*/
    int TS_Packet::get_below_header_offset() {
        int afc=this->get_adaptation_field_control();
        if (afc==3) {
            int len_af_payload=buffer[4];

            if (this->is_carrying_PES_packet()) {

                try {
                    PES_packet pes_packet(buffer+(4+1+len_af_payload),SIZE_PACKET-(4+1+len_af_payload));
                    return 1+len_af_payload+pes_packet.get_PES_not_payload_length();
                } catch ( PES_format_exception &e ) {
                    return 1+len_af_payload;
                }

            } else {
                return 1+len_af_payload;
            }

            int offset=1;
            if (this->get_adaptation_field_length()>0) {
                offset+= 1; /* various flag */

                if (get_PCR_flag()) {
                    offset+= 6;
                }
                if (get_OPCR_flag()) {
                    offset+= 6;
                }
                if (get_splicing_point_flag()){
                    offset+= 1;
                }
                if (get_transport_private_data_flag()) {
                    offset+= (1+get_transport_private_data_length());
                }
                if (get_adaptation_field_extension_flag()) {
                    offset+= 1+get_adaptation_field_extension_length() ;
                }
            }

            return offset;
        } else {
          return 0;
        }
    }


    int TS_Packet::get_header_size() {
        int val=4+this->get_below_header_offset();
        return val;
    }

     uint8_t *TS_Packet::get_payload() {
        uint8_t *ptr=buffer+this->get_header_size();
        return ptr;
    }
    size_t TS_Packet::get_payload_size() {
        int afc=this->get_adaptation_field_control();
        if (afc==2) {
            return 0;
        }

        int val=SIZE_PACKET-this->get_header_size();
        return val;
    }

    /* print the packet in hex */
    std::ostream& operator<<(std::ostream &output,TS_Packet &o){

        for (size_t i=0;i<SIZE_PACKET; i++) {
            if (i%4==0) {
                output<< "    ";
            }
            output<< std::hex << std::uppercase << static_cast<int>(o.buffer[i]) << std::dec << " ";
        }
        output<< std::endl;

        return output;
    }


}
