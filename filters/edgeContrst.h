/*
 * edgeContrst.h
 *
 *  Created on: 15 May 2010
 *      Author: matt
 */

#ifndef EDGECONTRST_H_
#include <cv.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../connectedComponentLabelling.h"
#include "convolution.h"
#define EDGECONTRST_H_

CCL_Object edgeContrastFilter(CCL_Object source,IplImage *img);


#endif /* EDGECONTRST_H_ */
