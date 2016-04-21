#include "preCal_kernel.h"

#include <opencv2/core/cuda_devptrs.hpp>
#include <thrust/device_vector.h>
#include <thrust/reduce.h>
#include "device_common.h"

#define BLOCK_W 16
#define BLOCK_H 16
#define BLOCK_SIZE1 BLOCK_W*BLOCK_H

using namespace cv;

__global__ void variance_kernel(float* variance, const gpu::PtrStepSz<float> imgptr, const int2 dim);

float getTVperNN(const gpu::PtrStepSz<float> &img, const int &w, const int &h) {
  // allocate
  const int area = w * h;
  thrust::device_vector<float> variance(area, 0.0);
  
  // kernel parameter for TV
  dim3 bDim(BLOCK_W, BLOCK_H);
  dim3 gDim((w - 1)/BLOCK_W + 1, (h - 1)/BLOCK_H + 1);
  variance_kernel<<<gDim,bDim>>>(thrust::raw_pointer_cast(variance.data()), img, make_int2(w, h));
  return thrust::reduce(variance.begin(), variance.end());
}

//void bindImgtoTex(const gpu::PtrStepSz<float4> img, const int& w, const int &h) {
//  tex_imgYCrCb.addressMode[0] = cudaAddressModeBorder;
//  tex_imgYCrCb.addressMode[1] = cudaAddressModeBorder;
//  tex_imgYCrCb.filterMode = cudaFilterModePoint;
//  tex_imgYCrCb.normalized = false;
//  cudaChannelFormatDesc desc = cudaCreateChannelDesc<float4>();
//  cudaBindTexture2D(0, &tex_imgYCrCb, img.data, &desc, w, h, img.step);
//}

__global__
void variance_kernel(float* variance, const gpu::PtrStepSz<float> imgptr, const int2 dim) {
  
  const int tidx = threadIdx.x;
  const int tidy = threadIdx.y;
  const int tid = tidy*BLOCK_W + tidx;
  const int x = blockIdx.x*BLOCK_W + tidx;
  const int y = blockIdx.y*BLOCK_H + tidy;
  
  if (x < 0 || x >= dim.x || y < 0 || y >= dim.y)
    return;

  const int wW = BLOCK_W + 2;
  const int wH = BLOCK_H + 2;
  const int wSize = wW*wH;
  __shared__ float window[wSize];
  
  // move data to shared
  int wXstart = blockIdx.x*BLOCK_W - 1;
  int wYstart = blockIdx.y*BLOCK_H - 1;
  for (int i = tid; i < wSize; i += BLOCK_SIZE1) {
    int wX = (i % wW) + wXstart;
    int wY = (i / wH) + wYstart;
    if (wX < 0 || wX >= dim.x || wY < 0 || wY >= dim.y)
      window[i] = 2;
    else
      window[i] = imgptr(wY, wX);
  }
  __syncthreads();
  
  // find max
  float max = 0;
  float value = imgptr(y, x);
  for (int idy = tidy; idy < tidy + 3; idy++)
    for (int idx = tidx; idx < tidx + 3; idx++) {
      int id = idy*wW + idx;
      if (window[id] != 2) {
        float diff = abs(value - window[id]);
        if (diff > max)
          max = diff;
      }
    }
  variance[y*dim.x + x] = max;
}