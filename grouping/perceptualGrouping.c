/////////////////////////////////////////////////////////////////////////////
//
// Matthew Colliss
//
//
/////////////////////////////////////////////////////////////////////////////


#include "perceptualGrouping.h"

#define PILULIMIT 0.01


//return the width of a BBox
int boxWidth(BBox a)
{
	int width = a.bottom.j - a.top.j;
	return width;
}

//bounding box defined by a,b
Point findCentre(Point a, Point b)
{
	Point centre;
	centre.i = a.i + (b.i - a.i)/2;
	centre.j = a.j + (b.j - a.j)/2;
	return centre;
}

//draw a BBox on the image
void drawBox(BBox box, IplImage *img)
{
	Point topLeft,topRight, bottomLeft,bottomRight;
	topLeft = box.top;
	bottomRight = box.bottom;
	topRight.i = topLeft.i;
	topRight.j = bottomRight.j;
	bottomLeft.i = bottomRight.i;
	bottomLeft.j = topLeft.j;

	//draw line from top left to top right
	cvLine(img,cvPoint(topLeft.j,topLeft.i),cvPoint(topRight.j,topRight.i),cvScalar(0,0,155,0),1,8,0);
	//draw line from top right to bottom right
	cvLine(img,cvPoint(topRight.j,topRight.i),cvPoint(bottomRight.j,bottomRight.i),cvScalar(0,0,155,0),1,8,0);
	//draw line from bottom right to bottom left
	cvLine(img,cvPoint(bottomRight.j,bottomRight.i),cvPoint(bottomLeft.j,bottomLeft.i),cvScalar(0,0,155,0),1,8,0);
	//draw line from bottom left to top left
	cvLine(img,cvPoint(bottomLeft.j,bottomLeft.i),cvPoint(topLeft.j,topLeft.i),cvScalar(0,0,155,0),1,8,0);

}

//combine 2 BBoxes
BBox combineBoxes(BBox a, BBox b)
{
	BBox result;
	result.top.i = MIN(a.top.i,b.top.i);
	result.top.j = MIN(a.top.j,b.top.j);
	result.bottom.i = MAX(a.bottom.i,b.bottom.i);
	result.bottom.j = MAX(a.bottom.j,b.bottom.j);

	return result;
}


//extend a BBox to contain a new BBox
BBox extendBox(BBox source, Point top,Point bottom)
{
	BBox result;
	result.top.i = MIN(source.top.i,top.i);
	result.top.j = MIN(source.top.j,top.j);
	result.bottom.i = MAX(source.bottom.i,bottom.i);
	result.bottom.j = MAX(source.bottom.j,bottom.j);
	result.size = source.size;
	return result;
}


void perceptualGrouping(CCL_Object positiveSource,CCL_Object negativeSource,IplImage *img)
{
	printf("Performing perceptual grouping...\n");

	IplImage *displayImage = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,3);
	cvCvtColor(img,displayImage,CV_GRAY2BGR);

	int positiveMappings[positiveSource.classCount];
	for(int i = 0;i < positiveSource.classCount;i++){ positiveMappings[i] = -1;}

	int negativeMappings[negativeSource.classCount];
	for(int i = 0;i < negativeSource.classCount;i++){ negativeMappings[i] = -1;}


	//POSITIVE PASS
	//count the number of non-zero classes
	int c = 0;
	for(int i = 0;i < positiveSource.classCount;i++)
	{
		if(positiveSource.classSizes[i] > 0) c++;
	}
	//build the blob data structure
	Blob positiveBlobs[c];
	int p = 0;
	for(int i = 0;i < positiveSource.classCount;i++)
	{
		if(positiveSource.classSizes[i] > 0)
		{
			positiveBlobs[p].size = positiveSource.classSizes[i];
			positiveBlobs[p].top.i = positiveSource.minI[i];
			positiveBlobs[p].top.j = positiveSource.minJ[i];
			positiveBlobs[p].bottom.i = positiveSource.maxI[i];
			positiveBlobs[p].bottom.j = positiveSource.maxJ[i];
			positiveBlobs[p].centre = findCentre(positiveBlobs[p].top,positiveBlobs[p].bottom);
			positiveBlobs[p].linked = -1;
			positiveBlobs[p].group = -1;
			positiveMappings[i] = p;
			p++;
		}
	}

	int groupCount = 0;
	for(int i = 0;i < c;i++)
	{
		float maxPilu = 0;
		int pointer = 0;

		for(int j = 0;j < c;j++)
		{
			if(i != j && positiveBlobs[j].linked != i)
			{
				float pilu = piluOperator(positiveBlobs[i].top,positiveBlobs[i].bottom,positiveBlobs[j].top,positiveBlobs[j].bottom);
				if(pilu > maxPilu)
				{
					maxPilu = pilu;
					pointer = j;
				}
			}
		}

		if(maxPilu >= PILULIMIT)
		{
			positiveBlobs[i].next = positiveBlobs[pointer].centre;
			positiveBlobs[i].linked = pointer;


			//update groups
			if(positiveBlobs[i].group == -1) //i.e new group
			{
				if(positiveBlobs[pointer].group != -1) //ie next is already in a group
				{
					//update group equivalences
					for(int k = 0;k < c;k++)
					{
						if(positiveBlobs[k].group == positiveBlobs[pointer].group) positiveBlobs[k].group = groupCount;
					}
				}
				positiveBlobs[pointer].group = groupCount;
				positiveBlobs[i].group = groupCount;
				groupCount++;
			}
			else
			{
				if(positiveBlobs[pointer].group == -1) positiveBlobs[pointer].group = positiveBlobs[i].group;
				else
				{
					//update group equivalences
					for(int k = 0;k < c;k++)
					{
						if(positiveBlobs[k].group == positiveBlobs[pointer].group) positiveBlobs[k].group = positiveBlobs[i].group;
					}
				}
			}


			//draw line, image, pt1, pt2, colour, thickness, type, offset
			cvLine(displayImage,cvPoint(positiveBlobs[i].centre.j,positiveBlobs[i].centre.i),cvPoint(positiveBlobs[i].next.j,positiveBlobs[i].next.i),cvScalar(155,0,0,0),2,8,0);

		}
	}
	/////////////////////////////////////////////////////////////////////////
	//NEGATIVE PASS
	//count the number of non-zero classes
	int n = 0;
	for(int i = 0;i < negativeSource.classCount;i++)
	{
		if(negativeSource.classSizes[i] > 0) n++;
	}
	//build the blob data structure
	Blob negativeBlobs[n];
	p = 0;
	for(int i = 0;i < negativeSource.classCount;i++)
	{
		if(negativeSource.classSizes[i] > 0)
		{
			negativeBlobs[p].size = negativeSource.classSizes[i];
			negativeBlobs[p].top.i = negativeSource.minI[i];
			negativeBlobs[p].top.j = negativeSource.minJ[i];
			negativeBlobs[p].bottom.i = negativeSource.maxI[i];
			negativeBlobs[p].bottom.j = negativeSource.maxJ[i];
			negativeBlobs[p].centre = findCentre(negativeBlobs[p].top,negativeBlobs[p].bottom);
			negativeBlobs[p].linked = -1;
			negativeBlobs[p].group = -1;
			negativeMappings[i] = p;
			p++;
		}
	}

	for(int i = 0;i < n;i++)
	{
		float maxPilu = 0;
		int pointer = 0;

		for(int j = 0;j < n;j++)
		{
			if(i != j && negativeBlobs[j].linked != i)
			{
				float pilu = piluOperator(negativeBlobs[i].top,negativeBlobs[i].bottom,negativeBlobs[j].top,negativeBlobs[j].bottom);
				if(pilu > maxPilu)
				{
					maxPilu = pilu;
					pointer = j;
				}
			}
		}

		if(maxPilu >= PILULIMIT)
		{
			negativeBlobs[i].next = negativeBlobs[pointer].centre;
			negativeBlobs[i].linked = pointer;


			//update groups
			if(negativeBlobs[i].group == -1) //i.e new group
			{
				if(negativeBlobs[pointer].group != -1) //ie next is already in a group
				{
					//update group equivalences
					for(int k = 0;k < n;k++)
					{
						if(negativeBlobs[k].group == negativeBlobs[pointer].group) negativeBlobs[k].group = groupCount;
					}
				}
				negativeBlobs[pointer].group = groupCount;
				negativeBlobs[i].group = groupCount;
				groupCount++;
			}
			else
			{
				if(negativeBlobs[pointer].group == -1) negativeBlobs[pointer].group = negativeBlobs[i].group;
				else
				{
					//update group equivalences
					for(int k = 0;k < n;k++)
					{
						if(negativeBlobs[k].group == negativeBlobs[pointer].group) negativeBlobs[k].group = negativeBlobs[i].group;
					}
				}
			}


			//draw line, image, pt1, pt2, colour, thickness, type, offset
			cvLine(displayImage,cvPoint(negativeBlobs[i].centre.j,negativeBlobs[i].centre.i),cvPoint(negativeBlobs[i].next.j,negativeBlobs[i].next.i),cvScalar(155,0,0,0),2,8,0);

		}
	}

	/////////////////////////////////////////////////////////////////

	//array of BBoxes
	BBox boxes[groupCount];
	//build BBoxes
	for(int i = 0;i < groupCount;i++)
	{
		//initialise box - inside out
		boxes[i].top.i = 9999;
		boxes[i].top.j = 9999;
		boxes[i].bottom.i = -1;
		boxes[i].bottom.j = -1;
		boxes[i].size = 0;

		//for each positive blob
		for(int j = 0;j < c;j++)
		{
			//if its in this group
			if(positiveBlobs[j].group == i)
			{
				//add it to the bbox
				boxes[i].size++;
				boxes[i] = extendBox(boxes[i],positiveBlobs[j].top,positiveBlobs[j].bottom);
			}
		}
		//for each negative blob
		for(int j = 0;j < n;j++)
		{
			//if its in this group
			if(negativeBlobs[j].group == i)
			{
				//add it to the bbox
				boxes[i].size++;
				boxes[i] = extendBox(boxes[i],negativeBlobs[j].top,negativeBlobs[j].bottom);
			}
		}
	}

	//for each box
	for(int i = 0;i < groupCount;i++)
	{
		//if its got more than one thing in it
		//printf("%i \n",boxes[i].size);
		if(boxes[i].size > 1)
		{
			//draw it
			drawBox(boxes[i],displayImage);
		}
	}


	//show the image
	cvSaveImage("grouping.png",displayImage);
	cvNamedWindow("ImageWindow", CV_WINDOW_AUTOSIZE);
	cvShowImage("ImageWindow", displayImage);
	printf("Image displayed...\n");

	//wait till key is pressed
	printf("PRESS A KEY NOW...\n");
	cvWaitKey(0);

	//update img
	int target[positiveSource.height][positiveSource.width];

	for(int i = 0;i < positiveSource.height;i++)
	{
		for(int j = 0;j < positiveSource.width;j++)
		{
			int pos = positiveSource.labels[(i * positiveSource.width) + j];
			pos = positiveMappings[pos];
			if(pos != -1)
			{
				pos = positiveBlobs[pos].group;
				pos = boxes[pos].size;
			}
			int neg = negativeSource.labels[(i * negativeSource.width) + j];
			//int neg = -1;
			neg = negativeMappings[neg];
			if(pos != -1)
			{
				neg = negativeBlobs[neg].group;
				neg = boxes[neg].size;
			}
			if(pos > 1 || neg > 1)
			{
				target[i][j] =  255;
			}else
			{
				target[i][j] = 0;
			}
		}
	}
	uchar* data = (uchar *)img->imageData;
	//copy new data into image
	for(int i = 0; i < positiveSource.height;i++)
	{
		for(int j = 0; j < positiveSource.width; j++)
		{
			data[(i*img->widthStep) + j ] = target[i][j];
			//printf("%i %i %u \n",i,j,test[i][j]);
		}
	}
	img->imageData = (char*)data;





	printf("Done\n");
}
