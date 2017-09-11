/*
 * PES_packet.h
 *
 *  Created on: Sep 9, 2017
 *      Author: Nicola Rossi
 */
#include <stdint.h>
#include "common_tool.h"

#ifndef PES_PACKET_H_
#define PES_PACKET_H_

namespace challenge {


    class PES_packet {
            uint8_t buffer[256];
            const uint8_t *PES_payload;
            int PES_not_payload_length; /* this data member is the sum of byte not contained in the payload */

            int packet_start_code_prefix;
            int stream_id;
            int PES_packet_length;

            int PES_scrambling_control;
            bool PES_priority;
            bool data_alignment_indicator;
            bool copyright;
            bool original_or_copy;
            int PTS_DTS_flags;
            bool ESCR_flag;
            bool ES_rate_flag;
            bool DSM_trick_mode_flag;
            bool additional_copy_info_flag;

            bool PES_CRC_flag;
            bool PES_extension_flag;
            int PES_header_data_length;
            long int PTS;
            long int DTS;
            long int ESCR_base,ESCR_extension;
            int ES_rate;
            int trick_mode_control;
            int field_id;
            bool intra_slice_refresh;
            int frequency_truncation;
            int rep_cntrl;
            int additional_copy_info;
            int previous_PES_packet_CRC;
            bool PES_private_data_flag=false;
            uint8_t PES_private_data[16];
            bool pack_header_field_flag=false;
            bool program_packet_sequence_counter_flag=false;
            bool PSTD_buffer_flag=false;
            bool PES_extension_flag_2=false;


            int pack_field_length;

            int program_packet_sequence_counter;
            bool MPEG1_MPEG2_identifier;
            int original_stuff_length;
            bool PSTD_buffer_scale;
            int PSTD_buffer_size;
            int PES_extension_field_length;

            bool correct_filled;
        public:
            PES_packet(const uint8_t *payload,int size_payload);

            int get_PES_not_payload_length(){
                return PES_not_payload_length;
            }
            bool is_valid() {
                return packet_start_code_prefix==1 && correct_filled;
            }
            int get_PES_packet_length() {
                return PES_packet_length;
            }

    };

} /* namespace challenge */

#endif /* PES_PACKET_H_ */
