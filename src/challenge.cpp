//============================================================================
// Name        : challenge.cpp
// Author      : Nicola Rossi
// Description : Unified Streaming challenge. Extract audio/video from a TS stream
//============================================================================

#include <iostream>
#include "exception_handling.h"
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
        ifstream ifs(argv[1], ios::ate | ifstream::binary);
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
        challenge::Demuxer demuxer(size_stream);

        demuxer.extract_stream(ifs);

        demuxer.dump_extracted_stream();

    } catch (challenge::TS_format_exception & e) {
        cout << e.what() << endl;
        cout << " on byte at position 0x" << hex <<e.get_position()
                                  << dec << " "<< e.get_position() <<endl;
        return -1;
    } catch (exception & e) {
        cout << e.what() << endl;
        return -1;
    }

	return 0;
}
