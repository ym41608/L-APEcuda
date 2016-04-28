#include <opencv2/opencv.hpp>
#include <iostream>
#include "APE.h"

using namespace std;
using namespace cv;

void drawCoordinate(float *ex_mat, const float &Sfx, const float &Sfy, const float &Px, const float &Py, Mat &img) {
  
  float trans[16];
  trans[0] = Sfx*ex_mat[0] + Px*ex_mat[8];
	 trans[1] = Sfx*ex_mat[1] + Px*ex_mat[9];
	 trans[2] = Sfx*ex_mat[2] + Px*ex_mat[10];
	 trans[3] = Sfx*ex_mat[3] + Px*ex_mat[11];
	 trans[4] = (-Sfy)*ex_mat[4] + (Py-1)*ex_mat[8];
	 trans[5] = (-Sfy)*ex_mat[5] + (Py-1)*ex_mat[9];
	 trans[6] = (-Sfy)*ex_mat[6] + (Py-1)*ex_mat[10];
	 trans[7] = (-Sfy)*ex_mat[7]  + (Py-1)*ex_mat[11];
	 trans[8] = ex_mat[8];
	 trans[9] = ex_mat[9];
	 trans[10] = ex_mat[10];
	 trans[11] = ex_mat[11];
	 trans[12] = 0;
	 trans[13] = 0;
	 trans[14] = 0;
	 trans[15] = 1;
   
  Point_<float> x, y, z, o;
  o.x = (trans[0]*0.0 + trans[1]*0.0 + trans[2]*0.0 + trans[3]) / 
        (trans[8]*0.0 + trans[9]*0.0 + trans[10]*0.0 + trans[11]);
  o.y = (trans[4]*0.0 + trans[5]*0.0 + trans[6]*0.0 + trans[7]) / 
        (trans[8]*0.0 + trans[9]*0.0 + trans[10]*0.0 + trans[11]);
  x.x = (trans[0]*0.5 + trans[1]*0.0 + trans[2]*0.0 + trans[3]) / 
        (trans[8]*0.5 + trans[9]*0.0 + trans[10]*0.0 + trans[11]);
  x.y = (trans[4]*0.5 + trans[5]*0.0 + trans[6]*0.0 + trans[7]) / 
        (trans[8]*0.5 + trans[9]*0.0 + trans[10]*0.0 + trans[11]);
  y.x = (trans[0]*0.0 + trans[1]*0.5 + trans[2]*0.0 + trans[3]) / 
        (trans[8]*0.0 + trans[9]*0.5 + trans[10]*0.0 + trans[11]);
  y.y = (trans[4]*0.0 + trans[5]*0.5 + trans[6]*0.0 + trans[7]) / 
        (trans[8]*0.0 + trans[9]*0.5 + trans[10]*0.0 + trans[11]);
  z.x = (trans[0]*0.0 + trans[1]*0.0 + trans[2]*0.5 + trans[3]) / 
        (trans[8]*0.0 + trans[9]*0.0 + trans[10]*0.5 + trans[11]);
  z.y = (trans[4]*0.0 + trans[5]*0.0 + trans[6]*0.5 + trans[7]) / 
        (trans[8]*0.0 + trans[9]*0.0 + trans[10]*0.5 + trans[11]);
  
  line(img, o, x, Scalar(255, 0, 0), 2, CV_AA);
  line(img, o, y, Scalar(0, 255, 0), 2, CV_AA);
  line(img, o, z, Scalar(0, 0, 255), 2, CV_AA);
}

int main() {
  Mat marker = cv::imread("img/Isetta.png");
  Mat img = cv::imread("img/10.png");
  if(!marker.data ) {
    cout <<  "Could not open marker" << std::endl ;
    return -1;
  }
  if(!img.data ) {
    cout <<  "Could not open img" << std::endl ;
    return -1;
  }
  
  float *ex_mat = new float[12];
  
  APE(ex_mat, marker, img, 1000, 1000, 400, 300, 0.5, 3.0, 8.0, 0.25, false, true);
  drawCoordinate(ex_mat, 1000, 1000, 400, 300, img);
  //imshow("img", img);
  //waitKey(0);
  imwrite("img/result_150.png", img);
  delete[] ex_mat;
  return 0;
}
