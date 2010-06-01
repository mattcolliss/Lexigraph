/////////////////////////////////////////////////////////////////////////////
//
// Matthew Colliss
//
// Performs analysis of result accuracy
//
/////////////////////////////////////////////////////////////////////////////

#include "analysis.h"

void analyseResult(IplImage *result, IplImage *groundTruth)
{
	int height = result->height;
	int width = result->width;
	int widthStepResult= result->widthStep;
	int widthStepGround = groundTruth->widthStep;


	cvNamedWindow ("tag", 1);
		cvShowImage ("tag", result);
		cvSaveImage("Result.png",result);
		cvWaitKey (0);
		cvDestroyWindow("tag");

	uchar* resultsData = (uchar *)result->imageData;
	uchar* groundData = (uchar *)groundTruth->imageData;

	float hit = 0;
	float error = 0;
	float miss = 0;

	//for each pixel in result
	for(int i = 0; i <  height; i++)
	{
		for(int j = 0; j <  width; j++)
		{
			int r = resultsData[(i*widthStepResult) + j];
			int g = groundData[(i*widthStepGround) + j];

			if(r > 0) r = 255; else r = 0;
			if(g > 0) g = 255; else g = 0;
			//calculate no oh misses, errors etc
			//if(g!=0)printf("%i %i \n",r,g);
			if(r > 0 && g > 0) {hit++;}
			else if(r > 0 && g == 0) {error++;}
			else if(r == 0 && g > 0) {miss++;}

		}
	}

	//printf("%2.2f %2.2f %2.2f \n",hit,error,miss);

	float accuracy = hit / (hit + error);
	float recall = hit / (hit + miss);

	printf("%2.8f %2.8f  \n",accuracy,recall);
}
