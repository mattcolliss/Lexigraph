/////////////////////////////////////////////////////////////////////////////
//
// Matthew Colliss
//
//
/////////////////////////////////////////////////////////////////////////////


#include "perceptualGrouping.h"

#define PILULIMIT 0.015


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


void perceptualGrouping(CCL_Object source,IplImage *img)
{
	printf("Performing perceptual grouping...\n");

	IplImage *displayImage = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,3);
	cvCvtColor(img,displayImage,CV_GRAY2BGR);

	//count the number of non-zero classes
	int c = 0;
	for(int i = 0;i < source.classCount;i++)
	{
		if(source.classSizes[i] > 0) c++;
	}
	//build the blob data structure
	Blob blobs[c];
	int p = 0;
	for(int i = 0;i < source.classCount;i++)
	{
		if(source.classSizes[i] > 0)
		{
			blobs[p].size = source.classSizes[i];
			blobs[p].top.i = source.minI[i];
			blobs[p].top.j = source.minJ[i];
			blobs[p].bottom.i = source.maxI[i];
			blobs[p].bottom.j = source.maxJ[i];
			blobs[p].centre = findCentre(blobs[p].top,blobs[p].bottom);
			blobs[p].linked = -1;
			blobs[p].group = -1;
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
			if(i != j && blobs[j].linked != i)
			{
				float pilu = piluOperator(blobs[i].top,blobs[i].bottom,blobs[j].top,blobs[j].bottom);
				if(pilu > maxPilu)
				{
					maxPilu = pilu;
					pointer = j;
				}
			}
		}

		if(maxPilu >= PILULIMIT)
		{
			blobs[i].next = blobs[pointer].centre;
			blobs[i].linked = pointer;


			//update groups
			if(blobs[i].group == -1) //i.e new group
			{
				if(blobs[pointer].group != -1) //ie next is already in a group
				{
					//update group equivalences
					for(int k = 0;k < c;k++)
					{
						if(blobs[k].group == blobs[pointer].group) blobs[k].group = groupCount;
					}
				}
				blobs[pointer].group = groupCount;
				blobs[i].group = groupCount;
				groupCount++;
			}
			else
			{
				if(blobs[pointer].group == -1) blobs[pointer].group = blobs[i].group;
				else
				{
					//update group equivalences
					for(int k = 0;k < c;k++)
					{
						if(blobs[k].group == blobs[pointer].group) blobs[k].group = blobs[i].group;
					}
				}
			}


			//draw line, image, pt1, pt2, colour, thickness, type, offset
			cvLine(displayImage,cvPoint(blobs[i].centre.j,blobs[i].centre.i),cvPoint(blobs[i].next.j,blobs[i].next.i),cvScalar(155,0,0,0),2,8,0);

		}
	}

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

		//for each bob
		for(int j = 0;j < c;j++)
		{
			//if its in this group
			if(blobs[j].group == i)
			{
				//add it to the bbox
				boxes[i].size++;
				boxes[i] = extendBox(boxes[i],blobs[j].top,blobs[j].bottom);
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





	/*
	//array of BBoxes
	BBox boxes[c];
	int b = 0;

	int finished = 0;
	while(finished == 0)
	{
		//find the next non-grouped blob
		Blob current;
		int cont = 0;
		for(int i = 0;i < c;i++)
		{
			if(blobs[i].grouped == -1)
			{
				cont = 1;
				blobs[i].grouped = b;
				current = blobs[i];
				break;
			}
		}

		//if we found a blob, and it has a link
		if(cont == 1 && current.linked != -1)
		{
			//BBox box;

			boxes[b].top = current.top;
			boxes[b].bottom = current.bottom;
			boxes[b].invalid = 0;
			int linked = 1;
			while(linked > 0)
			{
				if(current.linked != -1)
				{
					int pointer = current.linked;
					if(blobs[pointer].grouped == -1)
					{
						blobs[pointer].grouped = b;
						current = blobs[pointer];
						boxes[b] = extendBox(boxes[b],current.top,current.bottom);
					}
					else
					{
						int oldBox = blobs[pointer].grouped;
						boxes[b] = combineBoxes(boxes[b],boxes[oldBox]);
						//blobs[pointer].grouped = b;
						for(int i = 0;i < c;i++)
						{
							if(blobs[i].grouped == oldBox)
							{
								blobs[i].grouped = b;
							}
						}
						current = blobs[pointer];
						boxes[oldBox].invalid = 1;
					}
				}
				else
				{
					linked = 0;
				}
			}
			b++;

		}

		if(cont == 0) finished = 1;
	}

	//find the biggest box for demo purposes
	BBox biggest;
	int maxSize = -1;

	for(int i = 0;i < b;i++)
	{

		if(boxes[i].invalid == 0)
		{
			drawBox(boxes[i],displayImage);
			if(boxWidth(boxes[i]) > maxSize)
			{
				maxSize = boxWidth(boxes[i]);
				biggest = boxes[i];
			}
		}

	}

	drawBox(biggest,displayImage);
	 */

	//show the image
	cvNamedWindow("ImageWindow", CV_WINDOW_AUTOSIZE);
	cvShowImage("ImageWindow", displayImage);
	printf("Image displayed...\n");

	//wait till key is pressed
	printf("PRESS A KEY NOW...\n");
	cvWaitKey(0);




	printf("Done\n");
}
