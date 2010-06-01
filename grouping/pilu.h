/////////////////////////////////////////////////////////////////////////////
//
// Matthew Colliss
//
// Header for pilu
//
/////////////////////////////////////////////////////////////////////////////

#ifndef PILU_H_
#include <cv.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PILU_H_



typedef struct
{
	int i;
	int j;

} Point;

float piluOperator(Point aTop,Point aBottom,Point bTop,Point bBottom);


#endif /* PILU_H_ */


