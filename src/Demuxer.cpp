/*
 * demuxer.cpp
 *
 *  Created on: Sep 8, 2017
 *      Author: Nicola Rossi
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <memory>
#include <vector>
#include "exception_handling.h"
#include "TS_Packet.h"
#include "Demuxer.h"

namespace challenge {
    const int debug = 0;

    void update_frequency(std::map<int, int>& map_to_update, int key) {
        auto it = map_to_update.find(key);
        auto pid = key;
        if (it != map_to_update.end()) {
            ++it->second;
        } else {
            if (debug) {
                std::cerr << " DEBUG " << __LINE__
                        << " Encountered the first PES packet for stream_"
                        << pid << std::endl;
            }
            map_to_update[key] = 1;
        }
    }

    void Demuxer::extract_stream(std::ifstream &input) {
        // We make a single read in memory, the transport_stream is reserved in the Demuxer constructor .
        input.read(reinterpret_cast<char*>(transport_stream.data()),
                size_stream);

        packet_read = 0;

        TS_Packet p;
        for (auto it_stream = transport_stream.begin();
                it_stream != transport_stream.end(); ++packet_read) {

            p.construct_packet(it_stream);

            if (!p.is_valid()) {
                throw TS_format_sync_byte_exception(packet_read * SIZE_PACKET);
            }

            if (debug) {
                std::cerr << " Working with packet at position 0x" << std::hex
                        << (packet_read * SIZE_PACKET) << std::dec << " "
                        << (packet_read * SIZE_PACKET) << std::endl;
                std::cerr << p;
            }

            int afc = p.get_adaptation_field_control();

            if (afc != 1 && afc != 3) {
                throw TS_format_AFC_value_exception(packet_read);
            }

            int pid = p.get_PID();

            if (pid == 0) {
                /* TODO Read PAT information */
                continue;
            }

            update_frequency(freq_of, pid);

            // Save the payload in the fstream associated to the Program Id
            auto &stream_to_write = (stream_of[pid]);

            // if the fstream is not created we will create it in the "out/" directory
            if (!stream_to_write.is_open() ) {
                std::stringstream ss;
                ss << "out/stream_" << pid;
                std::string stream_path = ss.str();

                stream_to_write.open(stream_path,
                        std::fstream::out | std::fstream::binary);
                if (!stream_to_write.is_open()) {
                    throw FS_open_file_exception();
                }

            }

            uint8_t *payload = p.get_payload();
            int payload_size = p.get_payload_size();

            stream_to_write.write(reinterpret_cast<char*>(payload),
                    payload_size);

        }
        return;
    }
    void Demuxer::dump_extracted_stream() {
        std::cout << " Dump data " << std::endl;

        for (auto it = stream_of.begin(); it != stream_of.end(); ++it) {
            it->second.close();
        }

        std::cout << " PID \t Packets written " << std::endl;
        std::cout << "--------------------------------" << std::endl;

        for (auto it = freq_of.begin(); it != freq_of.end(); ++it) {
            std::cout << " " << it->first << "\t" << it->second << std::endl;
        }
        std::cout << " " << std::endl;

        return;
    }

}
