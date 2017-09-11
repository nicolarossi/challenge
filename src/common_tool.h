
#ifndef __CHALLENGE_COMMON_TOOL_
#define __CHALLENGE_COMMON_TOOL_
       enum class pack_offset { full = 4,half=2};

       int const program_stream_map    = 0b10111100;
       int const private_stream_1      = 0b10111101;
       int const padding_stream        = 0b10111110;
       int const private_stream_2      = 0b10111111;
       int const ECM_stream            = 0b11110000;
       int const EMM_stream            = 0b11110001;
       int const program_stream_directory = 0b11111111;
       int const DSMCC_stream             = 0b11110010;
       int const ITU_type_E               = 0b11111000;

       int const fast_forward = 0b000;
       int const slow_motion  = 0b001;
       int const freeze_frame = 0b010;
       int const fast_reverse = 0b011;
       int const slow_reverse  = 0b100;

       int const system_header_start_code = 0x000001BB;
#endif
