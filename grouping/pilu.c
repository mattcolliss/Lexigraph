/////////////////////////////////////////////////////////////////////////////
//
// Matthew Colliss
//
// Find pilu saliency measure for two CCs
//
/////////////////////////////////////////////////////////////////////////////


#include "pilu.h"

#define PI 3.141592

//finds euclidean distance between 2 points
float distance(Point p,Point q)
{
	float i = p.i - q.i;
	float j = p.j - q.j;

	i = i * i;
	j = j * j;

	float ed = sqrt(i + j);

	return ed;
}

//calculates minimum distance between BBoxes of  components
float calcDmin(Point aTop,Point aBottom,Point bTop,Point bBottom)
{
	Point aTR, aBL, bTR, bBL,aCentre,bCentre;
	//initialise all corners
	aTR.i = aTop.i;
	aTR.j = aBottom.j;
	aBL.i = aBottom.i;
	aBL.j = aTop.j;
	bTR.i = bTop.i;
	bTR.j = bBottom.j;
	bBL.i = bBottom.i;
	bBL.j = bTop.j;
	//find centre points
	aCentre.i = aTop.i + ((aBottom.i - aTop.i)/2);
	aCentre.j = aTop.j + ((aBottom.j - aTop.j)/2);
	bCentre.i = bTop.i + ((bBottom.i - bTop.i)/2);
	bCentre.j = bTop.j + ((bBottom.j - bTop.j)/2);
	//find which of A's corners is closest to centre of B
	Point minA = aTop;
	float minDist = distance(aTop,bCentre);
	if(distance(aTR,bCentre) < minDist)
	{
		minDist = distance(aTR,bCentre);
		minA = aTR;
	}
	if(distance(aBL,bCentre) < minDist)
	{
		minDist = distance(aBL,bCentre);
		minA = aBL;
	}
	if(distance(aBottom,bCentre) < minDist)
	{
		minDist = distance(aBottom,bCentre);
		minA = aBottom;
	}
	//find which of B's corners is closest to centre of A
	Point minB = bTop;
	minDist = distance(bTop,aCentre);
	if(distance(bTR,aCentre) < minDist)
	{
		minDist = distance(bTR,aCentre);
		minB = bTR;
	}
	if(distance(bBL,aCentre) < minDist)
	{
		minDist = distance(bBL,aCentre);
		minB = bBL;
	}
	if(distance(bBottom,aCentre) < minDist)
	{
		minDist = distance(bBottom,aCentre);
		minB = bBottom;
	}

	//Dmin is distance between nearest 2 corners
	float Dmin = distance(minA,minB);

	//printf("%2.4f \n",Dmin);

	return Dmin;
}

//calculate the minimum axis for a BBox
float calcMinAxis(Point top, Point bottom)
{
	Point topCentre, centreRight, centre;
	//find centre point
	centre.i = top.i + ((bottom.i - top.i)/2);
	centre.j = top.j + ((bottom.j - top.j)/2);
	//find other points
	topCentre.i = top.i;
	topCentre.j = centre.j;
	centreRight.i = centre.i;
	centreRight.j = bottom.j;
	//find minimum axis
	float vert = distance(topCentre,centre);
	float horiz = distance(centreRight,centre);
	//return the smallest
	if(vert < horiz) return vert;
	return horiz;
}

//calculate the maximum axis for a BBox
float calcMaxAxis(Point top, Point bottom)
{
	Point topCentre, centreRight, centre;
	//find centre point
	centre.i = top.i + ((bottom.i - top.i)/2);
	centre.j = top.j + ((bottom.j - top.j)/2);
	//find other points
	topCentre.i = top.i;
	topCentre.j = centre.j;
	centreRight.i = centre.i;
	centreRight.j = bottom.j;
	//find maximum axis
	float vert = distance(topCentre,centre);
	float horiz = distance(centreRight,centre);
	//return the largest
	if(vert >= horiz) return vert;
	return horiz;
}

//calculates gaussian distribution of x with mean a and stand deviation b > 0
//
//  = 1/b*root(2pi) * exp(-(x - a)^2 / 2b^2)
//
float gaussian(float x, float a, float b)
{
	//calc first term: 1/b*root(2pi)
	float firstTerm = 2 * PI;
	firstTerm =  sqrt(firstTerm);
	firstTerm = firstTerm * b;
	if(firstTerm != 0) firstTerm = 1 / firstTerm;
	//calc second term: exp((-(x - a)^2 / 2b^2)
	float nume = 0  - ((x - a) * (x - a));
	float demom = (b * b) * 2;
	float exponent = nume / demom;
	float secondTerm  = exp(exponent);
	//mulitply terms
	float gaussian = firstTerm * secondTerm;
	//return answer
	return gaussian;
}

float piluOperator(Point aTop,Point aBottom,Point bTop,Point bBottom)
{
	float Dmin = calcDmin( aTop, aBottom, bTop, bBottom);

	float aMin = calcMinAxis(aTop,aBottom);
	float aMax  = calcMaxAxis(aTop,aBottom);

	float bMin = calcMinAxis(bTop,bBottom);
	float bMax  = calcMaxAxis(bTop,bBottom);

	float lamda = 0;

	if((aMin + bMin) != 0)
	{
		lamda = (2 * Dmin) / (aMin + bMin);
	}

	float gamma = 0;

	if((bMin + bMax) != 0)
	{
		gamma = (aMin + aMax) / (bMin + bMax);
	}

	float piluTerm1 = gaussian(lamda,1,2);
	float piluTerm2 = gaussian(gamma,0,4);

	float pilu = piluTerm1 * piluTerm2;

	//printf("%2.112f \n",pilu);

	return pilu;


}
