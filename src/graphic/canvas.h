#ifndef _CANVAS_
#define _CANVAS_

#include <usesTypes.h>

class Canvas
{
public:
	Segment * segToPlot;
	int segCnt;

	Point rightTop;
	Point leftBottom;
	
	Canvas(Segment * _segToPlot, int _segCnt);
};

#endif