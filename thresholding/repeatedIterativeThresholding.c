/////////////////////////////////////////////////////////////////////////////
//
// Matthew Colliss
// Repeated Iterative Thresholding
// Applies iterative thresholding with n different initial thresholds,
// selecting the one that gives the most uniform distribution.
//
/////////////////////////////////////////////////////////////////////////////

#include "repeatedIterativeThresholding.h"

#define repeats 10

//Structure to hold threshold and corresponding ratio
typedef struct
{
	int threshold;
	float ratio;
} Result;


Result iterativeThresholdRep(int intialThreshold,uchar* data,int imageSize)
{
	Result currentResult;

	int oldThreshold = 0;
	int newThreshold = intialThreshold;
	float backgroundCount = 0;
	float foregroundCount = 0;

	while(newThreshold != oldThreshold)
	{
		//initialise variables
		int backgroundSum = 0;
		backgroundCount = 0;
		int backgroundAverage = 0;
		int foregroundSum = 0;
		foregroundCount = 0;
		int foregroundAverage = 0;

		oldThreshold = newThreshold;
		//for each pixel, threshold on initial value
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
		//new threshold is mean of means of each group
		if(backgroundCount != 0) backgroundAverage = backgroundSum / backgroundCount;
		if(foregroundCount != 0) foregroundAverage = foregroundSum / foregroundCount;
		newThreshold = (backgroundAverage + foregroundAverage) / 2;
	}

	//calculate ratio of FG and BG pixels
	currentResult.threshold = newThreshold;
	if(foregroundCount > backgroundCount && backgroundCount != 0)
	{
		currentResult.ratio = foregroundCount / backgroundCount;
	}
	else if(foregroundCount != 0)
	{
		currentResult.ratio = backgroundCount / foregroundCount;
	}
	else
	{
		//ratio is infinite
		currentResult.ratio = 999999;
	}


	return currentResult;
}

void repeatedIterativeThreshold(IplImage *img)
{
	printf("Performing repeated iterative thresholding...\n");
	int intialValues[repeats];
	Result results[repeats];
	uchar* data = (uchar *)img->imageData;
	int imageSize = img->imageSize;

	//generate n random starting thresholds
	//threshold with that value
	//get back ratio of pixels
	srand((unsigned int)time(NULL));
	for(int i = 0;i < repeats;i++)
	{
		//generate random no
		intialValues[i] = rand() % 255;
		//threshold with this value
		results[i] = iterativeThresholdRep(intialValues[i],data,imageSize);
		//printf("%i %4.2f \n",results[i].threshold,results[i].ratio);
	}

	//now select threshold with lowest ratio
	float bestRatio = 999999;
	int bestThreshold = 0;
	for(int i = 0;i < repeats;i++)
	{
		if(results[i].ratio < bestRatio)
		{
			bestRatio = results[i].ratio;
			bestThreshold = results[i].threshold;
		}
	}

	//now threshold image accordingly
	for(int i = 0;i < imageSize;i++)
	{

		//TODO: change this back
		if(data[i] >= bestThreshold)
		{
			data[i] = 0;
		}
		else
		{
			data[i] = 255;
		}
	}
	img->imageData = (char*)data;

	printf("Done\n");
}

