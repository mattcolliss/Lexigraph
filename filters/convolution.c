/////////////////////////////////////////////////////////////////////////////
//
// Matthew Colliss
//
// 2d convolution with image and kernel
//
/////////////////////////////////////////////////////////////////////////////

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
	//iterate over all pixels
	int margin = kernalSize / 2;
	for(int i = margin;i < height-margin;i++)
	{
		for(int j = margin;j < width-margin;j++)
		{

			int sum = 0;
			// memory release for img before exiting the application
			for(int n = 0-margin;n <= 0 + margin; n++)
			{
				for(int m = 0-margin;m <= 0 + margin; m++)
				{

					if(m != 0 || n != 0)
					{
						sum += image[i + n][j + m] * kernal2D[margin - n][margin - m];
					}

				}
			}
			results[i][j] = sum;

		}
	}

	//copy results into target image
	for(int i = 0; i < height;i++)
	{
		for(int j = 0; j < width; j++)
		{
			target[(i * width) + j] = results[i][j];
		}
	}
}

