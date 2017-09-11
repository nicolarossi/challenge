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

#include "TS_Packet.h"

namespace challenge {
    const int debug=0;

    void update_frequency(std::map<int,int>& map_to_update,int key) {
        auto it=map_to_update.find(key);
        auto pid=key;
         if (it!=map_to_update.end()){
             it->second++;
         } else {
             if (debug) {
                 std::cerr << " DEBUG " << __LINE__
                         << " Encountered the first PES packet for stream_" << pid << std::endl;
             }
             map_to_update[key]=1;
         }
    }

    std::ifstream& operator>>(std::ifstream &input,Demuxer &o) {
        auto packet_read=-1L;
        TS_Packet p;
        std::map<int,int> freq_of;

        while (p.construct_packet(input)){
            packet_read++;

            if (!p.is_valid()) {
                std::cout<< " The packet at position 0x"<< std::hex << (packet_read*SIZE_PACKET)<< std::dec <<" doesn't begin with 0x47"<<std::endl;
                throw TS_format();
            }

            if (debug) {
                std::cerr<< " Working with packet at position 0x"
                                    << std::hex << (packet_read*SIZE_PACKET)
                                    << std::dec << " "<< (packet_read*SIZE_PACKET)<<std::endl;
                std::cerr<<p;
            }

            /**/
            int afc=p.get_adaptation_field_control();

            if (afc!=1 && afc!=3) {
                std::cout<< " The packet at position 0x"
                        << std::hex << (packet_read*SIZE_PACKET)
                        << std::dec << " "<< (packet_read*SIZE_PACKET)<<" doesn't have a AFC value recognized"
                        <<" but "<<afc<<std::endl;
                throw TS_format();
            }


            /**/
            int pid=p.get_PID();

            if (pid==0) {
                /* TODO Read PAT information */
                continue;
            }

            update_frequency(freq_of,pid);

            /* Output the payload on "out/file_PID" */
            std::stringstream ss;
            ss << "out/stream_" << pid;

            std::string s=ss.str();

            std::fstream fs;
            fs.open (s, std::fstream::out | std::fstream::app|std::fstream::binary);
            uint8_t *payload=p.get_payload();
            int payload_size=p.get_payload_size();
            fs.write((char*)(payload),payload_size);
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
        return input;
    }

} /* namespace challenge */
