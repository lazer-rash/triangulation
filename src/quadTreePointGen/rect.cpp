#include "rect.h"
#include <mathUtils.h>

void Rect::Print()
{
	cout<<"  age: "<<age<<endl;
	cout<<"  size: "<<size<<endl;
	cout<<"  vertexes: "<<endl;
	cout<<"  ";
	for(int i = 0; i < 4; i++)
	{
		vertex[i]->Print();
		cout<<"  ";
	}
	cout<<endl;	

	NodeBack<Segment *> * tmp = firstSeg;
	cout<<"  borders: "<<endl;
	cout<<"  ";
	while(tmp != 0)
	{
		tmp->data->Print();
		tmp = tmp->prev;
		cout<<endl<<"  ";
	}
	cout<<endl;	
}

bool IsPointInRect(Rect * rect, Point * point)
{
	if ((rect->vertex[0]->x > point->x) || (rect->vertex[1]->x < point->x))
		return false;
	if ((rect->vertex[3]->y > point->y) || (rect->vertex[0]->y < point->y))
		return false;
	return true;
}

bool IsNeigbhour(Rect * rect1, Rect * rect2)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (rect1->vertex[i] == rect1->vertex[j])
				return true;
		}
	}

	float w1, h1, w2, h2, ch, cw;
	w1 = rect1->vertex[1]->x - rect1->vertex[0]->x;
	h1 = rect1->vertex[1]->y - rect1->vertex[2]->y;
	w2 = rect1->vertex[1]->x - rect1->vertex[0]->x;
	h2 = rect1->vertex[1]->y - rect1->vertex[2]->y;
	cw = fabs(rect1->vertex[3]->x - rect2->vertex[3]->x + 0.5 * (w1 -w2));
	ch = fabs(rect1->vertex[3]->y - rect2->vertex[3]->y + 0.5 * (h1 -h2));
	if((cw <=(w1 + w2)) && ((ch <=(h1 + h2)))) return true;
	return false;
}

bool IsSegmetCrosRect(Rect * rect,Segment * seg)
{
	if(IsPointInRect(rect, seg->vertex[0]) || IsPointInRect(rect, seg->vertex[1]))
	return true;

	int sideArr[4];

    //ay + bx + c == 0
	seg->CalculSegEq();

	//положение вершин относительно отрезка
	float tmp;
	for (int i = 0; i < 4; ++i)
	{
		tmp = seg->a * rect->vertex[i]->y + seg->b * rect->vertex[i]->x + seg->c;	
	 	sideArr[i] = Sign(tmp);
	}

	bool btmp = true; // по одну сторону?
	for (int i = 0; i < 3; ++i)
	{
	 	if(sideArr[3] != sideArr[i])
	 	{
	 		btmp =false;
	 		break;
	 	}
	}
	if(btmp) return false;

	//расположенение концов отрезка тносительно ребра 3-0
	float tmp1 = seg->vertex[0]->x - rect->vertex[0]->x;
	tmp = seg->vertex[1]->x - rect->vertex[0]->x;
	sideArr[0] = (Sign(tmp1) == Sign(tmp));
	//1-2
	tmp1 = seg->vertex[0]->x - rect->vertex[2]->x;
	tmp = seg->vertex[1]->x - rect->vertex[2]->x;
	sideArr[2] = (Sign(tmp1) == Sign(tmp));
	//0-1
    tmp1 = seg->vertex[0]->y - rect->vertex[0]->y;
	tmp = seg->vertex[1]->y - rect->vertex[0]->y;
	sideArr[1] = (Sign(tmp1) == Sign(tmp));
	//2-3
	tmp1 = seg->vertex[0]->y - rect->vertex[2]->y;
	tmp = seg->vertex[1]->y - rect->vertex[2]->y;
	sideArr[3] = (Sign(tmp1) == Sign(tmp));

	int sum = 0;
	for (int i = 0; i < 4; ++i)
	{
		sum +=sideArr[i];
	}
	if (sum < 3) return true;
	return false;
}
