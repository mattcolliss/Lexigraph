/////////////////////////////////////////////////////////////////////////////
//
// Matthew Colliss
// Local Iterative Threshold
// Performs local iterative thresholding on image
//
/////////////////////////////////////////////////////////////////////////////

#include "localIterativeThreshold.h"

#define windowHeight  17
#define windowWidth  17
#define levels 255


int iterativeThresholdLocal(uchar ROI[windowHeight][windowWidth])
{

	//randomly select initial threshold
	srand((unsigned int)time(NULL));
	int newThreshold = rand() % 255;
	int oldThreshold = 0;

	while(newThreshold != oldThreshold)
	{

		//initialise variables
		int backgroundSum = 0;
		int backgroundCount = 0;
		int backgroundAverage = 0;
		int foregroundSum = 0;
		int foregroundCount = 0;
		int foregroundAverage = 0;

		oldThreshold = newThreshold;
		//for each pixel, threshold with initial value
		for(int i = 0;i < windowHeight;i++)
		{
			for(int j = 0;j < windowWidth;j++)
			{
				uchar pix = ROI[i][j];

				if(pix <= oldThreshold)
				{
					backgroundSum += pix;
					backgroundCount++;
				}
				else
				{
					foregroundSum += pix;
					foregroundCount++;
				}
			}

		}
		//new threshold is mean of means of eahc group
		if(backgroundCount != 0) backgroundAverage = backgroundSum / backgroundCount;
		if(foregroundCount != 0) foregroundAverage = foregroundSum / foregroundCount;
		newThreshold = (backgroundAverage + foregroundAverage) / 2;

	}
	return newThreshold;
}

void localIterativeThreshold(IplImage *img)
{

	printf("Performing local iterative threshold...\n");

	//Initialise local variables
	int imageHeight = img->height;
	int imageWidth = img->width;
	uchar* data = (uchar *)img->imageData;
	int step = img->widthStep;
	uchar source[img->height][img->width];
	uchar target[img->height][img->width];
	uchar ROI[windowHeight][windowWidth];
	//build matrix representation of image
	for(int i = 0; i < img->height;i++)
	{
		for(int j = 0; j < img->width; j++)
		{
			source[i][j] = data[(i*step) + j ];
			//printf("%i %i %u \n",i,j,source[i][j]);
		}
	}

	//for each pixel
	for(int i = 0; i < img->height;i++)
	{
		for(int j = 0; j < img->width; j++)
		{
			//copy NxN neighbour pixels into ROI
			int a = 0;
			int b = 0;
			for(int p = i - (windowHeight/2); p <= i + (windowHeight/2); p++ )
			{
				b = 0;
				for(int q = j - (windowWidth/2); q <= j + (windowWidth/2); q++ )
				{
					if(p >= 0 && p < imageHeight && q >= 0 && q < imageWidth)
					{
						ROI[a][b] = source[p][q];
					}
					else
					{
						ROI[a][b] = 0;
					}

					b++;
				}
				a++;
			}

			//iterative threshold ROI
			int threshold = iterativeThresholdLocal(ROI);


			//threshold this pixel based on iterative threshold of ROI
			if(source[i][j] < threshold )
			{
				target[i][j] = 0;
			}
			else
			{
				target[i][j] = 255;
			}

		}
	}

	//copy new data back into image
	for(int i = 0; i < img->height;i++)
	{
		for(int j = 0; j < img->width; j++)
		{
			data[(i*step) + j ] = target[i][j];
		}
	}
	img->imageData = (char*)data;
	printf("Done\n");
}


