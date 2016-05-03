# L-APEcuda

Cuda implementation of "Approximated Pose Estimation" on Linux

ver1. 160421

ver2. 160421 run on tk1
  block size: 256
  pre-time: 1.6 s, post time: 31.6 s 

ver3. 160424 improve the traverse in finding maximum in preCal_kernel.cu

ver4. 160425
  block size: 192, 8 8
  pre-time: 1.67 s, post-time: 26.7 s

ver4.1 160428
  fix the step setting and restart setting
  pre-time: 1.94 s, post-time: 26.6 s

ver4.2 160429
  fix the redundant calculation of steps of rz0 rz1 * tz_mid

ver5 160504
  move to tx1, need to cudaDeviceSyn() after calEa
  fix the location of Eas.resize()
  add unbind texture in the end of the program
  pre-time: 1.95 s, post-time: 16.8 s
