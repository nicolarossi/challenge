/*
 * demuxer.h
 *
 *  Created on: Sep 8, 2017
 *      Author: Nicola Rossi
 */

#ifndef DEMUXER_H_
#define DEMUXER_H_

#include <fstream>
#include "PACKET.h"

namespace challenge {

    class demuxer {

        public:
            void write_on(PACKET&p,std::ofstream&o);
            void write_audio(PACKET&p);
            void write_video(PACKET&p);

            friend std::ifstream& operator>>(std::ifstream &input,demuxer &o);
    };

} /* namespace fanta */

#endif /* DEMUXER_H_ */
