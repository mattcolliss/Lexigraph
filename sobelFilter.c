
#include "sobelFilter.h"

#define SOBEL_THRESHOLD 100

CCL_Object sobelFilter(CCL_Object source,IplImage *img)
{
	printf("Filtering CCs by Sobel operator...\n");
	int height = source.height;
	int width = source.width;
	int imageSize = height * width;
	//convolve image by hX
	int gX[imageSize];
	int hX[9] = {1,0,-1,2,0,-2,1,0,-1};
	convolve(gX,hX,3,height,width,img);
	//convolve image by hY
	int gY[imageSize];
	int hY[9] = {1,2,1,0,0,0,-1,-2,-1};
	convolve(gY,hY,3,height,width,img);
	//combine results of gX and gY into G
	int G[imageSize];
	for(int i =0;i < imageSize;i++)
	{
		G[i] = sqrt((gX[i] * gX[i]) + (gY[i] * gY[i]));
	}

	//build 2d matrix of label values
	int labels2D[height][width];
	//for each pixel in labels
	for(int i = 0;i < height;i++)
	{
		for(int j = 0;j < width;j++)
		{
			labels2D[i][j] = source.labels[(i * width) + j];
		}
	}

	//list of average g values for each component
	int average[source.classCount];
	int contourCount[source.classCount];
	for(int i = 0;i < source.classCount;i++)
	{
		average[i] = 0;
		contourCount[i] = 0;

	}

	//for each pixel in labels - find sum of G values
	for(int i = 1;i < height - 1;i++)
	{
		for(int j = 1;j < width - 1;j++)
		{
			//if this pixel is part of a region i.e non-zero
			if(labels2D[i][j] != 0)
			{
				int isContour = 0;
				//check if this is a contour pixel
				for(int n = i - 1;n <= i + 1;n++)
				{
					for(int m = j - 1; m <= j + 1;m++)
					{

						if((n != i || m != j) && labels2D[n][m] != labels2D[i][j])
						{
							isContour = 1;

						}
					}
				}

				if(isContour)
				{
					average[labels2D[i][j]] += G[(i * width) + j];
					contourCount[labels2D[i][j]]++;
				}
			}

		}
	}

	//calculate average G value
	for(int i = 0;i < source.classCount;i++)
	{
		if(contourCount[i] != 0)
		{
			average[i] = average[i] / contourCount[i];
		}
	}

	for(int i = 0;i < source.classCount;i++)
	{
		printf("%i \n",average[i]);
		if(average[i] < SOBEL_THRESHOLD && average[i] != 0)
		{
			source.classSizes[i] = 0;
			source.minI[i] = 9999;
			source.minJ[i] = 9999;
			source.maxI[i] = -1;
			source.maxJ[i] = -1;
		}
	}

	for(int i = 0;i < imageSize;i++)
	{
		int size = source.classSizes[source.labels[i]];
		if(size == 0)
		{
			source.labels[i] = 0;
		}
	}


	/*///////////////////////////////////////////////

	for(int i =0;i < imageSize;i++)
	{
	  if(G[i] > 255) G[i] = 255;
	}

	uchar* data = (uchar *)img->imageData;
	int step = img->widthStep;
	//copy new data into image
	for(int i = 0; i < img->height;i++)
	{
		for(int j = 0; j < img->width; j++)
		{
			data[(i*step) + j ] = G[(i * width) + j];
			//printf("%i %i %u \n",i,j,test[i][j]);
		}
	}

	img->imageData = (char*)data;
	// a visualization window is created with title 'image'
	cvNamedWindow ("image2", 1);
	// img is shown in 'image' window
	cvShowImage ("image2", img);


	// wait for infinite delay for a keypress
	cvWaitKey (0);
	cvDestroyWindow("image2");
	 */




	printf("Done\n");
	return source;
}
