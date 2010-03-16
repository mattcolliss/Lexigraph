/////////////////////////////////////////////////////////////////////////////
//
// Matthew Colliss
// Local Adaptive Threshold
// Thresholds pixels based on mean of neighbour pixels
//
/////////////////////////////////////////////////////////////////////////////

#include "adaptiveThreshold.h"




//applies local adaptive threshold to the image, using mean value of window as threshold
void localAdaptiveThresholdSlow(IplImage *img,int windowHeight,int windowWidth)
{
	printf("performing local adaptive threshold...\n");
	//Initialise local variables
	int windowSize = windowHeight * windowWidth;
	int imageHeight = img->height;
	int imageWidth = img->width;
	uchar* data = (uchar *)img->imageData;
	int step = img->widthStep;
	uchar source[img->height][img->width];
	uchar target[img->height][img->width];
	//build matrix representation of image
	for(int i = 0; i < img->height;i++)
	{
		for(int j = 0; j < img->width; j++)
		{
			source[i][j] = data[(i*step) + j ];
			//printf("%i %i %u \n",i,j,test[i][j]);
		}
	}

	//for each pixel in image
	for(int i = 0; i < img->height;i++)
	{
		for(int j = 0; j < img->width; j++)
		{
			int sum = 0;

			//calculate sum of surrounding n x n pixels
			for(int p = i - (windowHeight/2); p <= i + (windowHeight/2); p++ )
			{
				for(int q = j - (windowWidth/2); q <= j + (windowWidth/2); q++ )
				{
					if(p >= 0 && p < imageHeight && q >= 0 && q < imageWidth)
					{
						sum += source[p][q];
					}
				}
			}
			//find the mean of these pixels
			sum = sum / windowSize;
			//threshold the pixel based on this sum
			if(source[i][j] < sum )
			{
				target[i][j] = 0;
			}
			else
			{
				target[i][j] = 255;
			}

		}
	}
	//copy new data into image
	for(int i = 0; i < img->height;i++)
	{
		for(int j = 0; j < img->width; j++)
		{
			data[(i*step) + j ] = target[i][j];
			//printf("%i %i %u \n",i,j,test[i][j]);
		}
	}
	img->imageData = (char*)data;

	printf("Done \n");
}

//more efficient version of above
void localAdaptiveThreshold(IplImage *img,int windowHeight,int windowWidth)
{
	printf("performing local adaptive threshold...\n");
	//Initialise local variables
	int windowSize = windowHeight * windowWidth;
	int imageHeight = img->height;
	int imageWidth = img->width;
	uchar* data = (uchar *)img->imageData;
	int step = img->widthStep;
	uchar source[img->height][img->width];
	uchar target[img->height][img->width];

	//build matrix representation of image
	for(int i = 0; i < img->height;i++)
	{
		for(int j = 0; j < img->width; j++)
		{
			source[i][j] = data[(i*step) + j ];
		}
	}

	int sum = 0;
	int average = 0;

	//for each pixel in image
	for(int i = 0; i < img->height;i++)
	{
		for(int j = 0; j < img->width; j++)
		{
			//if were in the first column calculate mean of neighbour pixels
			if(j == 0)
			{
				sum = 0;
				for(int p = i - (windowHeight/2); p <= i + (windowHeight/2); p++ )
				{
					for(int q = j - (windowWidth/2); q <= j + (windowWidth/2); q++ )
					{
						if(p >= 0 && p < imageHeight && q >= 0 && q < imageWidth)
						{
							sum += source[p][q];
						}
					}
				}
			}
			//else update mean from previous pixel
			else
			{
				for(int p = i - (windowHeight/2); p <= i + (windowHeight/2); p++ )
				{
					int q = (j - (windowWidth/2)) - 1;
					if(p >= 0 && p < imageHeight && q >= 0 && q < imageWidth)
					{
						sum -= source[p][q];
					}
					q = (j + (windowWidth/2));
					if(p >= 0 && p < imageHeight && q >= 0 && q < imageWidth)
					{
						sum += source[p][q];
					}
				}
			}

			//calculate average, and threshold pixel with average
			average = sum / windowSize;
			if(source[i][j] < average )
			{
				target[i][j] = 0;
			}
			else
			{
				target[i][j] = 255;
			}


			data[(i*step) + j ] = target[i][j];

		}
	}
	img->imageData = (char*)data;
	printf("Done \n");
}








