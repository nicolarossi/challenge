/*
 * demuxer.h
 *
 *  Created on: Sep 8, 2017
 *      Author: Nicola Rossi
 */

#ifndef DEMUXER_H_
#define DEMUXER_H_

#include <fstream>

#include "TS_Packet.h"

namespace challenge {

    class Demuxer {
        public:
            friend std::ifstream& operator>>(std::ifstream &input,Demuxer &o);
    };

} /* namespace challenge */

#endif /* DEMUXER_H_ */
