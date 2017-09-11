/*
 * PACKET_HEADER.cpp
 *
 *  Created on: Sep 10, 2017
 *      Author: nop
 */

#include <iostream>
#include "common_tool.h"
#include "PACKET_HEADER.h"
#include "SYSTEM_HEADER.h"
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>


namespace challenge {
    extern long int pack_33_bit(const uint8_t *b,int off);
    int PACKET_HEADER::fill_pack_header(const uint8_t *payload,int max_size){
        int off = 0;
        const uint8_t *p;
        p=payload;
        int pack_start_code = (p[0] << 24) | (p[1] << 16) | (p[2] << 8)
                | (p[3]);
        if (pack_start_code != (0x000001BA)) {

            std::cerr << " ERROR IN " << __FILE__ << " : " << __LINE__
                    << " pack_start_code != (0x000001BA) "
                    << std::hex << pack_start_code << std::dec<< std::endl;
            return -1;
        }
        off+=4;
        system_clock_reference_base = pack_33_bit(p + off, 2);

        /* we didn't update off because we need the last 2 bit of the 5th byte (p[4]) */
        system_clock_reference_extension = (((p[off+4] & 0b00000011)) << 7)
                                         | (((p[off+5] & 0b11111110) >> 1));

        off+=6;

        const uint8_t *l=p+off;
        program_mux_rate = (((l[0] & 0b11111111)) << 14)
                         | (((l[1] & 0b11111111)) << 6)
                         | (((l[2] & 0b11111100) >> 2));

        off+=3;

        l=p+off;
        /*... some bit-stuff ... */
        pack_stuffing_length = (l[0] & 0b00000111);

        off += 1+pack_stuffing_length;
        if (off>=max_size) return off;

        if (off>=max_size-4) {
            std::cerr << " ERROR IN " << __FILE__ << " : " << __LINE__
                    << " off is not complete to contain a system_header_start_code "
                    << system_header_start_code << std::endl;
            kill(11,getpid());
        }
        int nextbits=(p[off] << 24 )|(p[off+1] << 16 )|(p[off+2] << 8 )|(p[off+3]  );
        if (nextbits==system_header_start_code){
            SYSTEM_HEADER sh;
            off+=sh.fill_system_header(p+off,max_size-off);
        }

    return off;
    }

} /* namespace challenge */
