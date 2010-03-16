/////////////////////////////////////////////////////////////////////////////
//
// Matthew Colliss
// Repeated Iterative Thresholding
// Applies iterative thresholding with n different initial thresholds,
// selecting the one that gives the most uniform distribution.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef REPEATEDITERATIVETHRESHOLD_H_
#include <cv.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define REPEATEDITERATIVETHRESHOLD_H_


void repeatedIterativeThreshold(IplImage *img);

#endif /* REPEATEDITERATIVETHRESHOLD_H_ */
