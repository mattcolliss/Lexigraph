/////////////////////////////////////////////////////////////////////////////
//
// Matthew Colliss
//
// Header for size filter
//
/////////////////////////////////////////////////////////////////////////////
#ifndef SIZEFILTER_H_
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include "../connectedComponentLabelling.h"
#define SIZEFILTER_H_

CCL_Object sizeFilter(CCL_Object source);

#endif /* SIZEFILTER_H_ */
