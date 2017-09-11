//============================================================================
// Name        : challenge.cpp
// Author      : Nicola Rossi
// Description : Unified Streaming challenge. Extract audio/video from a TS stream
//============================================================================

#include <iostream>

#include "Demuxer.h"

using namespace std;

int print_usage(char *exe){
    cout<<" Error in usage:"<<endl;
    cout<<"\t"<<exe<<" <input_file>" << endl;
    return (-1);
}

int main(int argc,char**argv) {

    if (argc!=2) {
        return print_usage(argv[0]);
    }

    try {
        ifstream ifs(argv[1], ios::ate | std::ifstream::binary);
        if (!ifs.is_open()) {
            cout<< " Error in opening file ["<<argv[1]<<"] "<<endl;
            return -1;
        }

        auto size_stream=ifs.tellg();

        if (size_stream % SIZE_PACKET) {
            cout<< " Error in opening file ["<<argv[1]<<"] "<<endl;
            cout<< "  it doesn't contain the right size of packets."<<endl;
            cout<< size_stream << " is not a multiple of "<<SIZE_PACKET<<endl;
            return -1;
        } else {
            cout<< " The stream contains ["<<size_stream/SIZE_PACKET<<"] packets"<<endl;
        }

        ifs.seekg(0,ifs.beg);

        //--- the size_stream will be used to reserve() the output vector.
        challenge::Demuxer D(size_stream);

        //--- Parse the stream
        ifs >> D;
    } catch (exception & E) {
        cout << E.what() << endl;
        return -1;
    }

	return 0;
}
