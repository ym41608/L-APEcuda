#ifndef C2F_H
#define C2F_H

#include <opencv2/core/cuda_devptrs.hpp>
#include <thrust/device_vector.h>
#include "parameter.h"

using namespace cv;

void C2Festimate(float *ex_mat, const gpu::PtrStepSz<float3> &marker_d, const gpu::PtrStepSz<float4> &img_d, parameter* para, const bool &photo, const bool &verbose);

#endif