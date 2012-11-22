#ifndef _MEM_MANAGER_H_
#define _MEM_MANAGER_H_


//Класс для управления выделением памяти часто удалямых объктов 
class MemManager
{
private:
	ObjectPool<Point> * pointPool;
	ObjectPool<Rect> * rectsPool;
	ObjectPool<NodeBack<Rect *> > * nodeRectPool;
	ObjectPool<Segment> * segPool;
	ObjectPool<NodeBack<Segment *> > * nodeSegPool;


public:
	Point * NewPoint(float x = 0, float y = 0);
	Rect * NewRect(Point * a = 0, Point * b = 0, Point * c = 0, Point * d =0, size = 0, age = 0);
	NodeBack<Rect *> * NewNodeRect(Rect * data = 0, NodeBack<Rect *> * prev = 0);
	Segment * NewSegment(Point * a = 0, Point * b = 0);
	NodeBack<Segment *> * NewNodeSeg(Segment * data = 0, NodeBack<Segment *> * prev = 0);

	void DelObjPoint(Point * point);
	void DelObjRect(Rect * rect);
	void DelObjNodeRect(NodeBack<Rect *> * nodeRect);
	void DelObjSeg(Segment * seg);
	void DelObjNodeSeg(NodeBack<Segment * > * nodeSeg);

	MemManager();
	~MemManager();
};

#endif