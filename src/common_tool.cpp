/*
 * common_tool.cpp
 *
 *  Created on: Sep 10, 2017
 *      Author: Nicola Rossi
 */
#include <iostream>
#include "common_tool.h"

namespace challenge {

    long int pack_33_bit(const uint8_t *b,pack_offset off){
            long int rv;
            if (off==pack_offset::full) {
                rv= ( ( ( (b[0] & (0b00001110))>>1) << 30 ) |
                      ( ( (b[1] & (0b11111111))   ) << 22 ) |
                      ( ( (b[2] & (0b11111110))>>1) << 15 ) |
                      ( ( (b[3] & (0b11111111))   ) <<  7 ) |
                      ( ( (b[4] & (0b11111110))>>1)       )
                                       );
            } else if (off==pack_offset::half) {
                rv= ( ( ( (b[0] & (0b00111000))>>3) << 30 ) |
                      ( ( (b[0] & (0b00000011))   ) << 28 ) |
                      ( ( (b[1] & (0b11111111))   ) << 20 ) |
                      ( ( (b[2] & (0b11111000))>>3) << 15 ) |
                      ( ( (b[2] & (0b00000011))   ) << 13 ) |
                      ( ( (b[3] & (0b11111000))>>3) <<  8 ) |
                      ( ( (b[4] & (0b11111111))   )       )
                    );
            };
            return rv;
        }
}
