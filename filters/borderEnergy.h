/////////////////////////////////////////////////////////////////////////////
//
// Matthew Colliss
//
// Header for border energy
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BORDERENRGY_H_
#include <cv.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../connectedComponentLabelling.h"
#include "convolution.h"
#define BORDERENRGY_H_

CCL_Object borderEnergyFilter(CCL_Object source,IplImage *img);

#endif /* SOBELFILTER_H_ */
