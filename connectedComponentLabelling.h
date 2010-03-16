/////////////////////////////////////////////////////////////////////////////
//
// Matthew Colliss
//
//
/////////////////////////////////////////////////////////////////////////////

#ifndef CONNECTEDCOMPONENTLABELING_H_
#include <cv.h>
#include <stdio.h>
#include <stdlib.h>
#define CONNECTEDCOMPONENTLABELING_H_


typedef struct
{
	int height;
	int width;
	int *labels;
	int classCount;
	int *classSizes;
	int *minI;
	int *minJ;
	int *maxI;
	int *maxJ;

} CCL_Object;

CCL_Object connectedComponentLabeling(IplImage *img);

#endif /* CONNECTEDCOMPONENTLABELING_H_ */
