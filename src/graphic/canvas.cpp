#include "canvas.h"
#include <iostream>

using namespace std;

inline float Max(float a, float b)
{
	return (a>b)?a:b;
}

inline float Min(float a, float b)
{
	return (a<b)?a:b;
}


Canvas::Canvas(Segment * _segToPlot, int _segCnt)
{
	segToPlot = _segToPlot;
	segCnt = _segCnt;
	rightTop.x = segToPlot[0].vertex[0]->x;
	rightTop.x = Max(segToPlot[0].vertex[0]->x, segToPlot[0].vertex[1]->x);
	rightTop.y = Max(segToPlot[0].vertex[0]->y, segToPlot[0].vertex[1]->y);
	leftBottom.x = Min(segToPlot[0].vertex[0]->x, segToPlot[0].vertex[1]->x);
	leftBottom.y = Min(segToPlot[0].vertex[0]->y, segToPlot[0].vertex[1]->y);
	for (int i = 1; i < segCnt; ++i)
	{
		rightTop.x = Max(rightTop.x, Max(segToPlot[i].vertex[0]->x, segToPlot[i].vertex[1]->x));
		rightTop.y = Max(rightTop.y, Max(segToPlot[i].vertex[0]->y, segToPlot[i].vertex[1]->y));
		leftBottom.x = Min(leftBottom.x, Min(segToPlot[i].vertex[0]->x, segToPlot[i].vertex[1]->x));
		leftBottom.y = Min(leftBottom.y, Min(segToPlot[i].vertex[0]->y, segToPlot[i].vertex[1]->y));
	}
}

