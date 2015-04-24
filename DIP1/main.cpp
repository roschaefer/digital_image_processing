//============================================================================
// Name        : main.cpp
// Author      : Nicolas Nostheide
// Version     : 2.0
// Copyright   : -
// Description : only calls processing and test routines
//============================================================================

#include <iostream>
#include "Dip1.h"

using namespace std;

int main(int argc, const char * argv[]) {
    
    std::cout << "Open CV Version: " << CV_VERSION << std::endl;
    
    // will contain path to input image (taken from argv[1])
    string fname;
    
    // check if image path was defined
    if (argc != 2){
        cerr << "Usage: dip1 <path_to_image>" << endl;
        return -1;
    }else{
        // if yes, assign it to variable fname
        fname = argv[1];
    }
    
    // construct processing object
    Dip1 dip1;
    
    // start processing
    dip1.run(fname);
    
    // run some test routines
    dip1.test(fname);
    
    
    return 0;
}
