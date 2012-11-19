#include <iostream>
#include <quadTreePointGen.h>
#include "usesTypes.h"
#include <node.h>
#include <graphic.h>


#include <rect.h>

using namespace std;

int main(int argc, char *argv[])
{
	int pointCnt = 4;
	Point point[] = {Point(1, 0), Point(8, 3), Point(5, 8), Point(0, 4)};
	QuadroMesh mesh(point, pointCnt, 0.9);
	mesh.Start();
	Segment * segToPlot;
	int  segCnt;
	mesh.GetSegments(&segToPlot, &segCnt);
	Plot(segToPlot, segCnt);


/*
	Point * points = new Point[6];
	points[3].x = 2; points[3].y = 6;
	points[2].x = 4; points[2].y = 6;
	points[1].x = 4; points[1].y = 8;
	points[0].x = 2; points[0].y = 8;

	points[4].x = 0; points[4].y = 4;
	points[5].x = 9.1; points[5].y = 12.1;

	Rect * rect = new Rect();
	for (int i = 0; i < 4; ++i)
		rect->vertex[i] = points + i;

	Segment * seg = new Segment();
	seg->vertex[0] = points + 4;
	seg->vertex[1] = points + 5;

	cout<<"result "<<IsSegmetCrosRect(rect, seg)<<endl;

	Segment * segs = new Segment[5];
	segs[0].vertex[0] = points; segs[0].vertex[1] = points + 1;
	segs[1].vertex[0] = points + 1; segs[1].vertex[1] = points + 2;
	segs[2].vertex[0] = points + 2; segs[2].vertex[1] = points + 3;
	segs[3].vertex[0] = points + 3; segs[3].vertex[1] = points;

	segs[4].vertex[0] = points + 4; segs[4].vertex[1] = points + 5;
	Plot(segs, 5);*/
	return 0; 
}