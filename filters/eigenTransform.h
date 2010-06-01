/////////////////////////////////////////////////////////////////////////////
//
// Matthew Colliss
//
// Header for eigen transform
//
/////////////////////////////////////////////////////////////////////////////


#ifndef EIGENTRANSFORM_H_
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../connectedComponentLabelling.h"
#define EIGENTRANSFORM_H_

CCL_Object eigenTransform(CCL_Object source,IplImage *img);

#endif /* EIGENTRANSFORM_H_ */
