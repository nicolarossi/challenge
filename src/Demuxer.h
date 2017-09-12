/*
 * demuxer.h
 *
 *  Created on: Sep 8, 2017
 *      Author: Nicola Rossi
 */

#ifndef DEMUXER_H_
#define DEMUXER_H_

#include <fstream>
#include <vector>
#include <map>
#include "TS_Packet.h"

namespace challenge {

    class Demuxer {
            size_t size_stream;
            size_t packet_read;
            std::map<int,int> freq_of;
            std::map<int,std::vector<uint8_t>> stream_of;

        public:
            Demuxer(size_t size_stream):size_stream(size_stream){};
            void extract_stream(std::ifstream &input);
            void dump_extracted_stream();

    };

} 

#endif /* DEMUXER_H_ */
