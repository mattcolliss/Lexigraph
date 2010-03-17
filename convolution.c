/*
 * convolution.c
 *
 *  Created on: 17 Mar 2010
 *      Author: matt
 */

#include "convolution.h"

void convolve(int target[],int kernal[],int kernalSize,int height,int width,IplImage *img)
{
	//build 2d version if kernal
	int kernal2D[kernalSize][kernalSize];
	for(int i = 0; i < kernalSize;i++)
	{
		for(int j = 0; j < kernalSize;j++)
		{
			kernal2D[i][j] = kernal[(i * kernalSize) + j];
		}
	}
	//create 2d matrix to temp hold results of convultion
	int results[height][width];
	//build 2d matrix rep of image data
	//Initialise local variables
	uchar* data = (uchar *)img->imageData;
	int step = img->widthStep;
	int image[height][width];
	//build binary matrix representation of image
	for(int i = 0; i < height;i++)
	{
		for(int j = 0; j < width; j++)
		{
			image[i][j] = data[(i*step) + j ];
			//initialise results array while looping
			results[i][j] = 0;
		}

	}







}
