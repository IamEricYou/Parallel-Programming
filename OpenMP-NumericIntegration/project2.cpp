//Name : Jaehyung You
//Description : Project 2
//Reference: http://web.engr.oregonstate.edu/~mjb/cs575/Projects/proj02.html

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <stdio.h>

#ifndef NUMT
#define NUMT     4
#endif

#ifndef NUMNODES
#define NUMNODES 10
#endif 

#ifndef NUMTRIES
#define NUMTRIES 10
#endif

#define XMIN	 0.
#define XMAX	 3.
#define YMIN	 0.
#define YMAX	 3.

#define TOPZ00  0.
#define TOPZ10  1.
#define TOPZ20  0.
#define TOPZ30  0.

#define TOPZ01   1.
#define TOPZ11  12.
#define TOPZ21   1.
#define TOPZ31   0.

#define TOPZ02  0.
#define TOPZ12  1.
#define TOPZ22  0.
#define TOPZ32  4.

#define TOPZ03  3.
#define TOPZ13  2.
#define TOPZ23  3.
#define TOPZ33  3.

#define BOTZ00  0.
#define BOTZ10  -3.
#define BOTZ20  0.
#define BOTZ30  0.

#define BOTZ01  -2.
#define BOTZ11  10.
#define BOTZ21  -2.
#define BOTZ31  0.

#define BOTZ02  0.
#define BOTZ12  -5.
#define BOTZ22  0.
#define BOTZ32  -6.

#define BOTZ03  -3.
#define BOTZ13   2.
#define BOTZ23  -8.
#define BOTZ33  -3.


float
Height( int iu, int iv )	// iu,iv = 0 .. NUMNODES-1
{
	float u = (float)iu / (float)(NUMNODES-1);
	float v = (float)iv / (float)(NUMNODES-1);

	// the basis functions:

	float bu0 = (1.-u) * (1.-u) * (1.-u);
	float bu1 = 3. * u * (1.-u) * (1.-u);
	float bu2 = 3. * u * u * (1.-u);
	float bu3 = u * u * u;

	float bv0 = (1.-v) * (1.-v) * (1.-v);
	float bv1 = 3. * v * (1.-v) * (1.-v);
	float bv2 = 3. * v * v * (1.-v);
	float bv3 = v * v * v;

	// finally, we get to compute something:


        float top =       bu0 * ( bv0*TOPZ00 + bv1*TOPZ01 + bv2*TOPZ02 + bv3*TOPZ03 )
                        + bu1 * ( bv0*TOPZ10 + bv1*TOPZ11 + bv2*TOPZ12 + bv3*TOPZ13 )
                        + bu2 * ( bv0*TOPZ20 + bv1*TOPZ21 + bv2*TOPZ22 + bv3*TOPZ23 )
                        + bu3 * ( bv0*TOPZ30 + bv1*TOPZ31 + bv2*TOPZ32 + bv3*TOPZ33 );

        float bot =       bu0 * ( bv0*BOTZ00 + bv1*BOTZ01 + bv2*BOTZ02 + bv3*BOTZ03 )
                        + bu1 * ( bv0*BOTZ10 + bv1*BOTZ11 + bv2*BOTZ12 + bv3*BOTZ13 )
                        + bu2 * ( bv0*BOTZ20 + bv1*BOTZ21 + bv2*BOTZ22 + bv3*BOTZ23 )
                        + bu3 * ( bv0*BOTZ30 + bv1*BOTZ31 + bv2*BOTZ32 + bv3*BOTZ33 );

        return top - bot;	// if the bottom surface sticks out above the top surface
				// then that contribution to the overall volume is negative
}

int main( int argc, char *argv[ ] )
{
	// the area of a single full-sized tile:
	
    #ifndef _OPENMP
        fprintf( stderr, "No OpenMP support!\n" );
        return 1;
    #endif

    omp_set_num_threads( NUMT );
	float fullTileArea = (  ( ( XMAX - XMIN )/(float)(NUMNODES-1) )  *
				( ( YMAX - YMIN )/(float)(NUMNODES-1) )  );

	// sum up the weighted heights into the variable "volume"
	// using an OpenMP for loop and a reduction:
	double maxPerformance = 0.;
	double final_vol = 0.;

	for (int j = 0; j < NUMTRIES; j++)
	{
		double time0 = omp_get_wtime();
		double volume = 0;
		#pragma omp parallel for default(none) shared(fullTileArea),reduction(+:volume)
		for( int i =0; i < NUMNODES * NUMNODES; i++)
		{
			double temp = 0.;
			int iu = i % NUMNODES;
			int iv = i / NUMNODES;
			if ((iu == 0 || iu == NUMNODES -1 ) && (iv == 0 || iv == NUMNODES - 1))
				temp = fullTileArea * Height(iu, iv) / 4;
			else if ( (iu == 0 || iu == (NUMNODES -1)) || iv == 0 || iv == (NUMNODES -1))
				temp = fullTileArea * Height(iu, iv) / 2;
			else
				temp = fullTileArea * Height(iu, iv);
			volume += temp;
		}
		double time1 = omp_get_wtime();
		double megaTrialsPerSecond = (double) ((float)NUMNODES * (float)NUMNODES/(time1-time0)/1000000.);
		if (megaTrialsPerSecond > maxPerformance)
			maxPerformance = megaTrialsPerSecond;
		final_vol += volume;
	}
	
	printf("NUMT: %d NODES*NODES: %d\t\t Volume: %8.2lf\t MaxPerformance: %8.2lf \n", NUMT, NUMNODES * NUMNODES, final_vol/NUMTRIES, maxPerformance);
	return 0;
}