/*
 * demuxer.cpp
 *
 *  Created on: Sep 8, 2017
 *      Author: Nicola Rossi
 */
#include "Demuxer.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include "exception_handling.h"
#include <map>
#include <memory>
#include <vector>
#include "TS_Packet.h"

namespace challenge {
    const int debug=0;

    void update_frequency(std::map<int,int>& map_to_update,int key) {
        auto it=map_to_update.find(key);
        auto pid=key;
         if (it!=map_to_update.end()){
             ++it->second;
         } else {
             if (debug) {
                 std::cerr << " DEBUG " << __LINE__
                         << " Encountered the first PES packet for stream_" << pid << std::endl;
             }
             map_to_update[key]=1;
         }
    }

    void Demuxer::extract_stream(std::ifstream &input) {
        packet_read=-1L;
        TS_Packet p;

        while (p.construct_packet(input)){
            packet_read++;

            if (!p.is_valid()) {
                throw TS_format_sync_byte_exception(packet_read*SIZE_PACKET);
            }

            if (debug) {
                std::cerr<< " Working with packet at position 0x"
                                    << std::hex << (packet_read*SIZE_PACKET)
                                    << std::dec << " "<< (packet_read*SIZE_PACKET)<<std::endl;
                std::cerr<<p;
            }

            int afc=p.get_adaptation_field_control();

            if (afc!=1 && afc!=3) {
                throw TS_format_AFC_value_exception(packet_read);
            }


            //
            int pid=p.get_PID();

            if (pid==0) {
                /* TODO Read PAT information */
                continue;
            }

            update_frequency(freq_of,pid);


            // Save the payload in the vector associated to the Program Id
            // the vector is reserved to the input stream size possible
            // to increase the output performance
            auto &stream_to_write=stream_of[pid];

            // When the vector<> is allocated we reserve the space for the maximum number of byte
            // to write for that stream.
            // So the insert() will not copy element to increase the size.
            // Cons: We consume more memory
            if (stream_to_write.capacity()<this->size_stream){
                stream_to_write.reserve(this->size_stream);
            }

            uint8_t *payload=p.get_payload();
            int payload_size=p.get_payload_size();

            // We make a memory-to-memory copy to make a single  write on file system, no more using append.
            stream_to_write.insert(stream_to_write.end(),payload,payload+payload_size);

        }
        return ;
    }
    void Demuxer::dump_extracted_stream() {
        std::cout<< " Writing file "<<std::endl;

        for (auto it=stream_of.begin();it!=stream_of.end();++it){

            auto pid=it->first;
            auto v=it->second;

            // Dump the payload on "out/file_PID"
            std::stringstream ss;
            ss << "out/stream_" << pid;

            std::string s=ss.str();

            std::fstream fs;
            fs.open (s, std::fstream::out | std::fstream::binary);

            fs.write(reinterpret_cast<char*>(v.data()),v.size());
            fs.close();
        }

        std::cout << " PID \t Packets written "<< std::endl;
        std::cout << "----------------"<< std::endl;

        for (auto it=freq_of.begin();it!=freq_of.end();++it ){
            std::cout <<" " << it->first << "\t"<<it->second << std::endl;
        }
        std::cout << " "<<std::endl;

        std::cout << " Packets read \t\t"<<packet_read << std::endl;
        std::cout << " "<<std::endl;
        return ;
    }

} 
