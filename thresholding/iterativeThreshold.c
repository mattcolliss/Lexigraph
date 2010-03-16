/////////////////////////////////////////////////////////////////////////////
//
// Matthew Colliss
// Iterative Threshold
//
/////////////////////////////////////////////////////////////////////////////


#include "iterativeThreshold.h"


void iterativeThreshold(IplImage *img)
{
	printf("performing iterative threshold...\n");
	uchar* data = (uchar *)img->imageData;
	int imageSize = img->imageSize;
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

		//for each pixel, threshold based on initial threshold
		for(int i = 0;i < imageSize;i++)
		{
			if(data[i] <= oldThreshold)
			{
				backgroundSum += data[i];
				backgroundCount++;
			}
			else
			{
				foregroundSum += data[i];
				foregroundCount++;
			}

		}

		//update threshold to be mean of means
		if(backgroundCount != 0) backgroundAverage = backgroundSum / backgroundCount;
		if(foregroundCount != 0) foregroundAverage = foregroundSum / foregroundCount;
		newThreshold = (backgroundAverage + foregroundAverage) / 2;

	}

	//threshold image
	for(int i = 0;i < imageSize;i++)
	{
		if(data[i] <= newThreshold)
		{
			data[i] = 0;
		}
		else
		{
			data[i] = 255;
		}
	}
	//copy new data into image
	img->imageData = (char*)data;
	printf("Done \n");

}
