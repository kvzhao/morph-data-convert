// Disk IO
#include <google/protobuf/text_format.h>
#include <glog/logging.h>
#include <leveldb/db.h>
// std IO
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include <matio.h>

using namespace std;

int main (int argc, char **argv) {
    mat_t *matfp;
    matvar_t *matvar;
    matfp = Mat_Open(argv[1],MAT_ACC_RDONLY);

    if ( NULL == matfp ) {
        cerr << "Error opening MAT file " << argv[1] << "!\n";
        return EXIT_FAILURE;
    }
    
    matvar = Mat_VarReadInfo(matfp,"x");
    if ( NULL == matvar ) {
        cerr << "Variable ’x’ not found, or error reading MAT file\n";
    } else {
        if ( !matvar->isComplex )
            cerr << "Variable ’x’ is not complex!\n";
        if ( matvar->rank != 2 ||
                (matvar->dims[0] > 1 && matvar->dims[1] > 1) )
            cerr << "Variable ’x’ is not a vector!\n";
        Mat_VarFree(matvar);
    }

    Mat_Close(matfp);
    return EXIT_SUCCESS;
}

