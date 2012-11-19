#ifndef _RECT_H_
#define _RECT_H_

#include <usesTypes.h>
#include <node.h>

struct Rect
{
	Point * vertex[4];
	float size;
	int age;

	NodeBack<Rect *> * firstNeighb;

	NodeBack<Segment *> * firstSeg;
	void Print();
};

bool IsPointInRect(Rect * rect, Point * point);

bool IsNeigbhour(Rect * rect1, Rect * rect2);

bool IsSegmetCrosRect(Rect * rect, Segment * seg);
#endif
