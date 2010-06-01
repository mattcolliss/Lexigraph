/////////////////////////////////////////////////////////////////////////////
//
// Matthew Colliss
//
// Implementaion main, main method from where everything else is run
//
/////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <cv.h>
#include <highgui.h>
#include <time.h>
#include "thresholding/adaptiveThreshold.h"
#include "thresholding/otsuThreshold.h"
#include "thresholding/iterativeThreshold.h"
#include "thresholding/localOtsuThreshold.h"
#include "thresholding/localIterativeThreshold.h"
#include "thresholding/repeatedIterativeThresholding.h"
#include "connectedComponentLabelling.h"
#include "filters/sizeFilter.h"
#include "filters/borderEnergy.h"
#include "filters/edgeContrst.h"
#include "filters/eigenTransform.h"
#include "grouping/perceptualGrouping.h"
#include "analysis.h"

//show an image for demonstratiion
void showImage(CCL_Object sourceData, char *tag)
{

	////////////////////////////////////////////////////
	int target[sourceData.height][sourceData.width];

	for(int i = 0;i < sourceData.height;i++)
	{
		for(int j = 0;j < sourceData.width;j++)
		{
			int pix = sourceData.labels[(i * sourceData.width) + j];
			if(pix != 0)
				target[i][j] =  255;
			else
				target[i][j] = 0;
		}
	}

	//create temp image to display results
	IplImage *temp = cvCreateImage(cvSize(sourceData.width,sourceData.height),IPL_DEPTH_8U,1);
	uchar* data = (uchar *)temp->imageData;
	//copy new data into image
	for(int i = 0; i < sourceData.height;i++)
	{
		for(int j = 0; j < sourceData.width; j++)
		{
			data[(i*temp->widthStep) + j ] = target[i][j];
			//printf("%i %i %u \n",i,j,test[i][j]);
		}
	}
	temp->imageData = (char*)data;

	cvSaveImage("output.png",temp);

	cvNamedWindow (tag, 1);
	cvShowImage (tag, temp);
	cvWaitKey (0);
	cvDestroyWindow(tag);

	cvReleaseImage (&temp);

}

int main (int argc, char *argv[])
{
	if(argc < 2)
	{
		printf("Usage: main <image-file-name>\n");
		exit(0);
	}

	printf("Reading file %s \n",argv[1]);

	//determine whether demo mode is activated
	int demo = 0;
	if(argc >2 && !strncmp(argv[2],"-d",2))
	{
		printf("Demonstration mode... \n");
		demo = 1;
	}

	//create an image
	IplImage *img = 0;
	// load an image
	img = cvLoadImage(argv[1],-1);
	if(!img)
	{
		printf("Could not load image file: %s\n",argv[1]);
		exit(0);
	}

	//convert image to grey scale - will later be converted to binary, hence name
	IplImage *binaryImg = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);
	cvCvtColor(img,binaryImg,CV_BGR2GRAY);

	//create an inverted grey scaled image, for dark on light text
	IplImage *invertedBinaryImg = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);
	cvCvtColor(img,invertedBinaryImg,CV_BGR2GRAY);

	//convert image to grey scale, keep this one for later
	IplImage *greyImage = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);
	cvCvtColor(img,greyImage,CV_BGR2GRAY);

	//create a grey scale image to pass to edge contrast to apply gaussian blur to
	IplImage *greyBlur = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);
	cvCvtColor(img,greyBlur,CV_BGR2GRAY);
	IplImage *greyBlur2 = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);
	cvCvtColor(img,greyBlur2,CV_BGR2GRAY);


	//clock_t start, finish;
	//start = clock();


	//Threshold the image
	//localAdaptiveThresholdSlow(binaryImg,45,45);

	//finish = clock();
	//float time = ( (finish - start) );
	//printf("%2.2f \n",time);


	//localOtsuThreshold(binaryImg);
	repeatedIterativeThreshold(binaryImg,invertedBinaryImg);
	//iterativeThreshold(binaryImg);
	//otsuThreshold(binaryImg);
	// a visualisation window is created with title 'image'
	if(demo > 0)
	{
		cvSaveImage("output.png",binaryImg);
		cvNamedWindow ("Lexigraph", 1);
		cvShowImage ("Lexigraph", binaryImg);
		cvWaitKey (0);
	}

	//CC labelling
	CCL_Object cclPositive, cclNegative;
	cclPositive = connectedComponentLabeling(binaryImg);
	cclNegative = connectedComponentLabeling(invertedBinaryImg);

	//first test, size of cc
	cclPositive = sizeFilter(cclPositive);
	cclNegative = sizeFilter(cclNegative);
	if(demo > 0) showImage(cclPositive,"Geometric Filters");

	//second test, border energy OR edge contrast
	//cclPositive = borderEnergyFilter(cclPositive,greyImage);
	//cclNegative = borderEnergyFilter(cclNegative,greyImage);
	cclPositive = edgeContrastFilter(cclPositive,greyBlur);
	cclNegative = edgeContrastFilter(cclNegative,greyBlur2);
	if(demo > 0) showImage(cclPositive,"Border Energy");

	//third test, eigen transform
	cclPositive = eigenTransform(cclPositive,greyImage);
	cclNegative = eigenTransform(cclNegative,greyImage);
	if(demo > 0)showImage(cclPositive,"Eigen");

	perceptualGrouping(cclPositive,cclNegative,greyImage);



	if(argc >3 && !strncmp(argv[2],"-r",2))
	{
		printf("Results analysis... \n");
		IplImage *groundTruth = 0;
		// load an image
		groundTruth = cvLoadImage(argv[3],-1);

		IplImage *greyGround = cvCreateImage(cvSize(groundTruth->width,groundTruth->height),IPL_DEPTH_8U,1);
		cvCvtColor(groundTruth,greyGround,CV_BGR2GRAY);
		//cvThreshold(greyGround,greyGround,100,255,CV_THRESH_BINARY);
		analyseResult(greyImage,greyGround);

	}




	// memory release for img before exiting the application
	cvReleaseImage (&img);
	cvReleaseImage (&binaryImg);
	cvReleaseImage (&greyImage);
	cvReleaseImage (&greyBlur);
	cvReleaseImage (&greyBlur2);


	//free malloc'd mem
	printf("Freeing memory...\n");
	free(cclPositive.labels);
	free(cclPositive.classSizes);
	free(cclPositive.minI);
	free(cclPositive.minJ);
	free(cclPositive.maxI);
	free(cclPositive.maxJ);
	printf("Done\n");
	//destory image window - if it was created for demo
	if(demo > 0) cvDestroyWindow("Lexigraph");

	return 0;
}
