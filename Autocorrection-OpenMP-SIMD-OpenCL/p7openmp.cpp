//Name : Jaehyung You
//Description : Project 7B

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <stdio.h>

#ifndef NUMT
#define NUMT     16
#endif

#ifndef NUMTRIES
#define NUMTRIES 1
#endif

int main()
{
    #ifndef _OPENMP
        fprintf( stderr, "No OpenMP support!\n" );
        return 1;
    #endif

    omp_set_num_threads( NUMT );
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

    double get_megaval = 0.0;
    for (int tr = 0; tr < NUMTRIES; tr++)
    {
        get_megaval = 0.0;
        double time0 = omp_get_wtime();
        #pragma omp parallel for default(none) shared(Size, Array, Sums, get_megaval)
        for( int shift = 0; shift < Size; shift++ )
        {
            float sum = 0.;
            for( int i = 0; i < Size; i++ )
            {
                sum += Array[i] * Array[i + shift];
            }
            Sums[shift] = sum;	// note the "fix #2" from false sharing if you are using OpenMP
        }
        double time1 = omp_get_wtime();
        double megaTrialsPerSecond = (double) ((float)Size /(time1-time0)/10000.);
        if (get_megaval < megaTrialsPerSecond)
            get_megaval = megaTrialsPerSecond;
    }

    printf("NUMT: %d\tMaxPerformance: %8.2lf \n", NUMT, get_megaval);

    FILE *fp_open = fopen("result_openmp.txt", "w");
    if( fp_open == NULL )
    {
        fprintf( stderr, "Cannot open file 'result_openmp.txt'\n" );
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
