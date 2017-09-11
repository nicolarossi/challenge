/*
 * SYSTEM_HEADER.h
 *
 *  Created on: Sep 10, 2017
 *      Author: nop
 */

#ifndef SYSTEM_HEADER_H_
#define SYSTEM_HEADER_H_
 #include <stdint.h>
namespace challenge {

    class SYSTEM_HEADER {
        public:
            int fill_system_header(const uint8_t*payload,int max_size);
    };

} /* namespace challenge */

#endif /* SYSTEM_HEADER_H_ */
