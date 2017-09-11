/*
 * demuxer.cpp
 *
 *  Created on: Sep 8, 2017
 *      Author: Nicola Rossi
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include "demuxer.h"
#include "PACKET.h"
#include <map>

namespace challenge {

    void count_freq(std::map<int,int>& map_to_update,int key){
        auto it=map_to_update.find(key);
         if (it!=map_to_update.end()){
             it->second++;
         } else {
             map_to_update[key]=1;
         }

    }

    std::ifstream& operator>>(std::ifstream &input,demuxer &o){
        auto readed=-1L;

        PACKET p;
        std::map<int,int> freq_of;
        std::map<int,int> continuity_of;

        while (p.construct_packet(input)){
            readed++;

            if (!p.is_valid()) {
                std::cout<< " The packet at position 0x"<< std::hex << (readed*SIZE_PACKET)<< std::dec <<" doesn't begin with 0x47"<<std::endl;
                exit(-1);
            }

            const int debug=0;
            if (debug) {
                std::cerr<<p;
            }

            /**/
            int afc=p.get_adaptation_field_control();

            if (afc!=1 && afc!=3) {
                std::cout<< " The packet at position 0x"
                        << std::hex << (readed*SIZE_PACKET)
                        << std::dec << " "<< (readed*SIZE_PACKET)<<" doesn't have a AFC value recognised"
                        <<" but "<<afc<<std::endl;
                exit(-1);
            }


            /**/
            int pid=p.get_PID();

            if (pid==0) {
                /* TODO Read PAT information */
//                PAT pat(p.get_payload(),184);
//                continue;
            } else if (pid==1) {
                /* TODO Read CAT information */
                continue;
            } else if (pid==10) {
                /* TODO Read NIT information */
                continue;
            } else if (pid==8191) {
                /* TODO Null packet handling */
                continue;
            }

            count_freq(freq_of,pid);

            /**/
            if ((afc == 1) || (afc == 3)) {
                int seq_number = p.get_continuity();
                int expected_seq;
                auto it = continuity_of.find(pid);

                if (debug) {
                    std::cout << " DEBUG " << __LINE__
                                << " "<<readed  <<") working at position 0x" << std::hex
                                << (readed * SIZE_PACKET) << std::dec << std::endl;

                }

                if (it == continuity_of.end()) {
                    expected_seq = 0;
                    continuity_of[pid] = 0;
                    if (debug) {
                        std::cerr << " DEBUG " << __LINE__
                                << " the first PES for stream_" << pid
                                << " is at position 0x" << std::hex
                                << (readed * SIZE_PACKET) << std::dec << std::endl;
                    }
                } else {
                    expected_seq = it->second;
                }

                if (seq_number == expected_seq) {
                    continuity_of[pid] = (seq_number + 1) % 16;
                } else {
                    std::cout
                            << " SORRY: The continuity_field is not sequential, should be implemented code to handle it ."
                            << std::endl;
                }
            }

            /*  PES packet_length == 0 handled  */
            /*
            if ( p.is_carrying_PES_packet() && p.get_payload_size()==0) {
                 continue;
            }*/

            /* output the payload on "out/file_PID" */
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


        std::cout << " PID \t Packets writed "<< std::endl;
        std::cout << "----------------"<< std::endl;
        for (auto it=freq_of.begin();it!=freq_of.end();++it ){
            std::cout <<   std::dec <<" " <<it->first << "\t"<<it->second << std::endl;
        }
        std::cout << " "<<std::endl;

        std::cout << " Packets readed\t\t"<<readed << std::endl;
        std::cout << " "<<std::endl;
        return input;
    }

} /* namespace fanta */
