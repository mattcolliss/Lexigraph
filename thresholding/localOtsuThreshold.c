/////////////////////////////////////////////////////////////////////////////
//
// Matthew Colliss
// Otsu Threshold
// Performs Otsu thresholding on image
//
/////////////////////////////////////////////////////////////////////////////

#include "localOtsuThreshold.h"

#define windowHeight  17
#define windowWidth  17
#define levels 255




/////////////////////////////////////////////////////////////////////////////
// Otsu algorithm based on code from 'The Lab Book Pages'
// at http://www.labbookpages.co.uk/software/imgProc/otsuThreshold.html
/////////////////////////////////////////////////////////////////////////////
int otsuThresholdLocal(uchar ROI[windowHeight][windowWidth] )
{
	//initialise histogram
	int histogram[levels];
	for(int i = 0;i < levels;i++)
	{
		histogram[i] = 0;
	}

	//calculate histogram
	for(int i = 0; i < windowHeight;i++)
	{
		for(int j = 0; j < windowWidth; j++)
		{
			uchar pix = ROI[i][j];
			histogram[pix]++;
			//printf("value = %u count = %i \n",pix,histogram[pix]);
		}
	}

	//total number of pixels
	int total = windowHeight * windowWidth;

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
		//Mean background
		float backgroundMean = backgroundSum / backgroundWeight;
		//Mean foreground
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

	//printf("threshold = %i \n",threshold);

	return threshold;
}

void localOtsuThreshold(IplImage *img)
{

	printf("Performing local otsu threshold...\n");

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

			//Otsu threshold ROI
			int threshold = otsuThresholdLocal(ROI);


			//threshold this pixel based on ostu of ROI
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


