#include "stdio.h"
#include <omp.h>
#define XMIN	 0.
#define XMAX	 3.
#define YMIN	 0.
#define YMAX	 3.

#define TOPZ00  0.
#define TOPZ10  1.
#define TOPZ20  0.
#define TOPZ30  0.

#define TOPZ01  1.
#define TOPZ11  6.
#define TOPZ21  1.
#define TOPZ31  0.

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
#define NUMTRIES 5
// #define NUMNODES 
// #define NUMTHREADS 

float Height(int iu, int iv)	// iu,iv = 0 .. NUMNODES-1
{
	float u = (float)iu / (float)(NUMNODES - 1);
	float v = (float)iv / (float)(NUMNODES - 1);

	// the basis functions:

	float bu0 = (1. - u) * (1. - u) * (1. - u);
	float bu1 = 3. * u * (1. - u) * (1. - u);
	float bu2 = 3. * u * u * (1. - u);
	float bu3 = u * u * u;

	float bv0 = (1. - v) * (1. - v) * (1. - v);
	float bv1 = 3. * v * (1. - v) * (1. - v);
	float bv2 = 3. * v * v * (1. - v);
	float bv3 = v * v * v;

	// finally, we get to compute something:


	float top = bu0 * (bv0*TOPZ00 + bv1*TOPZ01 + bv2*TOPZ02 + bv3*TOPZ03)
		+ bu1 * (bv0*TOPZ10 + bv1*TOPZ11 + bv2*TOPZ12 + bv3*TOPZ13)
		+ bu2 * (bv0*TOPZ20 + bv1*TOPZ21 + bv2*TOPZ22 + bv3*TOPZ23)
		+ bu3 * (bv0*TOPZ30 + bv1*TOPZ31 + bv2*TOPZ32 + bv3*TOPZ33);

	float bot = bu0 * (bv0*BOTZ00 + bv1*BOTZ01 + bv2*BOTZ02 + bv3*BOTZ03)
		+ bu1 * (bv0*BOTZ10 + bv1*BOTZ11 + bv2*BOTZ12 + bv3*BOTZ13)
		+ bu2 * (bv0*BOTZ20 + bv1*BOTZ21 + bv2*BOTZ22 + bv3*BOTZ23)
		+ bu3 * (bv0*BOTZ30 + bv1*BOTZ31 + bv2*BOTZ32 + bv3*BOTZ33);

	return top - bot;	// if the bottom surface sticks out above the top surface
						// then that contribution to the overall volume is negative
}

int main( int argc, char *argv[ ] )
{
	// the area of a single full-sized tile:

	double fullTileArea = (  ( ( XMAX - XMIN )/(double)(NUMNODES-1) )  *
				( ( YMAX - YMIN )/(double)(NUMNODES-1) )  );
	
	// sum up the weighted heights into the variable "volume"
	double volume = 0.0;
	
	//Set units
	double megaHeights = 0.0;
	double sumMegaHeights = 0.0;
	double maxMegaHeights = 0.0;
	//Start time
	double startTime =omp_get_wtime();
	
	omp_set_num_threads(NUMTHREADS);
	#pragma omp parallel for default(none), reduction(+:volume), shared(fullTileArea)
	for (int i = 0; i < NUMNODES*NUMNODES; i++) {
		//Each thread has a copy of the current volume, that gets added
		//to the running total with each pass
	
		//get iu and iv, the x and y axes
		int iu = i % NUMNODES;
		int iv = i / NUMNODES;
		double columnVolume = fullTileArea * Height(iu, iv);

		//Handle partial tiles, ie those that are edge or corner
		if (iu == 0 || iu == NUMNODES - 1) {
			if (iv == 0 || iv == NUMNODES - 1) {
				columnVolume *= 0.25; //Corner tile
			}
			else {
				columnVolume *= 0.5;
			}
		}
		else if (iv == 0 || iv == NUMNODES - 1) {
			columnVolume *= 0.5; //Edge tile
		}
		volume += columnVolume;
	}
	
	//Timing
	//click stopwatch off
	double endTime = omp_get_wtime();
	double timeElapsed = endTime - startTime;

	//Find megaheights
	megaHeights = (double)NUMNODES*double(NUMNODES) / timeElapsed / 1000000.;
	
	//Write to a txt file
	FILE *fp;
	//headers
	fp = fopen("p1results.txt", "a");

	fprintf (fp, "%d\t", NUMNODES);
	fprintf (fp, "%d\t", NUMTHREADS);
	fprintf (fp, "%f\t",volume);
	fprintf (fp, "%f\t",1000000.*timeElapsed);
	fprintf (fp, "%f\t",megaHeights);
	fprintf (fp, "\n");
	

	printf("Threads: %d\n", NUMTHREADS);
	printf("Nodes: %d\n", NUMNODES);
	printf("volume: %f\n", volume);
	printf("Elapsed Time=%10.2lf microseconds\n",1000000.*timeElapsed);
	printf("Average Performance = %8.2lf MegaHeights/Sec\n", megaHeights);
	printf("***** \n");
	return 0;
}
