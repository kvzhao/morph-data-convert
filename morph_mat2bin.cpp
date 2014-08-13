// Disk IO
#include <google/protobuf/text_format.h>
#include <glog/logging.h>
#include <leveldb/db.h>
// std IO
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

// Matlab mat file IO
#include <matio.h>

// OpenCV
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;

const int kMORPH_FACE_W = 40;
const int kMORPH_FACE_H = 48;
const int kMORPHImageNBytes = kMORPH_FACE_W  * kMORPH_FACE_H * 8; 
const int kMORPHBatchSize = 54706;
const int kMORPHTrainBatches = 8;

//#define CONVERT_LABEL
//#define CHECK_LABEL
#define CONVERT_FACES

int main (int argc, char **argv) {
    mat_t *matfp;
    matvar_t *label_var, *img_var;
    matfp = Mat_Open(argv[1],MAT_ACC_RDONLY);

    if ( NULL == matfp ) {
        cerr << "Error opening MAT file " << argv[1] << "!\n";
        return EXIT_FAILURE;
    }

#ifdef CONVERT_LABEL
    // find a specific variable
    label_var = Mat_VarRead(matfp, "dataset_age");

    if ( NULL == label_var ) {
        cerr << "variable dataset_age can not be found\n" ;
    } else {
        // show some infomation
        cout << "find " << label_var->name << " array!\n";
        int size_byte = Mat_VarGetSize(label_var); 
        int len = size_byte/sizeof(double);
        cout << "Var size is " << size_byte << ", len is " <<  len << "\n";
        cout << "# of Fields: " << Mat_VarGetNumberOfFields(label_var) << "\n";
        cout << label_var->data << " with size = " << *label_var->dims << "\n";
        cout << "bytes : " << label_var->nbytes << "\n";
        cout << "rank : " << label_var->rank<< "\n";
        size_t *dims = label_var->dims;
        int dim_len = sizeof(dims)/sizeof(size_t);
        cout << "dim len: " << dim_len << "\n";
        for (size_t i = 0; i < label_var->rank; i++) {
            cout << "dims: " << *(dims +i)<< "\n";
        }

        // cast void data pointer to char pointer 
        double *label = (double*) label_var->data;
        ofstream of_ageLabel("age_label.bin", ios::out | ios::binary); 
        if (of_ageLabel.is_open()) {
            of_ageLabel.write(reinterpret_cast<char*>(label), kMORPHBatchSize * sizeof(double));
            of_ageLabel.close();
        } else {
            cout << "binary file is not opened!\n";
        }

    } // end of saving label
#endif

#ifdef CHECK_LABEL
    ifstream infile("age_label.bin", ios::in | ios::binary);
    double check_label[kMORPHBatchSize];

    infile.read(reinterpret_cast<char*>(&check_label), kMORPHBatchSize*sizeof(double));

    double *label= (double*) label_var->data;
    for (size_t i =0; i < kMORPHBatchSize; i++) {
        if( check_label[i] != *(label+i) ) {
            cout << "some bad thing happen"
                <<", which is " 
                << check_label[i]
                << " is != " 
                << *(label+i)
                << "\n";
        }
    }

    infile.close();
#endif

#ifdef CONVERT_LABEL | CHECK_LABEL
    Mat_VarFree(label_var);
#endif

#ifdef CONVERT_FACES
    // !! NOTICE: READ IMAGES COULD COST A LOT OF TIME !! //
    img_var   = Mat_VarRead(matfp, "dataset_im");
    if (NULL == img_var ) {
        cerr << "variable dataset_img can not be found!\n";
   } else {

        cout << "find " << img_var->name << " matrix !\n";
        int size_byte = Mat_VarGetSize(img_var); 
        int len = size_byte/sizeof(double);
        cout << "Var size is " << size_byte << ", len is " <<  len << "\n";
        cout << img_var->data << " with size = " << *img_var->dims << "\n";
        cout << "bytes : " << img_var->nbytes << "\n";
        cout << "rank : " << img_var->rank<< "\n";
        size_t *dims = img_var->dims;
        int dim_len = sizeof(dims)/sizeof(size_t);
        cout << "dim len: " << dim_len << "\n";
        cout << "dims: " << *dims << "\n";
        for (size_t i = 0; i < img_var->rank; i++) {
            cout << "dim[" << i << "]: " << *(dims +i)<< "\n";
        }

        // How to parse the chunk of data in memory?
        double *imgs = (double*) img_var->data;

        // binary file of image data
        ofstream of_img("morph_faces.bin", ios::out | ios::binary); 

#if 0
        // opencv compression params
        string face_dir = "morph_faces/";
        vector<int> compression_params;
        compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
        compression_params.push_back(9);

        
        double * one_pic = new double[kMORPHImageNBytes];
        for (int w =0; w < 40; w++) {
            for (int h =0; h < 48; h++) {
                one_pic[w*h+h] = *(imgs +w*h+h);
                cout << one_pic[w*h+h] << " ";
            }
            cout << endl;
        }
        //of_img.write(imgs, kMORPHImageNBytes);
        cv::Mat mat(48, 40, CV_64FC1, one_pic);
        cv::imwrite("OnePic.png", mat, compression_params);
        delete [] one_pic;
       
#endif
        if (of_img.is_open()) {
            for (int i = 0; i < kMORPHBatchSize; i++) {
                double *iptr = imgs + i * kMORPHImageNBytes;
                of_img.write(reinterpret_cast<char*>(iptr), kMORPHImageNBytes * sizeof(double));
                /*
                string img_name = face_dir + "face_" + to_string(i) + ".png";
                cv::Mat mat(kMORPH_FACE_H, kMORPH_FACE_W, CV_8U, imgs);
                cv::imwrite(img_name, mat, compression_params);
                */
            }
            of_img.close();
        } else {
            cout << "binary file is not opened!\n";
        }
    } // end of saving image set 
    Mat_VarFree(img_var);
#endif
    Mat_Close(matfp);
    return EXIT_SUCCESS;
}

