



#include "edgeContrst.h"

#define PI 3.141592

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


	//apply gaussian blur to image
	cvSmooth( img, img, CV_GAUSSIAN,5,5,0,0);

	//Find sobel edges
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
	int theta[imageSize];


	for(int i =0;i < imageSize;i++)
	{
		G[i] = sqrt((gX[i] * gX[i]) + (gY[i] * gY[i]));

		double thetaTemp = 0;
		if(gX[i] != 0)
		{
			thetaTemp = atan(gY[i] / gX[i]);
			thetaTemp = (thetaTemp * 180) / PI;

			//printf("%2.5f \n",thetaTemp);
		}

		if(thetaTemp < -45) theta[i] = 45;
		else if(thetaTemp >= -45 && thetaTemp < 0) theta[i] = 90;
		else if(thetaTemp >= 0 && thetaTemp < 45) theta[i] = 135;
		else if(thetaTemp >= 45 && thetaTemp < 90) theta[i] = 0;
	}

	//build 2d array to hold canny results
	int canny[height][width];
	for(int i = 0;i < height;i++)
	{
		for(int j = 0;j < width;j++)
		{
			canny[i][j] = theta[(i * width) + j];
		}
	}
	//find edges
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


	/*/////////////////

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

	*///////////////////////////






	printf("Done \n");
	return source;
}
