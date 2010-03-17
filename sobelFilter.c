
#include "sobelFilter.h"

CCL_Object sobelFilter(CCL_Object source,IplImage *img)
{
	printf("Filtering CCs by Sobel operator...\n");
	int height = source.height;
	int width = source.width;
	int imageSize = height * width;
	int gX[imageSize];
	int hX[9] = {1,0,-1,2,0,-2,1,0,-1};
	convolve(gX,hX,3,height,width,img);

	//convol
	//int gY[height][width];



	printf("Done\n");
	return source;
}
