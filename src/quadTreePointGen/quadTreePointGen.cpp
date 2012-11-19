#include "quadTreePointGen.h"
#include "rect.h"
#include <stdlib.h>
#include <iostream>
#include <mathUtils.h>

using namespace std;

void QuadroMesh::SplitRect()
{	
	int i;
	Point * center = pointPool.NewObj();
	Point * newPoints[4] = {pointPool.NewObj(), pointPool.NewObj(), pointPool.NewObj(), pointPool.NewObj()}; 
	center->x = (firstRect->data->vertex[0]->x + firstRect->data->vertex[1]->x) / 2;
	center->y = (firstRect->data->vertex[1]->y + firstRect->data->vertex[2]->y) / 2;
	newPoints[0]->x = center->x; newPoints[0]->y = firstRect->data->vertex[0]->y;
	newPoints[1]->y = center->y; newPoints[1]->x = firstRect->data->vertex[1]->x;
	newPoints[2]->x = center->x; newPoints[2]->y = firstRect->data->vertex[2]->y;
	newPoints[3]->y = center->y; newPoints[3]->x = firstRect->data->vertex[3]->x;

	Rect * newRects[4];
	for (i = 0; i < 4; ++i)
	{
		newRects[i] = rectsPool.NewObj();
		newRects[i]->age = firstRect->data->age + 1;
		newRects[i]->size = firstRect->data->size / 2;
		newRects[i]->firstNeighb = NULL;
		newRects[i]->firstSeg = NULL;
		AddRect(newRects[i]);
	}

	newRects[0]->vertex[0] = firstRect->data->vertex[0];
	newRects[0]->vertex[1] = newPoints[0];
	newRects[0]->vertex[2] = center;
	newRects[0]->vertex[3] = newPoints[3];

	newRects[1]->vertex[0] = newPoints[0];
	newRects[1]->vertex[1] = firstRect->data->vertex[1];
	newRects[1]->vertex[2] = newPoints[1];
	newRects[1]->vertex[3] = center;

	newRects[2]->vertex[0] = center;
	newRects[2]->vertex[1] = newPoints[1];
	newRects[2]->vertex[2] = firstRect->data->vertex[2];
	newRects[2]->vertex[3] = newPoints[2];

	newRects[3]->vertex[0] = newPoints[3];
	newRects[3]->vertex[1] = center;
	newRects[3]->vertex[2] = newPoints[2];
	newRects[3]->vertex[3] = firstRect->data->vertex[3];

	NodeBack<Segment *> * tmp = firstRect->data->firstSeg;
	while(tmp != NULL)
	{
		for (i = 0; i < 4; ++i)
		{
			if (IsSegmetCrosRect(newRects[i], tmp->data))
				AddSegment(newRects[i], tmp->data);
		}
		tmp = tmp->prev;
	}

	AddNeighbour(newRects[0],newRects[1]);
	AddNeighbour(newRects[0],newRects[2]);
	AddNeighbour(newRects[0],newRects[3]);
	AddNeighbour(newRects[1],newRects[2]);
	AddNeighbour(newRects[1],newRects[3]);
	AddNeighbour(newRects[2],newRects[3]);
	NodeBack<Rect *> * tmpRect = firstRect->data->firstNeighb;
	while(tmpRect != NULL)
	{
		for (i = 0; i < 4; ++i)
		{
			if (IsNeigbhour(newRects[i], tmpRect->data))
			{
				AddNeighbour(newRects[i], tmpRect->data);
			}
		}
		tmpRect = tmpRect->prev;
	}
	BreakReference(firstRect->data);
	RemoveRect();
}

bool QuadroMesh::Check()
{
	return firstRect->data->size >= stopLength;
}

void QuadroMesh::AddSegment(Rect * rect, Segment * seg)
{
	NodeBack<Segment * > * tmp = nodeSegPool.NewObj();
	tmp->NodeInit(seg);
	tmp->prev = rect->firstSeg;
	rect->firstSeg = tmp;
}

void QuadroMesh::AddNeighbour(Rect * rect1, Rect * rect2)
{
	NodeBack<Rect *> * tmp1 = nodeRectPool.NewObj();
	NodeBack<Rect *> * tmp2 = nodeRectPool.NewObj();
	tmp1->NodeInit(rect1);
	tmp2->NodeInit(rect2);
	tmp1->prev = rect2->firstNeighb;
	rect2->firstNeighb = tmp1;
	tmp2->prev = rect1->firstNeighb;
	rect1->firstNeighb = tmp2;
}

void QuadroMesh::BreakReference(Rect * rect)
{
	NodeBack<Rect *> * tmp = rect->firstNeighb;
	NodeBack<Rect *> * tmp2;
	NodeBack<Rect *> * tmp3;
	while(tmp != NULL)
	{
		tmp2 = tmp->data->firstNeighb;
		while(tmp2 != NULL)
		{
			if(rect == tmp2->data) 
			{
				if (tmp2 == tmp->data->firstNeighb)
				{
					tmp->data->firstNeighb = tmp2->prev;
				}
				else
				{
					tmp3->prev = tmp2->prev;
				}
				nodeRectPool.DelObj(tmp2);
				break;
			}		
			tmp3 = tmp2;
			tmp2 = tmp2 -> prev;	
		}
		tmp = tmp->prev;
	}
}

void QuadroMesh::AddRect(Rect * rect)
{
	lastRect->prev = nodeRectPool.NewObj();
	lastRect->prev ->NodeInit(rect);
	lastRect = lastRect->prev;
}

void QuadroMesh::RemoveRect()
{
	FreeRect(firstRect->data);
	NodeBack<Rect *> * tmp = firstRect->prev;
	rectsPool.DelObj(firstRect->data);
	nodeRectPool.DelObj(firstRect);
	firstRect = tmp;
}

void QuadroMesh::FreeRect(Rect * rect)
{
	NodeBack<Rect *> * tmpRect = rect->firstNeighb;
	while(tmpRect != NULL)
	{
		nodeRectPool.DelObj(tmpRect);
		tmpRect = tmpRect->prev;
	}

	NodeBack<Segment *> * tmpSeg = rect->firstSeg;
	while(tmpSeg != NULL)
	{
		nodeSegPool.DelObj(tmpSeg);
		tmpSeg = tmpSeg->prev;
	}	
}

QuadroMesh::QuadroMesh(Point * pointsList, int count, float _stopLength)
{
	stopLength = _stopLength;
	rectsPool.SetSize(1000);
	pointPool.SetSize(1000);
	nodeRectPool.SetSize(1000);
	nodeSegPool.SetSize(1000);

	Rect * tmp = rectsPool.NewObj();
	for (int i = 0; i < 4; ++i) tmp->vertex[i] = pointPool.NewObj();
	firstRect = nodeRectPool.NewObj();
	firstRect->NodeInit(tmp);
	tmp->age = 0;		
	border = new Segment[count];
	borderCnt = count;

	tmp->vertex[1]->x = pointsList[count - 1].x;
	tmp->vertex[1]->y = pointsList[count - 1].y;
	tmp->vertex[3]->x = tmp->vertex[1]->x;
	tmp->vertex[3]->y = tmp->vertex[1]->y;
	for (int i = 0; i < count - 1; ++i)
	{
		border[i].vertex[0] = pointsList + i;
		border[i].vertex[1] = pointsList + i + 1;
		border[i].fullInfo = false;
		tmp->vertex[1]->x = Max(tmp->vertex[1]->x, pointsList[i].x);
		tmp->vertex[1]->y = Max(tmp->vertex[1]->y, pointsList[i].y);
		tmp->vertex[3]->x = Min(tmp->vertex[3]->x, pointsList[i].x);
		tmp->vertex[3]->y = Min(tmp->vertex[3]->y, pointsList[i].y);
	}
	tmp->vertex[0]->x = tmp->vertex[3]->x;
	tmp->vertex[0]->y = tmp->vertex[1]->y;
	tmp->vertex[2]->x = tmp->vertex[1]->x;
	tmp->vertex[2]->y = tmp->vertex[3]->y;
	tmp->size = Min(-tmp->vertex[0]->x + tmp->vertex[1]->x,  tmp->vertex[0]->y - tmp->vertex[3]->y);

	border[count - 1].vertex[0] = pointsList + count - 1;
	border[count - 1].vertex[1] = pointsList;
	border[count - 1].fullInfo = false;
	tmp->firstNeighb = NULL;
	tmp->firstSeg = NULL;
	NodeBack<Segment *> * tmp1;
	for (int i = 0; i < count; ++i)
	{
		tmp1 = tmp->firstSeg;
		tmp->firstSeg = nodeSegPool.NewObj();
		tmp->firstSeg->NodeInit(border + i);
		tmp->firstSeg->prev = tmp1;
	}
	buffer = 0;
	bufCnt = 0;
}

QuadroMesh::~QuadroMesh()
{
	delete[] border;
}

void QuadroMesh::Start()
{
	lastRect = firstRect;
	while(Check())
		{
			if(firstRect->data->firstSeg == 0)
			{
				Jump();
			}
			else SplitRect();
		}

}

void QuadroMesh::PrintInfo()
{
	cout<<"RectInfo"<<endl;
	NodeBack<Rect *> * tmp = firstRect;
	int i = 0;
	while(tmp != 0)
	{
		cout<<"Rect "<<i<<endl;
		tmp->data->Print();
		tmp = tmp->prev;
	}
}

void QuadroMesh::GetSegments(Segment **resSeg, int * count)
{
	/* *count = 0;
	NodeBack<Rect *> * tmp = firstRect;
	while(tmp != 0)
	{
		(*count)++;
		tmp = tmp->prev;
	}
	(*count) +=bufCnt;
	(*count)*=4;
	(*resSeg) = new Segment[*count];
	tmp = firstRect;
	lastRect->prev = buffer;
	int i = 0;
	int j;
	while(tmp != 0)
	{
		for (j = 0; j < 3; ++j)
		{
			(*resSeg)[i + j].vertex[0] = new Point(tmp->data->vertex[j]->x,tmp->data->vertex[j]->y);
			(*resSeg)[i + j].vertex[1] = new Point(tmp->data->vertex[j + 1]->x,tmp->data->vertex[j + 1]->y);
		}
		(*resSeg)[i + 3].vertex[0] = new Point(tmp->data->vertex[3]->x,tmp->data->vertex[3]->y);
		(*resSeg)[i + 3].vertex[1] = new Point(tmp->data->vertex[0]->x,tmp->data->vertex[0]->y);
		i+= 4;
		tmp = tmp->prev;
	}

	*/
	RmOuter();
	(*count) = 4 * (bufCnt + buf2Cnt);
	NodeBack<Rect *> * tmp = firstRect;
	(*resSeg) = new Segment[*count];
	tmp = buffer;
	int i = 0;
	int j;
	while(tmp != 0)
	{
		for (j = 0; j < 3; ++j)
		{
			(*resSeg)[i + j].vertex[0] = new Point(tmp->data->vertex[j]->x,tmp->data->vertex[j]->y);
			(*resSeg)[i + j].vertex[1] = new Point(tmp->data->vertex[j + 1]->x,tmp->data->vertex[j + 1]->y);
		}
		(*resSeg)[i + 3].vertex[0] = new Point(tmp->data->vertex[3]->x,tmp->data->vertex[3]->y);
		(*resSeg)[i + 3].vertex[1] = new Point(tmp->data->vertex[0]->x,tmp->data->vertex[0]->y);
		i+= 4;
		tmp = tmp->prev;
	}
	tmp = buffer2;
	while(tmp != 0)
	{
		for (j = 0; j < 3; ++j)
		{
			(*resSeg)[i + j].vertex[0] = new Point(tmp->data->vertex[j]->x,tmp->data->vertex[j]->y);
			(*resSeg)[i + j].vertex[1] = new Point(tmp->data->vertex[j + 1]->x,tmp->data->vertex[j + 1]->y);
		}
		(*resSeg)[i + 3].vertex[0] = new Point(tmp->data->vertex[3]->x,tmp->data->vertex[3]->y);
		(*resSeg)[i + 3].vertex[1] = new Point(tmp->data->vertex[0]->x,tmp->data->vertex[0]->y);
		i+= 4;
		tmp = tmp->prev;
	}
}

void QuadroMesh::Jump()
{	
	bufCnt++;
	NodeBack<Rect *> *  tmp = firstRect->prev;
	firstRect -> prev = buffer;
	buffer = firstRect;
	firstRect = tmp;
}

void QuadroMesh::RmOuter()
{
	NodeBack<Rect *> * tmp1 = firstRect;
	NodeBack<Rect *> * tmp2;
	buffer2 = 0;
	buf2Cnt = 0;
	while(tmp1 != 0)
	{
		tmp2 = tmp1->prev;
		if(tmp1->data->firstSeg == 0)
			{tmp1->prev = buffer; buffer = tmp1; bufCnt++;}
		else
			{tmp1->prev = buffer2; buffer2 = tmp1; buf2Cnt++;}
		tmp1 = tmp2;
	}
	tmp1 = buffer;
	tmp2 = buffer;
	bool enterRect;
//	char c;
	while(tmp1 != 0)
	{
		//cin>>c;
		//tmp1->data->Print();
		//cout<<endl;
		enterRect = true;
		for (int i = 0; i < borderCnt; ++i)
		{

			//cout<<endl;
			if(Sign(border[i].a * tmp1->data->vertex[0]->y + 
				border[i].b * tmp1->data->vertex[0]->x +
				border[i].c) != -1)
			{
				enterRect = false;
				bufCnt--;
				//cout<<"[1.1]"<<endl;
				BreakReference(tmp1->data);
				//cout<<"[1.2]"<<endl;
				FreeRect(tmp1->data);
				//cout<<"[1.3]"<<endl;
				rectsPool.DelObj(tmp1->data);
				if(tmp1 == buffer)
				{
					//cout<<"[1.3.1]"<<endl;
					tmp1 = tmp1->prev;		
					nodeRectPool.DelObj(buffer);
					buffer = tmp1;
					tmp2 = tmp1;
				}
				else
				{
					//cout<<"[1.3.2]"<<endl;
					tmp2->prev = tmp1->prev;
					nodeRectPool.DelObj(tmp1);
					tmp1 = tmp2->prev;
				}
			//	cout<<"[1.4]"<<endl;
				break;
			}
		}
		if(enterRect)
		{
			//cout<<"[2]"<<endl;
			if(tmp1 != buffer)
			{
				tmp2->prev = tmp1;
				tmp2 = tmp1;
			}
				tmp1 = tmp1->prev;
		}
	}
	cout<<"free "<<bufCnt<<endl<<"Segment "<<buf2Cnt<<endl;

}