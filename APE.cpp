#include "APE.h"

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/gpu/gpu.hpp>
#include "parameter.h"
#include "C2Festimate.h"
#include "preCal.h"
#include <cuda_profiler_api.h>

using namespace cv;
using namespace std;

void APE(float *ex_mat, const Mat &marker, const Mat &img, const float &Sfx, const float &Sfy, const int &Px, const int &Py, const float &minDim, 
         const float &tzMin, const float &tzMax, const float &delta, const bool &photo, const bool &verbose) {
  cudaProfilerStart();
  gpu::setDevice(0);
  gpu::resetDevice();
  // allocate
  Timer time;
  time.Reset(); time.Start();
  parameter para;
  gpu::GpuMat marker_d(marker.rows, marker.cols, CV_32FC3);
  gpu::GpuMat img_d(img.rows, img.cols, CV_32FC4);
  
  // pre-calculation
  preCal(&para, marker_d, img_d, marker, img, Sfx, Sfy, Px, Py, minDim, tzMin, tzMax, delta, verbose);
  time.Pause();
  long long t1 = time.get_count();
  
  
  // C2Festimation
  time.Reset(); time.Start();
  C2Festimate(ex_mat, marker_d, img_d, &para, photo, verbose);
  time.Pause();
  cout << "pre-time: " << t1 << " us." << endl;
  cout << "post-time: " << time.get_count() << " us." << endl;
  cudaProfilerStop();
}
