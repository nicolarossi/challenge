//============================================================================
// Name        : challenge.cpp
// Author      : Nicola Rossi
// Description : Unified Streaming challenge. Extract audio/video from a TS stream
//============================================================================

#include <iostream>

#include "Demuxer.h"

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

    try {
        ifstream ifs(argv[1], std::ifstream::binary);
        if (!ifs.is_open()) {
            cerr<< " Error in opening file ["<<argv[1]<<"] "<<endl;
            return -1;
        }
        challenge::Demuxer D;
        ifs >> D;
    } catch (exception & E) {
        cerr << E.what() << endl;
        return -1;
    }

	return 0;
}
