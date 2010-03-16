/////////////////////////////////////////////////////////////////////////////
//
// Matthew Colliss
// Local Adaptive Threshold
// Thresholds pixels based on mean of neighbour pixels
//
/////////////////////////////////////////////////////////////////////////////

#ifndef ADAPTIVETHRESHOLD_H_
#include <cv.h>
#include <stdio.h>
#include <stdlib.h>
#define ADAPTIVETHRESHOLD_H_

void localAdaptiveThresholdSlow(IplImage *img,int windowHeight,int windowWidth);
void localAdaptiveThreshold(IplImage *img,int windowHeight,int windowWidth);

#endif /* ADAPTIVETHRESHOLD_H_ */
