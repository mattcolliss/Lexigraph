//TODO: header comment
#include "sizeFilter.h"

#define MIN_SIZE 50
#define MAX_SIZE 1000


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

CCL_Object sizeFilter(CCL_Object source)
{
	printf("Filtering CCs by size...\n");
	int width =  source.width;
	int height = source.height;
	int imageSize = width * height;
	int close[source.classCount];
	buildCloseList(close,source);

	for(int i = 0;i < source.classCount;i++)
	{
		//printf("%i \n",source.classSizes[i]);
		int size = source.classSizes[i];
		if((size <= MIN_SIZE && close[i] == 0) || size >= MAX_SIZE)
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




	printf("Done \n");
	return(source);
}

