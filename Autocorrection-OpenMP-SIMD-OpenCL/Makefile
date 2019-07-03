all: p7openmp p7simd p7opencl
	
p7openmp: p7openmp.cpp 
	g++ p7openmp.cpp -o p7openmp -lm -fopenmp

p7simd: p7simd.cpp
	g++ p7simd.cpp simd.p4.cpp -o p7simd -lm -fopenmp

p7opencl: p7opencl.cpp
	g++ -o p7opencl p7opencl.cpp /scratch/cuda-7.0/lib64/libOpenCL.so -lm -fopenmp