#include "memManager.h"

Point * MemManager::NewPoint(float x = 0, float y = 0)
{
	Point * tmp = pointPool->NewObj();
	tmp->x = x;
	tmp->y = y;
	return tmp;
}

Rect * MemManager::NewPoint(Point * a = 0, Point * b = 0, Point * c = 0, Point * d =0, size = 0, age = 0)
{
	Rect * tmp = rectsPool->NewObj();
	tmp->vertex[0] = a;
	tmp->vertex[1] = b;
	tmp->vertex[2] = c;
	tmp->vertex[3] = d;
	tmp->size = size;
	tmp->age = age;
	firstNeighb = 0;
	firstSeg = 0;
	return tmp;
}

NodeBack<Rect *> * MemManager::NewNodeRect(Rect * data = 0, NodeBack<Rect *> * prev = 0)
{
	NodeBack<Rect *> * tmp = nodeRectPool->NewObj();
	tmp->data = data;
	tmp->prev = prev;
	return tmp;
}

Segment * MemManager::NewSegment(Point * a = 0, Point * b = 0)
{
	Segment * tmp = segPool->NewObj();
	tmp->vertex[0] = a;
	tmp->vertex[1] = b;
	tmp->fullInfo = false;
	return tmp;
}

NodeBack<Segment *> * MemManager::NewNodeSeg(Segment * data = 0, NodeBack<Segment *> * prev = 0)
{
	NodeBack<Segment *> * tmp = nodeSegPool->NewObj();
	tmp->data = data;
	tmp->prev = prev;
	return tmp;
}

void MemManager::DelObjPoint(Point * point)
{
	pointPool->DelObj(point);

}

void MemManager::DelObjRect(Rect * rect)
{
	rectsPool->DelObj(rect);
}

void MemManager::DelObjNodeRect(NodeBack<Rect *> * nodeRect)
{
	nodeRectPool->DelObj(nodeRect);
}

void MemManager::DelObjSeg(Segment * seg)
{
	segPool->DelObj(seg);
}

void MemManager::DelObjNodeSeg(NodeBack<Segment * > * nodeSeg)
{
	nodeSegPool->DelObj(nodeSeg);
}

MemManager::MemManager()
{
	rectsPool.SetSize(1000);
	pointPool.SetSize(1000);
	nodeRectPool.SetSize(1000);
	SegPool.SetSize(1000);
	nodeSegPool.SetSize(1000);
}