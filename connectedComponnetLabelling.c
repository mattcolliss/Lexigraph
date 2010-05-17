/////////////////////////////////////////////////////////////////////////////
//
// Matthew Colliss
//
//
/////////////////////////////////////////////////////////////////////////////

#include "connectedComponentLabelling.h"

#define maxLabel 10000



CCL_Object connectedComponentLabeling(IplImage *img)
{
	printf("Performing connected component labelling...\n");

	//Initialise local variables
	uchar* data = (uchar *)img->imageData;
	int step = img->widthStep;
	int height = img->height;
	int width = img->width;
	int imageSize = width * height;
	int source[height][width];

	//build binary matrix representation of image
	for(int i = 0; i < height;i++)
	{
		for(int j = 0; j < width; j++)
		{
			if(data[(i*step) + j ] == 0)
			{
				source[i][j] = 0;
			}
			else
			{
				source[i][j] = 1;
			}
			//printf("%i ",source[i][j]);
		}
		//printf(" \n");
	}

	int nextLabel = 0;
	//initialise classes
	int classes[maxLabel];
	for(int i = 0;i < maxLabel;i++)
	{
		classes[i] = i;
	}

	//first pass
	for(int i = 1; i < height;i++)
	{
		for(int j = 1; j < width - 1; j++)
		{
			//if this is a foreground pixel
			if(source[i][j] != 0)
			{
				int top, left, topLeft, topRight;
				//get the 4 already visited neighbour pixels
				top = source[i - 1][j];
				topLeft = source[i - 1][j - 1];
				left = source[i][j - 1];
				topRight = source[i - 1][j + 1];

				//count how many are non-zero
				int count = 0;
				if(top != 0) count++;
				if(left != 0) count++;
				if(topLeft != 0) count++;
				if(topRight != 0) count++;

				//if all are background pixels, e count == 0
				if(count == 0)
				{
					if(nextLabel < maxLabel - 1) nextLabel++;
					source[i][j] = nextLabel;

				}
				//if more that one is foreground
				else if(count > 1)
				{
					int min = left;
					if((top < min && top != 0) || min == 0) min = top;
					if((topLeft < min && topLeft != 0) || min == 0) min = topLeft;
					if((topRight < min && topRight != 0) || min == 0) min = topRight;

					if(classes[left] != classes[min])
					{
						for(int k = 0;k <= nextLabel;k++)
						{
							if (classes[k] == classes[left])
							{
								classes[k] = classes[min];
							}
						}
					}

					if(classes[top] != classes[min])
					{
						for(int k = 0;k <= nextLabel;k++)
						{
							if (classes[k] == classes[top])
							{
								classes[k] = classes[min];
							}
						}
					}

					if(classes[topLeft] != classes[min])
					{
						for(int k = 0;k <= nextLabel;k++)
						{
							if (classes[k] == classes[topLeft])
							{
								classes[k] = classes[min];
							}
						}
					}

					if(classes[topRight] != classes[min])
					{
						for(int k = 0;k <= nextLabel;k++)
						{
							if (classes[k] == classes[topRight])
							{
								classes[k] = classes[min];
							}
						}
					}
					source[i][j] = min;
				}
				//if just one is foreground
				else if (left != 0)
				{
					source[i][j] = left;
				}
				else if (top != 0)
				{
					source[i][j] = top;
				}

				else if (topLeft != 0)
				{
					source[i][j] = topLeft;
				}

				else if (topRight != 0)
				{
					source[i][j] = topRight;
				}
			} //if
		} //for	j
	} //for i

	int maxClass = 0;

	//second pass
	//create 1d array of mem to store final results
	int *labels = malloc(imageSize * sizeof(int));

	for(int i = 1; i < height;i++)
	{
		for(int j = 1; j < width; j++)
		{

			if(source[i][j] != 0)
			{
				source[i][j] = classes[source[i][j]];
				if(classes[source[i][j]] >= maxClass)
				{
					maxClass = classes[source[i][j]];
				}
			}
			//printf("%i ",source[i][j]);
			labels[(i * width) + j] = source[i][j];

		}
		//printf("\n");
	}

	maxClass++;
	//printf("%i \n",maxClass);
	int *classSizes = malloc(maxClass * sizeof (int));
	int *minI = malloc(maxClass * sizeof(int));
	int *minJ = malloc(maxClass * sizeof(int));
	int *maxI = malloc(maxClass * sizeof(int));
	int *maxJ = malloc(maxClass * sizeof(int));
	for(int i = 0;i < maxClass;i++)
	{
		classSizes[i] = 0;
		minI[i] = 9999;
		minJ[i] = 9999;
		maxI[i] = -1;
		maxJ[i] = -1;
	}

	for(int i = 0; i < img->height;i++)
	{
		for(int j = 0; j < img->width; j++)
		{
			int val = source[i][j];
			if(val != 0)
			{
				classSizes[val]++;
				if(i < minI[val]) minI[val] = i;
				if(j < minJ[val]) minJ[val] = j;
				if(i > maxI[val]) maxI[val] = i;
				if(j > maxJ[val]) maxJ[val] = j;
			}
		}
	}


	/*for(int i = 0;i < maxClass;i++)
	{
		printf("%i ",classSizes[i]);
		printf("%i ",minI[i]);
		printf("%i ",minJ[i]);
		printf("%i ",maxI[i]);
		printf("%i\n",maxJ[i]);
	}*/



	//TODO: remove this probably
	/*
	for(int i = 0; i < img->height;i++)
	{
		for(int j = 0; j < img->width; j++)
		{

			data[(i*step) + j ] = source[i][j] % 255;

		}
	}
	img->imageData = (char*)data;
	 */

	//package up data into target
	CCL_Object target;
	target.height = height;
	target.width = width;
	target.labels = labels;
	target.classCount = maxClass;
	target.classSizes = classSizes;
	target.minI = minI;
	target.minJ = minJ;
	target.maxI = maxI;
	target.maxJ = maxJ;


	printf("Done\n");

	return target;
}

