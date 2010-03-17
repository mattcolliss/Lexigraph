/*
 * sobelFilter.h
 *
 *  Created on: 17 Mar 2010
 *      Author: matt
 */

#ifndef SOBELFILTER_H_
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include "connectedComponentLabelling.h"
#include "convolution.h"
#define SOBELFILTER_H_

CCL_Object sobelFilter(CCL_Object source,IplImage *img);

#endif /* SOBELFILTER_H_ */
