#include "quadTreePointGen.h"
#include "rect.h"
#include <stdlib.h>
#include <iostream>
#include <mathUtils.h>
#include <math.h>

using namespace std;


QuadroMesh::QuadroMesh(MemManager * _memMan, Point ** _border, int _borderCount, float _stopLength)
{
	cout<<"QuadroMesh"<<endl;
	memManager = _memMan;
	stopLength = _stopLength;
	CalcBorder(_border, _borderCount);
	FirstRect(_border, _borderCount);
	rectPureCnt = 0;
	rectPureE = rectPureF = 0;
	rectSegCnt = 1;
}

QuadroMesh::~QuadroMesh()
{
	for (int i = 0; i < borderCnt; ++i)
		memManager->DelObjSeg(border[i]);
	delete[] border;
}

void QuadroMesh::GetSegments(Segment **resSeg, int * count)
{
	cout<<"GetSegments"<<endl;
	(*count) = 4 * (rectPureCnt + rectSegCnt);
	(*resSeg) = new Segment[*count];
	int i = 0;
	int j;
	NodeBack<Rect *> * tmp = rectPureF;
	while(tmp != 0)
	{
		for (j = 0; j < 3; ++j)
		{
			(*resSeg)[i + j].vertex[0] = memManager->NewPoint(tmp->data->vertex[j]->x,tmp->data->vertex[j]->y);
			(*resSeg)[i + j].vertex[1] = memManager->NewPoint(tmp->data->vertex[j + 1]->x,tmp->data->vertex[j + 1]->y);
		}
		(*resSeg)[i + 3].vertex[0] = memManager->NewPoint(tmp->data->vertex[3]->x,tmp->data->vertex[3]->y);
		(*resSeg)[i + 3].vertex[1] = memManager->NewPoint(tmp->data->vertex[0]->x,tmp->data->vertex[0]->y);
		i+= 4;
		tmp = tmp->prev;
	}
	tmp = rectSegF;
	while(tmp != 0)
	{
		for (j = 0; j < 3; ++j)
		{
			(*resSeg)[i + j].vertex[0] = memManager->NewPoint(tmp->data->vertex[j]->x,tmp->data->vertex[j]->y);
			(*resSeg)[i + j].vertex[1] = memManager->NewPoint(tmp->data->vertex[j + 1]->x,tmp->data->vertex[j + 1]->y);
		}
		(*resSeg)[i + 3].vertex[0] = memManager->NewPoint(tmp->data->vertex[3]->x,tmp->data->vertex[3]->y);
		(*resSeg)[i + 3].vertex[1] = memManager->NewPoint(tmp->data->vertex[0]->x,tmp->data->vertex[0]->y);
		i+= 4;
		tmp = tmp->prev;
	}
}

void QuadroMesh::FirstRect(Point ** pointsList, int count)
{
	cout<<"FirstRect"<<endl;
	Point * vertexes[4];
	for (int i = 0; i < 4; ++i) vertexes[i] = memManager->NewPoint();

	Rect * tmpRect = memManager->NewRect(vertexes[0], vertexes[1], vertexes[2], vertexes[3]);
	rectSegF = rectSegE = memManager->NewNodeRect(tmpRect);//инициализация списка прямоугольников
	//вершины первого прямоугольника
	CalcEdges(tmpRect->vertex[1], tmpRect->vertex[3], pointsList, count);
	tmpRect->vertex[0]->x = tmpRect->vertex[3]->x;
	tmpRect->vertex[0]->y = tmpRect->vertex[1]->y;
	tmpRect->vertex[2]->x = tmpRect->vertex[1]->x;
	tmpRect->vertex[2]->y = tmpRect->vertex[3]->y;
	//остальные параметры
	tmpRect->age = 0;
	tmpRect->size = Min(-tmpRect->vertex[0]->x + tmpRect->vertex[1]->x,  tmpRect->vertex[0]->y - tmpRect->vertex[3]->y);
	//добавление отрезков границы в прямоугольник
	NodeBack<Segment *> * tmpSeg;
	for (int i = 0; i < count; ++i)
	{
		tmpSeg = memManager->NewNodeSeg(border[i], tmpRect->firstSeg);
		tmpRect->firstSeg = tmpSeg;
	}
}

void QuadroMesh::CalcEdges(Point * p1, Point * p3, Point ** pointsList, int count)
{
	p1->x = p3->x = pointsList[0]->x;
	p1->y = p3->y = pointsList[0]->y;
	for (int i = 1; i < count; ++i)
	{
		p1->x = Max(pointsList[i]->x, p1->x);
		p1->y = Max(pointsList[i]->y, p1->y);
		p3->x = Min(pointsList[i]->x, p3->x);
		p3->y = Min(pointsList[i]->y, p3->y);
	}
	borderCnt = count;
}

void QuadroMesh::CalcBorder(Point ** _border, int _borderCount)
{
	border = new Segment * [_borderCount];
	for (int i = 0; i < _borderCount - 1; ++i)
		border[i] = memManager->NewSegment(_border[i], _border[i + 1]);
	border[_borderCount - 1] = memManager->NewSegment(_border[_borderCount - 1], _border[0]);
}

bool QuadroMesh::IsCountinue()
{
	return rectSegF->data->size > stopLength;
}

void QuadroMesh::Start()
{
	cout<<"Start"<<endl;
	while(IsCountinue())
	{
		SplitRect(rectSegF->data);
		RemoveRect();
	}
	Smoth();
	cout<<"Rects count "<<(rectPureCnt + rectSegCnt)<<endl;
}

void QuadroMesh::SplitRect(Rect * rect)
{	
	int i;
	//определение вершин новых прямоугольников
	Point * center = memManager->NewPoint();
	Point * newPoints[4] = {memManager->NewPoint(), memManager->NewPoint(), memManager->NewPoint(), memManager->NewPoint()}; 
	center->x = (rect->vertex[0]->x + rect->vertex[1]->x) / 2;
	center->y = (rect->vertex[1]->y + rect->vertex[2]->y) / 2;
	newPoints[0]->x = center->x; newPoints[0]->y = rect->vertex[0]->y;
	newPoints[1]->y = center->y; newPoints[1]->x = rect->vertex[1]->x;
	newPoints[2]->x = center->x; newPoints[2]->y = rect->vertex[2]->y;
	newPoints[3]->y = center->y; newPoints[3]->x = rect->vertex[3]->x;
	Rect * newRects[4];
	for (i = 0; i < 4; ++i)
	{
		newRects[i] = memManager->NewRect();
		newRects[i]->age = rect->age + 1;
		newRects[i]->size = rect->size / 2;
		newRects[i]->firstNeighb = NULL;
		newRects[i]->firstSeg = NULL;
	}
	newRects[0]->vertex[0] = rect->vertex[0];
	newRects[0]->vertex[1] = newPoints[0];
	newRects[0]->vertex[2] = center;
	newRects[0]->vertex[3] = newPoints[3];

	newRects[1]->vertex[0] = newPoints[0];
	newRects[1]->vertex[1] = rect->vertex[1];
	newRects[1]->vertex[2] = newPoints[1];
	newRects[1]->vertex[3] = center;

	newRects[2]->vertex[0] = center;
	newRects[2]->vertex[1] = newPoints[1];
	newRects[2]->vertex[2] = rect->vertex[2];
	newRects[2]->vertex[3] = newPoints[2];

	newRects[3]->vertex[0] = newPoints[3];
	newRects[3]->vertex[1] = center;
	newRects[3]->vertex[2] = newPoints[2];
	newRects[3]->vertex[3] = rect->vertex[3];

	NodeBack<Segment *> * tmp = rect->firstSeg;
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

	NodeBack<Rect *> * tmpRect = rect->firstNeighb;
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

	for (int i = 0; i < 4; ++i)
	{
		AddRect(newRects[i], rect);
	}
}

void QuadroMesh::AddRect(Rect * rect, Rect * root)
{
	if(rect->firstSeg == 0)
	{
		if((root->firstSeg != 0) and (!IsRectInRegion(rect)))
		{
			FreeNodeInRect(rect);
			memManager->DelObjRect(rect);
			return;
		}
		if(rectPureE == 0) rectPureE = rectPureF = memManager->NewNodeRect(rect);
		else 
		{
			rectPureE->prev = memManager->NewNodeRect(rect);
			rectPureE = rectPureE->prev;
		}
		rectPureCnt++;
	}
	else
	{
		if(rectSegE == 0) rectSegE = memManager->NewNodeRect(rect);
		else 
		{
			rectSegE->prev = memManager->NewNodeRect(rect);
			rectSegE = rectSegE->prev;
		}
		rectSegCnt++;
	}
}

void QuadroMesh::AddSegment(Rect * rect, Segment * seg)
{
	NodeBack<Segment * > * tmp = memManager->NewNodeSeg(seg, rect->firstSeg);
	rect->firstSeg = tmp;
}

void QuadroMesh::AddNeighbour(Rect * rect1, Rect * rect2)
{
	NodeBack<Rect *> * tmp1 = memManager->NewNodeRect(rect1);
	NodeBack<Rect *> * tmp2 = memManager->NewNodeRect(rect2);
	tmp1->prev = rect2->firstNeighb;
	rect2->firstNeighb = tmp1;
	tmp2->prev = rect1->firstNeighb;
	rect1->firstNeighb = tmp2;
}

void QuadroMesh::FreeNodeInRect(Rect * rect)
{
	BreakNeihbRef(rect);
	NodeBack<Rect *> * tmpRect = rect->firstNeighb;
	while(tmpRect != 0)
	{
		memManager->DelObjNodeRect(tmpRect);
		tmpRect = tmpRect->prev;
	}

	NodeBack<Segment *> * tmpSeg = rect->firstSeg;
	while(tmpSeg != 0)
	{
		memManager->NewNodeSeg(tmpSeg->data);
		tmpSeg = tmpSeg->prev;
	}	
}

void QuadroMesh::RemoveRect()
{
	FreeNodeInRect(rectSegF->data);
	NodeBack<Rect *> * tmp = rectSegF->prev;
	memManager->DelObjRect(rectSegF->data);
	memManager->DelObjNodeRect(rectSegF);
	rectSegF = tmp;
	rectSegCnt--;
}

bool QuadroMesh::IsRectInRegion(Rect * rect)
{
	for (int i = 0; i < borderCnt; ++i)
		if(Sign(border[i]->a * rect->vertex[0]->y + 
				border[i]->b * rect->vertex[0]->x +
				border[i]->c) != -1)
			return false;
	return true;
}

void QuadroMesh::Smoth()
{
	cout<<"Smoth"<<endl;
	NodeBack<Rect *> * tmp = rectPureF;
	NodeBack<Rect *> * tmp2 = rectPureF;
	bool allright = true;
	while(tmp != 0)
	{
		if(!CheckRect(tmp->data))
		{
			allright = false;
			rectPureCnt--;
			SplitRect(tmp->data);
			FreeNodeInRect(tmp->data);
			if(tmp == rectPureF)
			{
				rectPureF = tmp->prev;
				memManager->DelObjRect(tmp->data);
				memManager->DelObjNodeRect(tmp);
				tmp = rectPureF;
				tmp2 = rectPureF;
			}
			else
			{
				tmp2->prev = tmp->prev;
				memManager->DelObjRect(tmp->data);
				memManager->DelObjNodeRect(tmp);
				tmp = tmp2->prev;
			}
		}
		else
		{
			tmp2 = tmp;
			tmp = tmp2->prev;
		}
		if((tmp == 0) && (!allright)) {tmp = tmp2 = rectPureF; allright = true;}
	}
}

bool QuadroMesh::CheckRect(Rect *  rect)
{
	NodeBack<Rect *> * tmp = rect->firstNeighb;
	while(tmp != 0)
	{
		if(rect->age - tmp->data->age < -1) 
			return false;
		tmp = tmp->prev; 
	}
	return true;
}


void QuadroMesh::BreakNeihbRef(Rect * rect)
{
	NodeBack<Rect *> * tmp = rect->firstNeighb;
	while(tmp != 0)
	{
		//пробегаем по всем соседям и удаляем себя из их списаа соседей
		DropFromNeihb(tmp->data, rect);
		tmp = tmp->prev;
	}
}

//удаление из соседей fromRect ссылку на target
void QuadroMesh::DropFromNeihb(Rect * fromRect, Rect * target)
{
	//если вписок соседей пустой, то выходим
	if(fromRect->firstNeighb == 0) return;

	NodeBack<Rect *> * tmp = fromRect->firstNeighb;
	NodeBack<Rect *> * tmp1 = tmp;
	while(tmp != 0)
	{
		if(tmp->data == target)
		{
			if(tmp == fromRect->firstNeighb)
				fromRect->firstNeighb = fromRect->firstNeighb->prev;
			else
				tmp1->prev = tmp->prev;
			memManager->DelObjNodeRect(tmp);
			return;
		}
		{
			tmp1 = tmp;
			tmp = tmp1->prev;
		}
	}
}

void QuadroMesh::PrintInfo()
{
	cout<<"Pure "<<endl<<"count "<<rectPureCnt<<endl;
	NodeBack<Rect *> * tmp = rectPureF;
	while(tmp != 0)
	{
		tmp->data->Print();
		cout<<endl;
		tmp = tmp->prev;
	}

	cout<<"Seg "<<endl<<"count "<<rectSegCnt<<endl;
	tmp = rectSegF;
	while(tmp != 0)
	{
		tmp->data->Print();
		cout<<endl;
		tmp = tmp->prev;
	}
}