/////////////////////////////////////////////////////////////////////////////
//
// Matthew Colliss
// Otsu Threshold
//
/////////////////////////////////////////////////////////////////////////////

#include "otsuThreshold.h"

int levels = 256;

/////////////////////////////////////////////////////////////////////////////
// Otsu algorithm based on code from 'The Lab Book Pages'
// at http://www.labbookpages.co.uk/software/imgProc/otsuThreshold.html
/////////////////////////////////////////////////////////////////////////////
void otsuThreshold(IplImage *img)
{

	printf("Performing otsu threshold...\n");

	uchar* data = (uchar *)img->imageData;
	int step = img->widthStep;

	int histogram[levels];
	for(int i = 0;i < levels;i++)
	{
		histogram[i] = 0;
	}

	//calculate histogram
	for(int i = 0; i < img->height;i++)
	{
		for(int j = 0; j < img->width; j++)
		{
			uchar pix = data[(i*step) + j ];
			histogram[pix]++;
			//printf("value = %u count = %i \n",pix,histogram[pix]);
		}
	}

	for(int i = 0;i < levels;i++)
	{
		printf("%i \n",histogram[i]);
	}

	//total number of pixels
	int total = (img->height * img->width);

	float sum = 0;
	for(int i = 0;i < levels;i++)
	{
		sum += i * histogram[i];
	}

	float backgroundSum = 0;
	int backgroundWeight = 0;
	int foregroundWeight = 0;

	float varMax = 0;
	int threshold = 0;

	for(int i = 0;i < levels;i++)
	{
		backgroundWeight += histogram[i];
		if(backgroundWeight == 0) continue;

		foregroundWeight = total - backgroundWeight;
		if (foregroundWeight == 0) break;

		backgroundSum += (float) (i * histogram[i]);

		//Background mean
		float backgroundMean = backgroundSum / backgroundWeight;
	    //Foreground mean
		float foregroundMean = (sum - backgroundSum) / foregroundWeight;

		// Calculate Between Class Variance
		float varBetween = (float) (backgroundWeight * foregroundWeight) * (backgroundMean - foregroundMean) * (backgroundMean - foregroundMean);

		// Check if new maximum found
		if (varBetween > varMax)
		{
			varMax = varBetween;
			threshold = i;
		}
	}

	//threshold image
	for(int i =0; i < img->imageSize; i++)
	{
		if(data[i] < threshold)
		{
			data[i] = 0;
		}
		else
		{
			data[i] = 255;
		}
	}
	//copy data
	img->imageData = (char*)data;
	printf("Done \n");

}
