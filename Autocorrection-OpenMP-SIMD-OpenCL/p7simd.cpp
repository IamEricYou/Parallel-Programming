//Name : Jaehyung You
//Description : Project 7B

#include <math.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <stdio.h>

#include "simd.p4.h"

#ifndef NUMTRIES
#define NUMTRIES 10
#endif

int main( int argc, char *argv[ ] )
{
    FILE *fp = fopen( "signal.txt", "r" );
    if( fp == NULL )
    {
        fprintf( stderr, "Cannot open file 'signal.txt'\n" );
        exit( 1 );
    }
    int Size;
    fscanf( fp, "%d", &Size );
    float *Array = new float[ 2*Size ];
    float *Sums  = new float[ 1*Size ];
    for( int i = 0; i < Size; i++ )
    {
        fscanf( fp, "%f", &Array[i] );
        Array[i+Size] = Array[i];		// duplicate the array
    }
    fclose( fp );

    double time0 = omp_get_wtime();
    for ( int  shift = 0; shift < Size; shift++ )
    {
        Sums[shift] = SimdMulSum( &Array[0], &Array[0+shift], Size );
    }
    double time1 = omp_get_wtime();

    double megaTrialsPerSecond = (double) ((float)Size /(time1-time0)/10000.);

    printf("SIMD \tMaxPerformance: %8.2lf \n", megaTrialsPerSecond);
    FILE *fp_open = fopen("result_simd.txt", "w");
    if( fp_open == NULL )
    {
        fprintf( stderr, "Cannot open file 'result_simd.txt'\n" );
        exit( 1 );
    }
    for(int i = 0; i < 512; i++)
    {
        fprintf(fp_open, "%10.2f\n", Sums[i]);
    }

    fclose( fp_open );

    delete[] Array;
    delete[] Sums; 
    return 0;
}