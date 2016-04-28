CC=g++
NVCC=nvcc
CFLAGS=-std=c++11 -g -O3
INCS=-I/usr/local/cuda/include
LIBS=-L/usr/local/cuda/lib -lcudart

all: testAPE

testAPE: testAPE.o APE.o preCal.o preCal_kernel.o C2Festimate.o getPoses.o expandPoses.o
	$(CC) -o testAPE testAPE.o APE.o preCal.o preCal_kernel.o C2Festimate.o getPoses.o expandPoses.o `pkg-config --libs opencv` $(LIBS)

testAPE.o: testAPE.cpp APE.h
	$(CC) -c $(CFLAGS) testAPE.cpp `pkg-config --cflags opencv`

APE.o: APE.cpp APE.h parameter.h preCal.h C2Festimate.h
	$(CC) -c $(CFLAGS) APE.cpp `pkg-config --cflags opencv` $(INCS)

preCal.o: preCal.cpp preCal.h parameter.h preCal_kernel.h
	$(CC) -c $(CFLAGS) preCal.cpp $(INCS)

preCal_kernel.o: preCal_kernel.cu preCal_kernel.h device_common.h
	$(NVCC) -c -O3 preCal_kernel.cu

C2Festimate.o: C2Festimate.cu C2Festimate.h parameter.h device_common.h getPoses.h expandPoses.h
	$(NVCC) -c -O3 C2Festimate.cu

getPoses.o: getPoses.cu getPoses.h device_common.h
	$(NVCC) -c -O3 getPoses.cu

expandPoses.o: expandPoses.cu expandPoses.h parameter.h device_common.h
	$(NVCC) -c -O3 expandPoses.cu

clean: 
	rm -rf *.o testAPE
