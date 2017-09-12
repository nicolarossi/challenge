/*
 * PACKET.h
 *
 *  Created on: Sep 8, 2017
 *      Author: Nicola Rossi
 */

#ifndef TS_PACKET_H_
#define TS_PACKET_H_
#include <iostream>
#include <fstream>
#include <vector>

namespace challenge {
    size_t const SIZE_PACKET=188;

    class TS_Packet {
            bool PES_packet_length_zero;
            uint8_t *buffer;

        public:
            void construct_packet(std::vector<uint8_t>::iterator &input);
            bool is_valid();
            int get_TEI();
            int get_PUSI();
            int get_Transport_Priority();

            bool is_carrying_PES_packet();
            int get_adaptation_field_control();
            int get_adaptation_field_length();
            int get_random_access_indicator();
            int get_elementary_stream_priority_indicator();

            int get_PCR_flag();
            int get_OPCR_flag();
            int get_splicing_point_flag();
            int get_transport_private_data_flag();
            int get_transport_private_data_length();
            int get_adaptation_field_extension_flag();
            int get_adaptation_field_extension_length();

            int get_below_header_offset();
            int get_header_size();

            int get_continuity();
            int get_PID();
            uint8_t *get_payload();
            size_t get_payload_size();

            friend std::ostream& operator<<(std::ostream &input,TS_Packet &o);

    };

}

#endif /* TS_PACKET_H_ */
