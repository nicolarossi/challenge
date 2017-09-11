/*
 * common_tool.cpp
 *
 *  Created on: Sep 10, 2017
 *      Author: nop
 */
#include <iostream>

#include "common_tool.h"

namespace challenge {
    long int pack_33_bit(const uint8_t *b,int off){
            long int rv;
            if (off==4) {
                rv= ( ( ( (b[0] & (0b00001110))>>1) << 30 ) |
                      ( ( (b[1] & (0b11111111))   ) << 22 ) |
                      ( ( (b[2] & (0b11111110))>>1) << 15 ) |
                      ( ( (b[3] & (0b11111111))   ) <<  7 ) |
                      ( ( (b[4] & (0b11111110))>>1)       )
                                       );
            } else if (off==2) {
                rv= ( ( ( (b[0] & (0b00111000))>>3) << 30 ) |
                      ( ( (b[0] & (0b00000011))   ) << 28 ) |
                      ( ( (b[1] & (0b11111111))   ) << 20 ) |
                      ( ( (b[2] & (0b11111000))>>3) << 15 ) |
                      ( ( (b[2] & (0b00000011))   ) << 13 ) |
                      ( ( (b[3] & (0b11111000))>>3) <<  8 ) |
                      ( ( (b[4] & (0b11111111))   )       )
                    );
            } else {
                std::cerr << " OFFSET "<<off<<" NOT IMPLEMENTED "<<std::endl;
                exit(-1);
            };
            return rv;
        }
}
