/*
 * SYSTEM_HEADER.cpp
 *
 *  Created on: Sep 10, 2017
 *      Author: nop
 */

#include "SYSTEM_HEADER.h"

namespace challenge {
    int SYSTEM_HEADER::fill_system_header(const uint8_t*payload,int max_size){
        const uint8_t *p=payload;
        int off=0;

        off+=(32+16+1+22+1+6+1*5+5+1+7)/8;
        while (1) {
            if (off>=max_size) {
                break;
            }
            if (!(p[off]& 0b10000000)) {
                break;
            }
            off+=3;
        }
        return off;
    }
} /* namespace challenge */
