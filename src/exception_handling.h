/*
 * exception_handling.h
 *
 *  Created on: Sep 11, 2017
 *      Author: nop
 */

#ifndef EXCEPTION_HANDLING_H_
#define EXCEPTION_HANDLING_H_
#include <iostream>
#include <exception>
using namespace std;

namespace challenge {

    class TS_format : public exception {
      virtual const char* what() const throw()
      {
        return "Error in Transport Stream format\n";
      }
    } ;

} /* namespace challenge */

#endif /* EXCEPTION_HANDLING_H_ */
