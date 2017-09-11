/*
 * PES_packet.cpp
 *
 *  Created on: Sep 9, 2017
 *      Author: Nicola Rossi
 */

//#include <string.h>
#include <string.h>
#include "PES_packet.h"

namespace challenge {
    extern long int pack_33_bit(const uint8_t *b,int off);


    PES_packet::PES_packet(const uint8_t *payload,int size_payload) {
        const uint8_t *p=payload;
        correct_filled=false;

        if (size_payload<6) {
            return ;
        }
        memcpy(buffer,p,size_payload);
        PES_payload=0; /**/
        packet_start_code_prefix=(p[0]<<16) | (p[1]<<8) | p[2];
        stream_id = ( p[3] );
        PES_packet_length = (p[4]<<8) | (p[5]);
        int off=6;
        if ( stream_id != program_stream_map
            && stream_id != padding_stream
                && stream_id != private_stream_2
                && stream_id != ECM_stream
                && stream_id != EMM_stream
                && stream_id != program_stream_directory
                && stream_id != DSMCC_stream
                && stream_id != ITU_type_E) {
            if (size_payload<9) {
                return ;
            }
            PES_scrambling_control  = ( p[6] & 0b00110000 ) >> 4;
            PES_priority            = ( p[6] & 0b00001000 ) >> 3;
            data_alignment_indicator= ( p[6] & 0b00000100 ) >> 2;
            copyright               = ( p[6] & 0b00000010 ) >> 1;
            original_or_copy        = ( p[6] & 0b00000001 )     ;
            PTS_DTS_flags           = ( p[7] & 0b11000000 ) >> 6;
            ESCR_flag               = ( p[7] & 0b00100000 ) >> 5;
            ES_rate_flag            = ( p[7] & 0b00010000 ) >> 4;
            DSM_trick_mode_flag     = ( p[7] & 0b00001000 ) >> 3;
         additional_copy_info_flag  = ( p[7] & 0b00000100 ) >> 2;
            PES_CRC_flag            = ( p[7] & 0b00000010 ) >> 1;
            PES_extension_flag      = ( p[7] & 0b00000001 )    ;
            PES_header_data_length  =   p[8] ;
            int base_of_header=off=9;
            if (PTS_DTS_flags == 0b10 ) {
                if (size_payload<(off+5)) {
                    return ;
                }
                PTS=pack_33_bit(p+off,4);
                off+=5;
            }
            if(PTS_DTS_flags == 0b11) {
                if (size_payload<(off+10)) {
                    return ;
                }
                PTS=pack_33_bit(p+off,4);
                off+=5;
                DTS=pack_33_bit(p+off,4);
                off+=5;
            }
            if (ESCR_flag){
                const uint8_t *l=p+off;
                if (size_payload<(off+6)) {
                    return ;
                }

                ESCR_base=pack_33_bit(p+off,2);


                ESCR_extension= ( ((( l[4] & 0b00000011)      ) << 7 )|
                                   (( l[5] & 0b11111110) >> 1 )
                                 );
                off+=6;
            }
            if (ES_rate_flag) {
                if (size_payload<(off+3)) {
                    return ;
                }
                const uint8_t *l=p+off;
                ES_rate=(
                         (((l[0] & 0b01111111)      ) << 15 ) |
                         (((l[1] & 0b11111111)      ) <<  7 ) |
                         (((l[2] & 0b11111110) >> 1 )       )
                        );
                off+=3;
            }
            if (DSM_trick_mode_flag) {
                if (size_payload<(off+1)) {
                    return ;
                }
                const uint8_t *l=p+off;
                trick_mode_control= (l[0] & 0b11100000 ) >> 5;

                if (trick_mode_control == fast_forward) {
                    rep_cntrl = (l[0] & 0b00011111);
                } else if ( trick_mode_control == freeze_frame ) {
                    field_id =  (l[0] & 0b00011000) >> 3 ;
                } else if ( trick_mode_control == fast_reverse ) {
                    field_id =  (l[0] & 0b00011000) >> 3 ;
                    intra_slice_refresh=  (l[0] & 0b00000100) >> 2 ;
                    frequency_truncation= (l[0] & 0b00000011)  ;
                } else if ( trick_mode_control == slow_reverse ) {
                    rep_cntrl = (l[0] & 0b00011111);
                }

                off+=1;
            }
            if (additional_copy_info_flag) {
                if (size_payload<(off+1)) {
                    return ;
                }

                const uint8_t *l=p+off;

                additional_copy_info= (l[0] & 0b01111111 );
                off+=1;
            }

            if (PES_CRC_flag) {
                if (size_payload<(off+2)) {
                    return ;
                }

                const uint8_t *l=p+off;
                previous_PES_packet_CRC = (l[0] << 8)| l[1];
                off+=2;
            }
            if (PES_extension_flag) {
                if (size_payload<(off+1)) {
                    return ;
                }

                const uint8_t *l=p+off;
                PES_private_data_flag                   = (l[0] & 0b10000000 ) >> 7;
                pack_header_field_flag                  = (l[0] & 0b01000000 ) >> 6;
                program_packet_sequence_counter_flag    = (l[0] & 0b00100000 ) >> 5;
                PSTD_buffer_flag                        = (l[0] & 0b00010000 ) >> 4;
                PES_extension_flag_2                    = (l[0] & 0b00000001 )     ;
                off+=1;

                if (PES_private_data_flag) {
                    if (size_payload<(off+16)) {
                        return ;
                    }
                    const uint8_t *l=p+off;
                    memcpy(PES_private_data,l,16);
                    off+=16;
                }
                if (pack_header_field_flag) {
                    pack_field_length=p[off];
                    off+=1;
                    if (pack_field_length > 0 ) {
                        off+=pack_field_length;
                    }
                }
                if (program_packet_sequence_counter_flag){
                    if (size_payload<(off+2)) {
                        return ;
                    }
                    const uint8_t *l=p+off;
                    program_packet_sequence_counter =  l[0]&0b01111111 ;
                    MPEG1_MPEG2_identifier          = (l[1]&0b01000000 ) >> 6 ;
                    original_stuff_length           =  l[1]&0b00111111 ;
                    off+=2;
                }
                if (PSTD_buffer_flag) {
                    if (size_payload<(off+3)) {
                        return ;
                    }

                    const uint8_t *l=p+off;
                    PSTD_buffer_scale=  ( l[0] & 0b00100000 ) >> 5;
                    PSTD_buffer_size = (( l[0] & 0b00011111 ) << 8 ) | l[1] ;
                    off+=3;
                }
                if (PES_extension_flag_2) {
                    if (size_payload<(off+1)) {
                        return ;
                    }

                    const uint8_t *l=p+off;
                    PES_extension_field_length=l[0]&0b01111111;
                    off+=1+PES_extension_field_length;
                    if (size_payload<(off)) {
                        return ;
                    }
                }
            }/* controllare qui off */
            int stuffing_len= off-base_of_header-PES_header_data_length;
            if (stuffing_len>0){
                off+=stuffing_len ;
                /* TODO It was enough use the last PES_packet_length byte */
                if (size_payload<(off)) {
                    return ;
                }
            }
            PES_payload=p+off;
        } else if ( stream_id == program_stream_map
                || stream_id == private_stream_2
                || stream_id == ECM_stream
                || stream_id == EMM_stream
                || stream_id == program_stream_directory
                || stream_id == DSMCC_stream
                || stream_id == ITU_type_E ) {
            /**/
            PES_payload=p+off;
            if (size_payload<(off)) {
                return ;
            }

        } else if ( stream_id == padding_stream) {
            /* PAYLOAD is empty */
            PES_payload=0;
        }

        PES_not_payload_length=p+off-payload;
        correct_filled=true;
        return ;
    }

} /* namespace challenge */
