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

    // find a specific variable
    matvar = Mat_VarRead(matfp, "dataset_age");
    void *data_ptr;
    int *start, *stride, *edge;

    if ( NULL == matvar ) {
        cerr << "variable dataset_age cant not find\n" ;
    } else {
        cout << "find " << matvar->name << " array!\n";
        int len = Mat_VarGetSize(matvar)/sizeof(double);
        cout << "Var size is " << Mat_VarGetSize(matvar) << ", len is " <<  len << "\n";
        cout << "# of Fields: " << Mat_VarGetNumberOfFields(matvar) << "\n";
        cout << matvar->data << " with size = " << *matvar->dims << "\n";
        cout << "bytes : " << matvar->nbytes << "\n";
        cout << "rank : " << matvar->rank<< "\n";

        double *label = (double*) matvar->data;
        ofstream of_ageLabel("age_label.txt", std::ofstream::out); 

        for (int i=0; i < len; i++) {
            of_ageLabel << *(label + i) << "\n";
        }
        of_ageLabel.close();
    }
    Mat_VarFree(matvar);

    Mat_Close(matfp);
    return EXIT_SUCCESS;
}

