#include <opencv2/opencv.hpp>
#include <iostream>
#include "APE.h"

using namespace std;
using namespace cv;

void drawCoordinate(float *ex_mat, const float &Sfx, const float &Sfy, const float &Px, const float &Py, Mat &img, const Mat &marker, const float &minDim) {
  
  float trans[16];
  trans[0] = Sfx*ex_mat[0] + Px*ex_mat[8];
	trans[1] = Sfx*ex_mat[1] + Px*ex_mat[9];
	trans[2] = Sfx*ex_mat[2] + Px*ex_mat[10];
	trans[3] = Sfx*ex_mat[3] + Px*ex_mat[11];
	trans[4] = Sfy*ex_mat[4] + Py*ex_mat[8];
	trans[5] = Sfy*ex_mat[5] + Py*ex_mat[9];
	trans[6] = Sfy*ex_mat[6] + Py*ex_mat[10];
	trans[7] = Sfy*ex_mat[7]  + Py*ex_mat[11];
	trans[8] = ex_mat[8];
	trans[9] = ex_mat[9];
	trans[10] = ex_mat[10];
	trans[11] = ex_mat[11];
	trans[12] = 0;
	trans[13] = 0;
	trans[14] = 0;
	trans[15] = 1;
  
 float dimX = (marker.cols < marker.rows) ? minDim : (minDim * float(marker.cols) / float(marker.rows));
 float dimY = (marker.cols < marker.rows) ? (minDim * float(marker.rows) / float(marker.cols)) : minDim;
  Point_<float> B1, B2, B3, B4, T1, T2, T3, T4;
  B1.x = (trans[0]*dimX + trans[1]*dimY + trans[2]*0.0 + trans[3]) / 
         (trans[8]*dimX + trans[9]*dimY + trans[10]*0.0 + trans[11]);
  B1.y = (trans[4]*dimX + trans[5]*dimY + trans[6]*0.0 + trans[7]) / 
         (trans[8]*dimX + trans[9]*dimY + trans[10]*0.0 + trans[11]);
  B2.x = (trans[0]*dimX + trans[1]*(-dimY) + trans[2]*0.0 + trans[3]) / 
         (trans[8]*dimX + trans[9]*(-dimY) + trans[10]*0.0 + trans[11]);
  B2.y = (trans[4]*dimX + trans[5]*(-dimY) + trans[6]*0.0 + trans[7]) / 
         (trans[8]*dimX + trans[9]*(-dimY) + trans[10]*0.0 + trans[11]);
  B3.x = (trans[0]*(-dimX) + trans[1]*(-dimY) + trans[2]*0.0 + trans[3]) / 
         (trans[8]*(-dimX) + trans[9]*(-dimY) + trans[10]*0.0 + trans[11]);
  B3.y = (trans[4]*(-dimX) + trans[5]*(-dimY) + trans[6]*0.0 + trans[7]) / 
         (trans[8]*(-dimX) + trans[9]*(-dimY) + trans[10]*0.0 + trans[11]);
  B4.x = (trans[0]*(-dimX) + trans[1]*dimY + trans[2]*0.0 + trans[3]) / 
         (trans[8]*(-dimX) + trans[9]*dimY + trans[10]*0.0 + trans[11]);
  B4.y = (trans[4]*(-dimX) + trans[5]*dimY + trans[6]*0.0 + trans[7]) / 
         (trans[8]*(-dimX) + trans[9]*dimY + trans[10]*0.0 + trans[11]);
  T1.x = (trans[0]*dimX + trans[1]*dimY + trans[2]*dimY + trans[3]) / 
         (trans[8]*dimX + trans[9]*dimY + trans[10]*dimY + trans[11]);
  T1.y = (trans[4]*dimX + trans[5]*dimY + trans[6]*dimY + trans[7]) / 
         (trans[8]*dimX + trans[9]*dimY + trans[10]*dimY + trans[11]);
  T2.x = (trans[0]*dimX + trans[1]*(-dimY) + trans[2]*dimY + trans[3]) / 
         (trans[8]*dimX + trans[9]*(-dimY) + trans[10]*dimY + trans[11]);
  T2.y = (trans[4]*dimX + trans[5]*(-dimY) + trans[6]*dimY + trans[7]) / 
         (trans[8]*dimX + trans[9]*(-dimY) + trans[10]*dimY + trans[11]);
  T3.x = (trans[0]*(-dimX) + trans[1]*(-dimY) + trans[2]*dimY + trans[3]) / 
         (trans[8]*(-dimX) + trans[9]*(-dimY) + trans[10]*dimY + trans[11]);
  T3.y = (trans[4]*(-dimX) + trans[5]*(-dimY) + trans[6]*dimY + trans[7]) / 
         (trans[8]*(-dimX) + trans[9]*(-dimY) + trans[10]*dimY + trans[11]);
  T4.x = (trans[0]*(-dimX) + trans[1]*dimY + trans[2]*dimY + trans[3]) / 
         (trans[8]*(-dimX) + trans[9]*dimY + trans[10]*dimY + trans[11]);
  T4.y = (trans[4]*(-dimX) + trans[5]*dimY + trans[6]*dimY + trans[7]) / 
         (trans[8]*(-dimX) + trans[9]*dimY + trans[10]*dimY + trans[11]);
  
  line(img, B1, B2, Scalar(255, 0, 0), 2, CV_AA);
  line(img, B2, B3, Scalar(255, 0, 0), 2, CV_AA);
  line(img, B3, B4, Scalar(255, 0, 0), 2, CV_AA);
  line(img, B4, B1, Scalar(255, 0, 0), 2, CV_AA);
  line(img, B1, T1, Scalar(0, 255, 0), 2, CV_AA);
  line(img, B2, T2, Scalar(0, 255, 0), 2, CV_AA);
  line(img, B3, T3, Scalar(0, 255, 0), 2, CV_AA);
  line(img, B4, T4, Scalar(0, 255, 0), 2, CV_AA);
  line(img, T1, T2, Scalar(255, 255, 0), 2, CV_AA);
  line(img, T2, T3, Scalar(255, 255, 0), 2, CV_AA);
  line(img, T3, T4, Scalar(255, 255, 0), 2, CV_AA);
  line(img, T4, T1, Scalar(255, 255, 0), 2, CV_AA);
}

int main(int argc, char *argv[]) {
  if (argc != 13) {
    cout << "invalid argument!" << endl;
    cout << "./testAPE markerFile imgFile Sfx Sfy Px Py minMarkerDim minTz maxTz delta photometric verbose" << endl;
    return -1;
  }

  // assign parameters
  float Sfx = stof(string(argv[3]));
  float Sfy = stof(string(argv[4]));
  int Px = stoi(string(argv[5]));
  int Py = stoi(string(argv[6]));
  float minDim = stof(string(argv[7]));
  float minTz = stof(string(argv[8]));
  float maxTz = stof(string(argv[9]));
  float delta = stof(string(argv[10]));;
  bool photo = bool(stoi(string(argv[11])));
  bool verbose = bool(stoi(string(argv[12])));
  
  Mat marker = cv::imread(argv[1]);
  Mat img = cv::imread(argv[2]);
  if(!marker.data ) {
    cout <<  "Could not open marker" << std::endl ;
    return -1;
  }
  if(!img.data ) {
    cout <<  "Could not open img" << std::endl ;
    return -1;
  }
  
  float *ex_mat = new float[12];
  APE(ex_mat, marker, img, Sfx, Sfy, Px, Py, minDim, minTz, maxTz, delta, photo, verbose);
  drawCoordinate(ex_mat, Sfx, Sfy, Px, Py, img, marker, minDim);
  imwrite("img/result.png", img);
  delete[] ex_mat;
  return 0;
}
