//----------------------------------------------------------------------------
// Copyright 2020,  all rights reserved.
//----------------------------------------------------------------------------

#include "MathEngine.h"
#include "BoundingSphere.h"
#include <math.h>

Sphere::Sphere()
	: cntr(Vect(0.0f, 0.0f, 0.0f, 1.0f)),
	rad(0.0f),
	pad{ 0 }
{

}

void Sphere::privMostSeparatedPointsOnAABB(int &min, int &max, Vect *pt, int numPts)
{
    // First find most extreme points along principal axes
    int minx = 0;
	int maxx = 0;
	int miny = 0;
	int maxy = 0;
	int minz = 0;
	int maxz = 0;
	 
    for (int i = 1; i < numPts; i++) 
	{
        if (pt[i][x] < pt[minx][x]) 
			minx = i;
        if (pt[i][x] > pt[maxx][x]) 
			maxx = i;
        if (pt[i][y] < pt[miny][y]) 
			miny = i;
        if (pt[i][y] > pt[maxy][y]) 
			maxy = i;
        if (pt[i][z] < pt[minz][z]) 
			minz = i;
        if (pt[i][z] > pt[maxz][z]) 
			maxz = i;
    }

	// Given 6pts minx,miny,minz,maxx,maxy,maxz find the greatest distance
	struct tmpNode
	{
		Vect v;
		int index;
		int pad[3];  // padding
	};

	tmpNode data[6];

	data[0].v.set(pt[minx]);
	data[1].v.set(pt[miny]);
	data[2].v.set(pt[minz]);
	data[3].v.set(pt[maxx]);
	data[4].v.set(pt[maxy]);
	data[5].v.set(pt[maxz]);

	data[0].index = minx;
	data[1].index =miny;
	data[2].index =minz;
	data[3].index =maxx;
	data[4].index =maxy;
	data[5].index =maxz;


	float maxDist = (data[0].v - data[1].v).mag();
	int maxA = data[0].index;
	int maxB = data[1].index;
	float dist = maxDist;

	for( int i=0;i<6;i++)
	{
		for(int j=0;j<6;j++)
		{
			dist = (data[i].v - data[j].v).mag();
			//out("dist(%d,%d) = %f\n",i,j,dist);
			if( dist > maxDist )
			{
				//out("--->maxdist(%d,%d) = %f\n",i,j,dist);
				// new max
				maxDist = dist;
				maxA = data[i].index;
				maxB = data[j].index;
			}
		}
	}

	min = maxA;
	max = maxB;
}

void Sphere::privSphereOfSphereAndPt(Vect &p)
{
    // Compute squared distance between point and sphere center
    Vect d = p - this->cntr;

    float dist2 = d.dot( d);
    // Only update s if point p is outside it
    if (dist2 > this->rad * this->rad) 
	{
        float dist = sqrtf(dist2);
        float newRadius = (this->rad + dist) * 0.5f;
        float k = (newRadius - this->rad) / dist;
        this->rad = newRadius;
        this->cntr += d * k;
    }
}

void Sphere::privSphereFromDistantPoints(Vect *pt, int numPts)
{
	assert(pt);

    // Find the most separated point pair defining the encompassing AABB
    int min, max;
    this->privMostSeparatedPointsOnAABB(min, max, pt, numPts);

    // Set up sphere to just encompass these two points
    this->cntr = (pt[min] + pt[max]) * 0.5f;
    this->rad = (pt[max] - this->cntr).dot(pt[max] - this->cntr);
	this->rad = sqrtf(this->rad);
}

void Sphere::RitterSphere(Vect *pt, int numPts)
{
	assert(pt);

    // Get sphere encompassing two approximately most distant points
    this->privSphereFromDistantPoints(pt, numPts);

    // Grow sphere to include all points
    for (int i = 0; i < numPts; i++)
	{
        this->privSphereOfSphereAndPt(pt[i]);
	}
}

//--- End of File -------------------------------------------------------------
