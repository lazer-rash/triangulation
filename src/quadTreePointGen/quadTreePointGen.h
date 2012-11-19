#ifndef _QUADRO_MESH_H_
#define _QUADRO_MESH_H_

#include <usesTypes.h>
#include <node.h>
#include <objectPool.h>
#include <rect.h>

class QuadroMesh
{
private:
	ObjectPool<Rect> rectsPool;
	ObjectPool<Point> pointPool;
	ObjectPool<NodeBack<Rect *> > nodeRectPool;
	ObjectPool<NodeBack<Segment *> > nodeSegPool;

	NodeBack<Rect *> * firstRect;
	NodeBack<Rect *> * lastRect;

	NodeBack<Rect *> * buffer; //прямоугольники без ребер 
	int bufCnt;

	NodeBack<Rect *> * buffer2;//прямоугольники с ребрами 
	int buf2Cnt;

	Segment * border;
	int borderCnt;

	float stopLength;

	void SplitRect();
	bool Check();
	void AddSegment(Rect * rect, Segment * seg);
	void AddNeighbour(Rect * rect1, Rect * rect2);
	void BreakReference(Rect * rect);
	void AddRect(Rect * rect);
	void FreeRect(Rect * rect);
	void RemoveRect();
	void Jump();
	void RmOuter();
public:
	void PrintInfo();
	void Start();
	void GetSegments(Segment ** resSeg, int * count);
	QuadroMesh(Point * pointsList, int count, float _stopLength);
	~QuadroMesh();
};

#endif
