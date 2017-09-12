/*
 * exception_handling.h
 *
 *  Created on: Sep 11, 2017
 *      Author: Nicola Rossi
 */

#ifndef EXCEPTION_HANDLING_H_
#define EXCEPTION_HANDLING_H_
#include <iostream>
#include <exception>

namespace challenge {

    class TS_format_exception: public std::exception {
            size_t position;
        public:
            size_t get_position() {
                return position;
            }
            TS_format_exception(size_t position) :
                    position(position) {

            }
            virtual const char* what() const noexcept {
                return "Error in Transport Stream format\n";
            }
    };

    class TS_format_sync_byte_exception: public TS_format_exception {
        public:
            TS_format_sync_byte_exception(size_t position) :
                    TS_format_exception(position) {
            }

            virtual const char* what() const noexcept {
                return " The sync_byte (0x47) is not present ";
            }
    };

    class TS_format_AFC_value_exception: public TS_format_exception {
        public:
            TS_format_AFC_value_exception(size_t position) :
                    TS_format_exception(position) {
            }

            virtual const char* what() const noexcept {
                return " The packet doesn't have an AFC value recognized ";
            }
    };

    class PES_format_exception: public std::exception {
            virtual const char* what() const noexcept {
                return "Error in PES packet format\n";
            }
    };

    class FS_open_file_exception: public std::exception {
            virtual const char* what() const noexcept {
                return "Error in opening file \n";
            }
    };

}

#endif /* EXCEPTION_HANDLING_H_ */
