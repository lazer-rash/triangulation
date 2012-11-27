#include <iostream>
#include <quadTreePointGen.h>
#include "usesTypes.h"
#include <node.h>
#include <graphic.h>
#include <rect.h>
#include <memManager.h>

using namespace std;

int main(int argc, char *argv[])
{
	MemManager memory; 
	int pointCnt = 6;
	Point ** point = (Point **) malloc(pointCnt * sizeof(Point*));
	point[0] = memory.NewPoint(1, 1);
	point[1] = memory.NewPoint(5, 0);
	point[2] = memory.NewPoint(10, 2);
	point[3] = memory.NewPoint(7, 10);
	point[4] = memory.NewPoint(1, 9);
	point[5] = memory.NewPoint(0, 4);
	QuadroMesh mesh(&memory, point, pointCnt, 0.0001);
	mesh.Start();
	Segment * segToPlot;
	int  segCnt;
	mesh.GetSegments(&segToPlot, &segCnt);
	Plot(segToPlot, segCnt);
	return 0; 
}