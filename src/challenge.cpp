//============================================================================
// Name        : challenge.cpp
// Author      : Nicola Rossi
// Description : Unified Streaming challenge. Extract audio/video from a TS stream
//============================================================================

#include <iostream>
#include "demuxer.h"

using namespace std;

int print_usage(char *exe){
    cerr<<" Error in usage:"<<endl;
    cerr<<"\t"<<exe<<" <input_file>" << endl;
    return (-1);
}

int main(int argc,char**argv) {

    if (argc!=2) {
        return print_usage(argv[0]);
    }

    ifstream ifs(argv[1],std::ifstream::binary);

    challenge::demuxer D;
    ifs >> D;

	return 0;
}
