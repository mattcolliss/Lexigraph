



#include "edgeContrst.h"

#define PI 3.141592
#define EDGELIMIT 0.75

int getPixel(IplImage *img,int i,int j)
{
	uchar* data = (uchar *)img->imageData;
	int step = img->widthStep;

	if(i < 0 || j < 0 || i >= img->height || j >= img->width) return 0;

	return data[(i*step) + j ];
}


CCL_Object edgeContrastFilter(CCL_Object source,IplImage *img)
{
	printf("Filtering CCs by edge contrast...\n");

	int height = source.height;
	int width = source.width;
	int imageSize = height * width;

	//Find sobel edges
	int *gX = (int*)malloc(imageSize * sizeof (int));
	int hX[9] = {1,0,-1,2,0,-2,1,0,-1};
	convolve(gX,hX,3,height,width,img);
	//convolve image by hY
	int *gY = (int*)malloc(imageSize * sizeof (int));
	int hY[9] = {1,2,1,0,0,0,-1,-2,-1};
	convolve(gY,hY,3,height,width,img);
	//combine results of gX and gY into G
	int *G = (int*)malloc(imageSize * sizeof (int));
	for(int i =0;i < imageSize;i++)
	{
		G[i] = sqrt((gX[i] * gX[i]) + (gY[i] * gY[i]));
		//printf("%i \n",G[i]);
	}
	//no longer need gX or gY, so free them
	free(gX);
	free(gY);




	//Find canny edges
	//apply gaussian blur to image
	cvSmooth( img, img, CV_GAUSSIAN,7,7,0,0);
	//Find sobel edges of smoothed version
	int *gXCanny = (int*)malloc(imageSize * sizeof (int));
	convolve(gXCanny,hX,3,height,width,img);
	int *gYCanny = (int*)malloc(imageSize * sizeof (int));
	convolve(gYCanny,hY,3,height,width,img);
	// combine gx and gy to magnitude and angle of gradient
	int *GCanny = (int*)malloc(imageSize  * sizeof (int));
	int *theta = (int*)malloc(imageSize *  sizeof(int));


	for(int i =0;i < imageSize;i++)
	{
		// G = root(gx^2 + gy^2)
		GCanny[i] = sqrt((gXCanny[i] * gXCanny[i]) + (gYCanny[i] * gYCanny[i]));
		//printf("%i \n",GCanny[i]);
		//find angle
		if(gXCanny[i] == 0) //special case when gX = 0
		{
			if(gYCanny[i] == 0) theta[i] = 0;
			else theta[i] = 90;
		}
		else //otherwise Theta = inverseTan(gy / gx)
		{
			float thetaRad = atan(gYCanny[i] / gXCanny[i]);
			thetaRad = (thetaRad * 180) / PI;
			if(thetaRad < 0) thetaRad += 180;
			//printf("%2.4f \n",thetaRad);  22.5 to 67.5 degrees
			if((thetaRad >= 0 &&thetaRad < 22.5) || (thetaRad >= 157.5)) theta[i] = 0;
			else if(thetaRad >= 22.5 && thetaRad < 67.5) theta[i] = 45;
			else if(thetaRad >= 67.5 && thetaRad < 112.5) theta[i] = 90;
			else if(thetaRad >= 122.5 && thetaRad < 157.5) theta[i] = 135;
		}
		//printf("%i \n",theta[i]);
	}

	//no longer need gX, gY etc so free them
	free(gXCanny);
	free(gYCanny);
	free(GCanny);


	//build 2d array to hold canny results
	int canny[height][width];
	for(int i = 0;i < height;i++)
	{
		for(int j = 0;j < width;j++)
		{
			canny[i][j] = theta[(i * width) + j];
		}
	}

	//done with theta so free it
	free(theta);



	//find edges non-maximal
	for(int i = 0;i < height;i++)
	{
		for(int j = 0;j < width;j++)
		{
			int pixel = getPixel(img,i,j);
			//printf("%i ",pixel);
			if(canny[i][j] == 0)
			{
				int north = getPixel(img,i-1,j);
				int south = getPixel(img,i+1,j);
				if(pixel > north && pixel > south)
				{
					canny[i][j] = 1;
				}
				else
				{
					canny[i][j] = 0;
				}
			}
			else if(canny[i][j] == 45)
			{
				int northWest = getPixel(img,i-1,j-1);
				int southEast = getPixel(img,i+1,j+1);
				if(pixel > northWest && pixel > southEast)
				{
					canny[i][j] = 1;
				}
				else
				{
					canny[i][j] = 0;
				}
			}
			else if(canny[i][j] == 90)
			{
				int west = getPixel(img,i,j-1);
				int east = getPixel(img,i,j+1);
				if(pixel > west && pixel > east)
				{
					canny[i][j] = 1;
				}
				else
				{
					canny[i][j] = 0;
				}
			}
			else if(canny[i][j] == 135)
			{
				int northEast = getPixel(img,i-1,j+1);
				int southWest = getPixel(img,i+1,j-1);
				if(pixel > northEast && pixel > southWest)
				{
					canny[i][j] = 1;
				}
				else
				{
					canny[i][j] = 0;
				}
			}

		}

	}


	//Find union of canny and sobel
	for(int i = 0;i < height;i++)
	{
		for(int j = 0;j < width;j++)
		{
			//printf("%i \n",G[i * width + j]);
			if(canny[i][j] > 0 || G[i * width + j] > 90)
			{
				canny[i][j] = 1;
			}
			else
			{
				canny[i][j] = 0;
			}
		}
	}

	//done with G now, so free it
	free(G);



	//calculate edge contrast
	//list of contour count and countour hits
	float hits[source.classCount];
	int contourCount[source.classCount];
	for(int i = 0;i < source.classCount;i++)
	{
		hits[i] = 0;
		contourCount[i] = 0;
	}

	//build 2d matrix of label values
	int labels2D[height][width];
	//for each pixel in labels
	for(int i = 0;i < height;i++)
	{
		for(int j = 0;j < width;j++)
		{
			labels2D[i][j] = source.labels[(i * width) + j];
			//printf("%i ",labels2D[i][j]);
		}
		//printf("\n");
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
					contourCount[labels2D[i][j]]++;
					//check if this pixel is on an edge
					if(canny[i][j] > 0) hits[labels2D[i][j]]++;
				}
			}

		}
	}

	//calculate ratio of hits to contour pixels for each component
	for(int i = 0;i < source.classCount;i++)
	{
		//
		if(hits[i] != 0)
		{
			//
			if(contourCount[i] != 0)
			{
				hits[i] = hits[i] / contourCount[i];
				//printf("%2.4f \n",hits[i]);
			}
		}
	}

	for(int i = 0;i < source.classCount;i++)
	{
		//printf("%i \n",average[i]);
		if(hits[i] < EDGELIMIT && hits[i] != 0)
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


	/*////////////////

	uchar* data = (uchar *)img->imageData;
	int step = img->widthStep;
	//copy new data into image
	for(int i = 0; i < img->height;i++)
	{
		for(int j = 0; j < img->width; j++)
		{
			data[(i*step) + j ] = canny[i][j];   ///also need to change to 255
			//data[(i*step) + j ] = G[i *  width + j];
			//printf("%i %i %u \n",i,j,test[i][j]);
		}
	}

	img->imageData = (char*)data;
	// a visualization window is created with title 'image'

	cvSaveImage("canny.jpg",img);
	cvNamedWindow ("image2", 1);
	// img is shown in 'image' window
	cvShowImage ("image2", img);


	// wait for infinite delay for a keypress
	cvWaitKey (0);
	cvDestroyWindow("image2");

	/*//////////////////////////






	printf("Done \n");
	return source;
}
