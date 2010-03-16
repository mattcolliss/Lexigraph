/*
 * sizeFilter.h
 *
 *  Created on: 12 Mar 2010
 *      Author: matt
 */

#ifndef SIZEFILTER_H_
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include "connectedComponentLabelling.h"
#define SIZEFILTER_H_

void sizeFilter(CCL_Object source);
void sizeFilter2(CCL_Object source);

#endif /* SIZEFILTER_H_ */
