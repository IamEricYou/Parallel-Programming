//Name : Jaehyung You
//Description : Project 3
//Reference: http://web.engr.oregonstate.edu/~mjb/cs575/Projects/proj03.html

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <stdio.h>

int	NowYear;		// 2019 - 2024
int	NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int HuntedDeer;
int	NowNumDeer;		// number of deer in the current population

const float GRAIN_GROWS_PER_MONTH =		8.0;
const float ONE_DEER_EATS_PER_MONTH =		0.5;

const float AVG_PRECIP_PER_MONTH =		6.0;	// average
const float AMP_PRECIP_PER_MONTH =		6.0;	// plus or minus
const float RANDOM_PRECIP =			2.0;	// plus or minus noise

const float AVG_TEMP =				50.0;	// average
const float AMP_TEMP =				20.0;	// plus or minus
const float RANDOM_TEMP =			10.0;	// plus or minus noise

const float MIDTEMP =				40.0;
const float MIDPRECIP =				10.0;

float
Ranf( unsigned int *seedp,  float low, float high )
{
        float r = (float) rand_r( seedp );              // 0 - RAND_MAX

        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}

unsigned int seed = 0;  // a thread-private variable
float x = Ranf( &seed, -1.f, 1.f );

float
SQR( float x )
{
        return x*x;
}

void update_temp_prec()
{
    float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

    float temp = AVG_TEMP - AMP_TEMP * cos( ang );
    NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
    NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
    if( NowPrecip < 0. )
        NowPrecip = 0.;
}

void GrainDeer()
{
    while( NowYear < 2025 )
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        //. . .
        int checkdeer = NowNumDeer;

        if( checkdeer < NowHeight)
        {
            checkdeer = checkdeer + 1;
        }
        else if(checkdeer > 0)
        {
            checkdeer = checkdeer - 1;
        }
        else
        {
            //Nothing
        }
        
        // DoneComputing barrier:
        #pragma omp barrier
        //. . .
        NowNumDeer = checkdeer;
        // DoneAssigning barrier:
        #pragma omp barrier
        //. . .

        // DonePrinting barrier:
        #pragma omp barrier
        //. . .
    }

}

void Grain()
{
    while( NowYear < 2025 )
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        //. . .

        float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );
        float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );
        // DoneComputing barrier:
        #pragma omp barrier
        //. . .
        NowHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
        NowHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
        if(NowHeight < 0)
            NowHeight = 0;
        // DoneAssigning barrier:
        #pragma omp barrier
        //. . .

        // DonePrinting barrier:
        #pragma omp barrier
        //. . .
    }

}

void Watcher()
{
    float celsius, cmPrecip, cmHeight; 
    while( NowYear < 2025 )
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        //. . .
        celsius = (5.0/9.0) * (NowTemp - 32.0);
        cmPrecip = NowPrecip * 2.54;
        cmHeight = NowHeight * 2.54;

        // DoneComputing barrier:
        #pragma omp barrier
        //. . .

        // DoneAssigning barrier:
        #pragma omp barrier
        //. . .
        printf("%d\t%d\t%.2f\t%.2f\t%.2f\t%d\t%d\n", NowYear, NowMonth, celsius, cmPrecip, cmHeight, NowNumDeer, HuntedDeer);
        NowMonth = NowMonth + 1;
        if(NowMonth > 11)
        {
            NowYear = NowYear + 1;
            NowMonth = 0;
        }

        update_temp_prec();
        // DonePrinting barrier:
        #pragma omp barrier
        //. . .
    }

}

void MyAgent()
{
    float hunting = 0.15;
    while( NowYear < 2025 )
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        //. . .
        //Based on the hunting regulation, usually hunting seasons are between 9 to 11.
        if (NowMonth >= 8 && NowMonth <= 10 )
        {
            NowNumDeer = int(NowNumDeer * (1.0 - hunting));
            HuntedDeer = NowNumDeer;
        }
        else
        {
            HuntedDeer = 0;
        }
        
        // DoneComputing barrier:
        #pragma omp barrier
        //. . .
        if(NowNumDeer < 0)
            NowNumDeer = 0;
        // DoneAssigning barrier:
        #pragma omp barrier
        //. . .

        // DonePrinting barrier:
        #pragma omp barrier
        //. . .
    }

}

int main( int argc, char *argv[ ] )
{
    #ifndef _OPENMP
        fprintf( stderr, "No OpenMP support!\n" );
        return 1;
    #endif

    unsigned int seed = 0;
    update_temp_prec();
   // starting date and time:
    NowMonth =    0;
    NowYear  = 2019;

    // starting state (feel free to change this if you want):
    NowNumDeer = 1;
    NowHeight =  1.;

    printf("Year\tMonth\ttemp\tprecip\theight\tNumDeer\tHuntedDeer\n");
    omp_set_num_threads( 4 );	// same as # of sections
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            GrainDeer( );
        }

        #pragma omp section
        {
            Grain( );
        }

        #pragma omp section
        {
            Watcher( );
        }

        #pragma omp section
        {
            MyAgent( );	// your own
        }
    }       // implied barrier -- all functions must return in order
	// to allow any of them to get past here
    return 0;
}
