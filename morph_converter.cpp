// Disk IO
#include <google/protobuf/text_format.h>
#include <glog/logging.h>
#include <leveldb/db.h>
// std IO
#include <iostream>
#include <fstream>
#include <stdlib.h>

// Matlab mat file IO
#include <matio.h>

using namespace std;

const int kMORPH_FACE_W = 40;
const int kMORPH_FACE_H = 48;
const int kMORPHImageNBytes = kMORPH_FACE_W  * kMORPH_FACE_H * 8; 
const int kMORPHBatchSize = 54706;
const int kMORPHTrainBatches = 8;

int main (int argc, char **argv) {
    mat_t *matfp;
    matvar_t *label_var, *img_var;
    matfp = Mat_Open(argv[1],MAT_ACC_RDONLY);

    if ( NULL == matfp ) {
        cerr << "Error opening MAT file " << argv[1] << "!\n";
        return EXIT_FAILURE;
    }

    // find a specific variable
    label_var = Mat_VarRead(matfp, "dataset_age");

    void *data_ptr;

    if ( NULL == label_var ) {
        cerr << "variable dataset_age can not be found\n" ;
    } else {
        cout << "find " << label_var->name << " array!\n";
        int len = Mat_VarGetSize(label_var)/sizeof(double);
        cout << "Var size is " << Mat_VarGetSize(label_var) << ", len is " <<  len << "\n";
        cout << "# of Fields: " << Mat_VarGetNumberOfFields(label_var) << "\n";
        cout << label_var->data << " with size = " << *label_var->dims << "\n";
        cout << "bytes : " << label_var->nbytes << "\n";
        cout << "rank : " << label_var->rank<< "\n";

        char *label = (char*) label_var->data;
        ofstream of_ageLabel("age_label.bin", ios::out | ios::binary); 
        of_ageLabel.write(label, kMORPHBatchSize * 8);
        /*
        for (int i=0; i < len; i++) {
            // Readable data
            of_ageLabel << *(label + i) << "\n";
        }*/
        of_ageLabel.close();
    }
    Mat_VarFree(label_var);
/*
    // !! NOTICE: READ IMAGES COULD COST A LOT OF TIME !! //
    img_var   = Mat_VarRead(matfp, "dataset_im");
    if (NULL == img_var ) {
        cerr << "variable dataset_img can not be found!\n";
   }
    Mat_VarFree(img_var);
*/

    Mat_Close(matfp);
    return EXIT_SUCCESS;
}

