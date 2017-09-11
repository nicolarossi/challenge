/*
 * PAS.cpp
 *
 *  Created on: Sep 10, 2017
 *      Author: Nicola Rossi
 */

#include "PAS.h"

namespace challenge {
    PAS::PAS(const uint8_t*payload,int size_payload) {
        int off=0;
        const uint8_t *l=payload;

        /**/
        if (size_payload < (off+3)) {
            return;
        }
        correct_filled=false;

        table_id=l[0];

        section_syntax_indicator= ((l[1] & 0b10000000)>>7);

        section_length = ((l[1] & 0b1111)<<8)|l[2];
        off+=3;

        /**/
        if (size_payload < (off+2)) {
            return;
        }
        l=payload+off;
        transport_stream_id = l[0]<<8 | l[1];
        off+=2;

        /**/
        if (size_payload < (off+3)) {
            return;
        }

        l=payload+off;
        version_number=(l[0]&0b00111110)>>1;
        current_next_indicator=(l[0]&0b1);

        section_number=l[1];
        last_section_number=l[2];

        off+=3;

//        for (int i = 0; i < N; i++) {
        while (off< size_payload-4) {
            l=payload+off;
            program_number=l[0]<<8|l[1];
            int val=(l[2]&0b11111 << 8) | l[3];

            if (program_number == 0) {
                network_PID = val;
            } else {
                program_map_PID=val;
            }
            off+=4;
        }
        l=payload+off;

        /**/
        if (size_payload < (off+4)) { /* CRC-32 space */
            return;
        }

//        CRC_32=....;
        correct_filled=true;
    }

} /* namespace challenge */
