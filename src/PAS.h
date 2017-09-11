/*
 * PAS.h
 *
 *  Created on: Sep 10, 2017
 *      Author: Nicola Rossi
 */

#ifndef PAS_H_
#define PAS_H_
#include <stdint.h>

namespace challenge {

    class PAS {
            bool correct_filled;
            uint8_t table_id;
            bool section_syntax_indicator;

            int section_length;
            int transport_stream_id;
            uint8_t version_number;
            bool current_next_indicator;
            uint8_t section_number;
            uint8_t last_section_number;

            int program_number;
            int network_PID;

            int program_map_PID;

          //  int CRC_32;

        public:
            PAS(const uint8_t*payload,int size_payload);
    };

} /* namespace challenge */

#endif /* PAS_H_ */
