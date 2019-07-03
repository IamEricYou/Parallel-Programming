#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <sys/time.h>
#include <sys/resource.h>

#include <omp.h>

#ifndef SIMD_H
#define SIMD_H

// SSE stands for Streaming SIMD Extensions

#define SSE_WIDTH	4

#define ALIGNED		__attribute__((aligned(16)))


void	SimdMul(    float *, float *,  float *, int );
float	SimdMulSum( float *, float *, int );
float   NonSimdMulSum( float *, float *, int );

#endif		// SIMD_H