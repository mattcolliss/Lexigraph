//TODO: header comment
#include "sizeFilter.h"


#define MAX_WIDTH_RATIO 0.1
#define MIN_WIDTH_RATIO 0.01
#define MAX_HEIGHT_RATIO 0.1
#define MIN_HEIGHT_RATIO 0.01
#define MAX_OCCUPY_RATIO 0.95
#define MIN_OCCUPY_RATIO 0.2
#define MAX_ASPECT_RATIO 7



CCL_Object sizeFilter(CCL_Object source)
{
	printf("Filtering CCs by size...\n");
	int imageWidth =  source.width;
	int imageHeight = source.height;
	int imageSize = imageWidth * imageHeight;

	//width ratio
	for(int i = 0;i < source.classCount;i++)
	{
		if(source.classSizes[i] > 0)
		{
			float width = (source.maxJ[i] - source.minJ[i]) + 1;
			float widthRatio = width / imageWidth;
			//printf("%2.2f %i %2.10f \n",width,imageWidth,widthRatio);
			if(widthRatio > MAX_WIDTH_RATIO || widthRatio <  MIN_WIDTH_RATIO ) source.classSizes[i] = 0;
		}
	}

	//height ratio
	for(int i = 0;i < source.classCount;i++)
	{
		if(source.classSizes[i] > 0)
		{
			float height = (source.maxI[i] - source.minI[i]) + 1;
			float heightRatio = height / imageHeight;
			//printf("%2.2f %i %2.10f \n",height,imageHeight,heightRatio);
			if(heightRatio > MAX_HEIGHT_RATIO || heightRatio <  MIN_HEIGHT_RATIO ) source.classSizes[i] = 0;
		}
	}

	//occupy ratio
	for(int i = 0;i < source.classCount;i++)
	{
		if(source.classSizes[i] != 0)
		{
			int bbWidth = (source.maxJ[i] - source.minJ[i]) + 1;
			int bbHeight = (source.maxI[i] - source.minI[i]) + 1;
			float bbArea = bbWidth * bbHeight;
			float occupyRatio = source.classSizes[i] / bbArea;
			if(occupyRatio > MAX_OCCUPY_RATIO || occupyRatio < MIN_OCCUPY_RATIO) source.classSizes[i] = 0;
		}
	}

	//apsect ratio
	for(int i = 0;i < source.classCount;i++)
	{
		if(source.classSizes[i] != 0)
		{
			int bbWidth = (source.maxJ[i] - source.minJ[i]) + 1;
			int bbHeight = (source.maxI[i] - source.minI[i]) + 1;
			float wh = bbWidth / bbHeight;
			float hw = bbHeight / bbWidth;
			float aspectRatio = MAX(wh,hw);
			//printf("%2.4f \n",aspectRatio);
			if(aspectRatio >=  MAX_ASPECT_RATIO) source.classSizes[i] = 0;
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

