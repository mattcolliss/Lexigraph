#include "sizeFilter.h"

#define MIN_SIZE 50
#define MAX_SIZE 700


void buildCloseList(int close[],CCL_Object source)
{
	int labels[source.height][source.width];
	int width =  source.width;
	int height = source.height;

	for(int i = 0;i < source.classCount;i++)
	{
		close[i] = 0;
	}

	for(int i = 0;i < height;i++)
	{
		for(int j = 0;j < width;j++)
		{
			labels[i][j] = source.labels[(i * width) + j];
		}
	}

	for(int i = 1;i < height;i++)
	{
		for(int j = 1;j < width;j++)
		{
			if(labels[i][j] != 0 && close[labels[i][j]] == 0)
			{
				for(int a = i - 1;a <= i + 1;a++)
				{
					for(int b = j - 1;b <= j + 1;b++)
					{
						if(labels[a][b] != 0 && labels[a][b] != labels[i][j])
						{
							close[labels[i][j]] = 1;
							break;
						}
					}
				}
			}
		}
	}
}

void sizeFilter2(CCL_Object source)
{
	printf("Filtering CCs by size...\n");
	int width =  source.width;
	int height = source.height;
	int close[source.classCount];
	buildCloseList(close,source);


	int target[height][width];

	for(int i = 0;i < height;i++)
		{
			for(int j = 0;j < width;j++)
			{
				int pix = source.labels[(i * width) + j];
				//printf("%i \n",source.classSizes[pix]);
				int size = source.classSizes[pix];
				if(size > MIN_SIZE && size < MAX_SIZE)
				{
					target[i][j] = 255;
				}
				else if(size <= MIN_SIZE && close[pix] == 1)
				{
					target[i][j] = 255;
				}
				else
				{
					target[i][j] = 0;
				}

			}
		}

	//create temp image to display results
	IplImage *temp = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
	uchar* data = (uchar *)temp->imageData;
	//copy new data into image
	for(int i = 0; i < height;i++)
	{
		for(int j = 0; j < width; j++)
		{
			data[(i*temp->widthStep) + j ] = target[i][j];
			//printf("%i %i %u \n",i,j,test[i][j]);
		}
	}
	temp->imageData = (char*)data;

	// a visualization window is created with title 'image'
	cvNamedWindow ("image2", 1);
	// img is shown in 'image' window
	cvShowImage ("image2", temp);


	// wait for infinite delay for a keypress
	cvWaitKey (0);
	// memory release for img before exiting the application
	cvReleaseImage (&temp);
	cvDestroyWindow("image2");



	printf("Done \n");




}

void sizeFilter(CCL_Object source)
{
	printf("Filtering CCs by size...\n");
	int width =  source.width;
	int height = source.height;
	int target[height][width];

	for(int i = 0;i < height;i++)
	{
		for(int j = 0;j < width;j++)
		{
			int pix = source.labels[(i * width) + j];
			//printf("%i \n",source.classSizes[pix]);
			int size = source.classSizes[pix];
			if(size > MIN_SIZE && size < MAX_SIZE)
			{
				target[i][j] = 255;
			}
			else
			{
				target[i][j] = 0;
			}
		}
	}

	//create temp image to display results
	IplImage *temp = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
	uchar* data = (uchar *)temp->imageData;
	//copy new data into image
	for(int i = 0; i < height;i++)
	{
		for(int j = 0; j < width; j++)
		{
			data[(i*temp->widthStep) + j ] = target[i][j];
			//printf("%i %i %u \n",i,j,test[i][j]);
		}
	}
	temp->imageData = (char*)data;

	// a visualization window is created with title 'image'
	cvNamedWindow ("image2", 1);
	// img is shown in 'image' window
	cvShowImage ("image2", temp);


	// wait for infinite delay for a keypress
	cvWaitKey (0);
	// memory release for img before exiting the application
	cvReleaseImage (&temp);
	cvDestroyWindow("image2");



	printf("Done \n");
}
