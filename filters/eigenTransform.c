/*
 * convolution.c
 *
 *  Created on: 31 Mar 2010
 *      Author: matt
 */



#include "eigenTransform.h"

#define MAXW 55
#define MINW 3
#define SAMPLES 10
#define EIGENLIMIT 15

int inRange(int i,int j,int height,int width)
{
	if(i >= 0 && i < height && j >= 0 && j < width)
	{
		return 1;
	}

	return 0;
}

int generateEigenValue(int i,int j,int height,int width,int source[],int W)
{


	if(W > MAXW) W = MAXW;
	if(W <= MINW) W = MINW;
	if(W % 2 == 0) W++;
	//printf("%i \n",W);

	float window[W * W];
	int c = 0;
	int L = W / 3;

	//populate window with W * W neighbour pixels of i,j
	for(int p = i - W/2; p <= i + W/2; p++)
	{
		for(int q = j - W/2; q <= j + W/2; q++)
		{
			if(inRange(p,q,height,width))
			{
				window[c] = source[(p * width) + q];
			}
			else
			{
				window[c] = 0;
			}
			c++;
		}
	}

	//make window a symetrical matrix
	for(int i = 0;i < W;i++)
	{
		for(int j = 0;j < W;j++)
		{
			if(i > j)
			{
				window[(i * W) + j] = window[(j * W) + i];
			}
			//printf("%4.2f ",window[(i * W) + j]);
		}
		//printf("\n");
	}

	//calculate eigen transform of window
	//create cv matrix - dims,type,source
	CvMat mat;
	mat = cvMat(W, W, CV_32FC1, window);

	CvMat* E  = cvCreateMat(W,W,CV_32FC1); //matrix to get vecs
	CvMat* l  = cvCreateMat(W,1,CV_32FC1); //matrix to get vals

	cvEigenVV(&mat, E, l,1e-300);


	float values[W];
	int index = 0;

	//reverse order and take absolute value
	for(i=W-1;i >= 0; i--)
	{
		//float x = cvmGet(l,i,0);
		values[index] = cvmGet(l,i,0);
		if(values[index] < 0) values[index]  = 0 - values[index];

		index++;
	}

	float sum = 0;
	for(int i= 0;i<W-L;i++)
	{
		//printf("%4.2f \n",values[i]);
		sum += values[i];
	}


	//average smallest w - l + 1 values
	int divisor = (W - L) + 1;
	//printf("%2.2f %2.2f \n",sum,sum/divisor);
	float average = sum / divisor;


	//release opencv matrix objects
	cvReleaseMat(&E);
	cvReleaseMat(&l);
	//cvReleaseMat(&mat);

	return average;

}


CCL_Object eigenTransform(CCL_Object source,IplImage *img)
{
	printf("Filtering CCs by eigen transform...\n");


	int width =  source.width;
	int height = source.height;
	int imageSize = width * height;

	int imageArray[imageSize];
	uchar* data = (uchar *)img->imageData;
	int step = img->widthStep;
	//build binary matrix representation of image
	for(int i = 0; i < height;i++)
	{
		for(int j = 0; j < width; j++)
		{
			imageArray[(i * width) + j] = data[(i*step) + j ];
		}
	}


	/*
	for(int i = 0; i < height;i++)
	{
		for(int j = 0; j < width; j++)
		{
			int label  = source.labels[(i * width) + j];
			if(label != 0)
			{
				int regionHeight = (source.maxI[label] - source.minI[label]) + 1;
;				data[(i*step) + j ] = generateEigenValue(i,j,height,width,imageArray,regionHeight);
			}
			else
			{
				data[(i*step) + j ] = 0;
			}
		}
	}*/


	//seed randon numbers
	srand((unsigned int)time(NULL));

	int results[source.classCount];

	for(int c = 0;c < source.classCount;c++)
	{
		if(source.classSizes[c] != 0)
		{
			int rangeI = (source.maxI[c] - source.minI[c]) + 1;
			int rangeJ = (source.maxJ[c] - source.minJ[c]) + 1;
			int regionHeight = (source.maxI[c] - source.minI[c]) + 1;

			float eigenValueSum = 0;

			for(int i = 0;i < SAMPLES;i++)
			{
				//select random point in range from min
				int pointI = source.minI[c] + (rand() % rangeI);
				int pointJ = source.minJ[c] + (rand() % rangeJ);
				eigenValueSum += generateEigenValue(pointI,pointJ,height,width,imageArray,regionHeight);
			}
			//printf("%2.2f \n",eigenValueSum/SAMPLES);
			eigenValueSum = eigenValueSum / SAMPLES;

			if(eigenValueSum >= EIGENLIMIT) results[c] = 1;
			else results[c] = 0;

			//TODO: break CC entry here


		}
	}

	for(int i = 0; i < height;i++)
	{
		for(int j = 0; j < width; j++)
		{
			int label  = source.labels[(i * width) + j];
			if(results[label] == 0) source.labels[(i * width) + j] = 0;
		}
	}


	//img->imageData = data;

	printf("Done \n");
	return(source);
}
