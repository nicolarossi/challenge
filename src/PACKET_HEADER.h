/*
 * PACKET_HEADER.h
 *
 *  Created on: Sep 10, 2017
 *      Author: nop
 */

#ifndef PACKET_HEADER_H_
#define PACKET_HEADER_H_
 #include <stdint.h>

namespace challenge {

    class PACKET_HEADER {
            long int system_clock_reference_base;
            long int system_clock_reference_extension;
            int program_mux_rate;
            int pack_stuffing_length;

        public:
/*            PACKET_HEADER();
            virtual ~PACKET_HEADER();*/
            int fill_pack_header(const uint8_t *p,int max_size);

    };

} /* namespace challenge */

#endif /* PACKET_HEADER_H_ */
