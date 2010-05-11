/////////////////////////////////////////////////////////////////////////////
//
// Matthew Colliss
//
//
/////////////////////////////////////////////////////////////////////////////


#include "perceptualGrouping.h"
#include "pilu.h"

void perceptualGrouping()
{
	printf("Performing perceptual grouping...\n");

	Point aMin,aMax,bMin,bMax;
	aMin.i = 1;
	aMin.j = 1;
	aMax.i = 10;
	aMax.j = 10;

	bMin.i = 115;
	bMin.j = 115;
	bMax.i = 1120;
	bMax.j = 1120;



	piluOperator(aMin,aMax,bMin,bMax);

	printf("Done\n");
}
