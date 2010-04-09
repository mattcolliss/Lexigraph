/*
 * convolution.h
 *
 *  Created on: 17 Mar 2010
 *      Author: matt
 */

#ifndef CONVOLUTION_H_
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#define CONVOLUTION_H_

void convolve(int target[],int kernal[],int kernalSize,int height,int width,IplImage *img);

#endif /* CONVOLUTION_H_ */
