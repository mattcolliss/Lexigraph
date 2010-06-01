/////////////////////////////////////////////////////////////////////////////
//
// Matthew Colliss
//
// Header for perceptual group
//
/////////////////////////////////////////////////////////////////////////////

#ifndef PERCEPTUALGROUPING_H_
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../connectedComponentLabelling.h"
#include "pilu.h"
#define PERCEPTUALGROUPING_H_

typedef struct
{
	int size;
	Point top;
	Point bottom;
	Point centre;
	Point next;
	int linked;
	int group;
} Blob;

typedef struct
{
	Point top;
	Point bottom;
	int size;
} BBox;

void perceptualGrouping(CCL_Object positiveSource,CCL_Object negativeSource,IplImage *img);

#endif /* PERCEPTUALGROUPING_H_ */

