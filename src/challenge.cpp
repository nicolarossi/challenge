//============================================================================
// Name        : challenge.cpp
// Author      : Nicola Rossi
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "demuxer.h"

using namespace std;

void print_usage(char *exe){
    cerr<<" Error in usage:"<<endl;
    cerr<<"\t"<<exe<<" <input_file>" << endl;
    exit(-1);
}

int main(int argc,char**argv) {

    if (argc!=2) {
        print_usage(argv[0]);
    }
    ifstream ifs(argv[1],std::ifstream::binary);

    challenge::demuxer D;

    ifs >> D;

	return 0;
}
