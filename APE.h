#ifndef APE_H
#define APE_H

#include <opencv2/opencv.hpp>
#include <chrono>

using namespace cv;

class Timer {
  typedef std::chrono::time_point<std::chrono::high_resolution_clock> Clock;
  long long count;
  bool running;
  Clock prev_start_;
  Clock Now() {
    return std::chrono::high_resolution_clock::now();
  }
public:
  void Start() {
    running = true;
    prev_start_ = Now();
  }
  void Pause() {
    if (running) {
      running = false;
      auto diff = Now() - prev_start_;
      count += std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
    }
  }
  void Reset() {
    running = false;
    count = 0;
  }
  long long get_count() {
    return count;
  }
  Timer() { Reset(); }
};

void APE(float *ex_mat, const Mat &marker, const Mat &img, const float &Sfx, const float &Sfy, const int &Px, const int &Py, const float &delta, 
         const float &tzMin, const float &tzMax, const bool photo = true, const bool verbose = true);

#endif