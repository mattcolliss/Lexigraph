/////////////////////////////////////////////////////////////////////////////
//
// Matthew Colliss
//
//
/////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <cv.h>
#include <highgui.h>
#include "thresholding/adaptiveThreshold.h"
#include "thresholding/otsuThreshold.h"
#include "thresholding/iterativeThreshold.h"
#include "thresholding/localOtsuThreshold.h"
#include "thresholding/localIterativeThreshold.h"
#include "thresholding/repeatedIterativeThresholding.h"
#include "connectedComponentLabelling.h"
#include "sizeFilter.h"

int main (int argc, char *argv[])
{
	if(argc<2)
	{
		printf("Usage: main <image-file-name>\n");
		exit(0);
	}

	printf("%s \n",argv[1]);

	//create an image
	IplImage *img = 0;
	// load an image
	img = cvLoadImage(argv[1],-1);
	if(!img)
	{
		printf("Could not load image file: %s\n",argv[1]);
		exit(0);
	}

	//convert image to grey scale
	IplImage *img2 = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);
	cvCvtColor(img,img2,CV_BGR2GRAY);

	//Threshold the image
	//localAdaptiveThreshold(img2,15,15);
	//otsuThreshold(img2);
	//iterativeThreshold(img2);
	//TODO: debug this
	//localOtsuThreshold(img2);
	//localIterativeThreshold(img2);
	repeatedIterativeThreshold(img2);

	//CC labelling
	CCL_Object cclPositive;
	cclPositive = connectedComponentLabeling(img2);

	//first test, size of cc
	//sizeFilter(cclPositive);
	sizeFilter2(cclPositive);



	//save the image
	cvSaveImage("output.jpg",img2);

	// a visualization window is created with title 'image'
	cvNamedWindow ("image", 1);
	// img is shown in 'image' window
	cvShowImage ("image", img2);


	// wait for infinite delay for a keypress
	cvWaitKey (0);
	// memory release for img before exiting the application
	cvReleaseImage (&img);
	cvReleaseImage (&img2);
	//free malloc'd mem
	printf("Freeing memory...\n");
	free(cclPositive.labels);
	free(cclPositive.classSizes);
	free(cclPositive.minI);
	free(cclPositive.minJ);
	free(cclPositive.maxI);
	free(cclPositive.maxJ);
	printf("Done\n");
	//destory image window
	cvDestroyWindow("image");

	return 0;
}
