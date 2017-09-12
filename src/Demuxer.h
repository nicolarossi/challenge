/*
 * demuxer.h
 *
 *  Created on: Sep 8, 2017
 *      Author: Nicola Rossi
 */

#ifndef DEMUXER_H_
#define DEMUXER_H_
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <memory>
#include "TS_Packet.h"

namespace challenge {

    class Demuxer {
            size_t size_stream;
            size_t packet_read;
            std::vector<uint8_t> transport_stream;
            std::map<int, int> freq_of;
            std::map<int, std::fstream> stream_of;

        public:
            Demuxer(size_t size_stream) :
                    size_stream(size_stream),
                    packet_read(0),
                    transport_stream(size_stream) {
            }

            void extract_stream(std::ifstream &input);
            void dump_extracted_stream();

    };

}

#endif /* DEMUXER_H_ */
