/////////////////////////////////////////////////////////////////////////////
//
// Matthew Colliss
//
// Header for convolution
//
/////////////////////////////////////////////////////////////////////////////
#ifndef CONVOLUTION_H_
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#define CONVOLUTION_H_

void convolve(int target[],int kernal[],int kernalSize,int height,int width,IplImage *img);

#endif /* CONVOLUTION_H_ */
