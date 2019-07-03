//Name : Jaehyung You
//Description : Project 4 - arraymult.cpp
//Reference: http://web.engr.oregonstate.edu/~mjb/cs575/Projects/proj04.html

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <stdio.h>

#include "simd.p4.h"

#ifndef NUMT
#define NUMT 4
#endif

#ifndef ARRAYSIZE
#define ARRAYSIZE 100000
#endif

#define NUM_TRIES 1000

void MyMultFunc(float *A1, float *A2, float *result, int size)
{
    int i = 0;
    for (; i < size; i++)
    {
        result[i] = A1[i] * A2[i];
    }
}

//we don't need this, the function is already provided
float MyMultSumFunc(float *A1, float *A2, int size)
{
    float sum = 0.0;

    for(int i = 0; i < size; i++)
    {
        sum = sum + (A1[i] * A2[i]);
    }

    return sum;
}

int main( int argc, char *argv[ ] )
{
    #ifndef _OPENMP
        fprintf( stderr, "No OpenMP support!\n" );
        return 1;
    #endif

    float* A = new float[ARRAYSIZE];
    float* B = new float[ARRAYSIZE];
    float* C = new float[ARRAYSIZE];

    //omp_set_num_threads( NUMT );

    double maxMegaMultsPerSecond = 0.0;
    double maxMegaMultsPerSecond2 = 0.0;
    double maxMegaMultsPerSecond3 = 0.0;
    double maxMegaMultsPerSecond4 = 0.0;

    for( int t = 0; t < NUM_TRIES; t++ )
    {
        //SIMD - MUL
        double time0 = omp_get_wtime( );
        SimdMul( A, B, C, ARRAYSIZE );
        double time1 = omp_get_wtime( );
        double megaMultsPerSecond = (double)ARRAYSIZE / ( time1 - time0 ) / 1000000.;
        if( megaMultsPerSecond > maxMegaMultsPerSecond )
        maxMegaMultsPerSecond = megaMultsPerSecond;

        //non-SIMD - MUL
        double time2 = omp_get_wtime( );
        MyMultFunc( A, B, C, ARRAYSIZE );
        double time3 = omp_get_wtime( );
        double megaMultsPerSecond2 = (double)ARRAYSIZE / ( time3 - time2 ) / 1000000.;
        if( megaMultsPerSecond2 > maxMegaMultsPerSecond2 )
        maxMegaMultsPerSecond2 = megaMultsPerSecond2;

        //SIMD - MUL SUM
        double time4 = omp_get_wtime( );
        SimdMulSum( A, B, ARRAYSIZE );
        double time5 = omp_get_wtime( );
        double megaMultsPerSecond3 = (double)ARRAYSIZE / ( time5 - time4 ) / 1000000.;
        if( megaMultsPerSecond3 > maxMegaMultsPerSecond3 )
        maxMegaMultsPerSecond3 = megaMultsPerSecond3;

        //non-SIMD - MUL SUM
        double time6 = omp_get_wtime( );
        NonSimdMulSum( A, B, ARRAYSIZE );
        double time7 = omp_get_wtime( );
        double megaMultsPerSecond4 = (double)ARRAYSIZE / ( time7 - time6 ) / 1000000.;
        if( megaMultsPerSecond4 > maxMegaMultsPerSecond4 )
        maxMegaMultsPerSecond4 = megaMultsPerSecond4;
    }

    printf("    MUL-SIMD    :  Arraysize: %d\t MaxPerformance1: %8.2f\tMaxPerformance2: %8.2f \n", ARRAYSIZE, maxMegaMultsPerSecond,maxMegaMultsPerSecond2);
    printf("        Speed-up: %8.2f\n", maxMegaMultsPerSecond/maxMegaMultsPerSecond2);
    printf("    MUL-SUM SIMD: Arraysize: %d\t MaxPerformance3: %8.2f\tMaxPerformance4: %8.2f \n", ARRAYSIZE, maxMegaMultsPerSecond3,maxMegaMultsPerSecond4);
    printf("        Speed-up: %8.2f\n", maxMegaMultsPerSecond3/maxMegaMultsPerSecond4);
    printf("\n");

    delete[] A;
    delete[] B;
    delete[] C;
    return 0;
}